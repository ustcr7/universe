#include <stdio.h>
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "move/move.h"
#include "actor_logic.h"
#include "actor_db.h"
#include "actor_mgr.h"
#include "actor.h"
#include "gamesvr_msg_processer.h"
#include "instance_mgr.h"
#include "db/gamesvr_db.h"

static ActorReqHandle *gs_actor_req_handle = NULL;

ActorReqHandle::ActorReqHandle()
{
}

ActorReqHandle* ActorReqHandle::GetSingleInstance()
{
	if (gs_actor_req_handle != NULL)
	{
		return gs_actor_req_handle;
	}
	gs_actor_req_handle = new ActorReqHandle();
	massert_retval(gs_actor_req_handle != NULL, NULL);

	return gs_actor_req_handle;
}
int ActorReqHandle::ActorRegisteReq(u64 conn_id, u64 id, const char *name)
{
	//ActorDbMgr *db_mgr = ActorDbMgr::GetSingleInstance();
	//massert_retval(db_mgr != NULL, ERR_INVALID_PARAM);

	printf("actor %llu registe name:%s\n", id, name);

	Actor actor;
	actor.SetId(id);
	actor.SetConnId(conn_id);
	actor.SetName(name);
	int ret = 0;
	//ret = db_mgr->InsertActorData(&actor);
	//massert_retval(0 == ret, ret);

	ActorDB db_actor;
	actor.DumpToDb(&db_actor);
	GameSvrDbMgr *db_mgr = GameSvrDbMgr::GetInstance();
	db_mgr->InsertActor(&db_actor);


	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProcesser != NULL, ERR_UNKNOWN);
	msgProcesser->SendActorRegisteRsp(conn_id, id, ret);

	return 0;
}

int ActorReqHandle::ActorLoginReq(u64 conn_id, u64 id)
{
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	massert_retval(actor_mgr != NULL, ERR_INVALID_PARAM);
	if (actor_mgr->GetActorById(id) != NULL)
	{
		printf("actor %llu already login\n", id);
		return ERR_ALREADY_EXISTS;
	}

	//ActorDbMgr *db_mgr = ActorDbMgr::GetSingleInstance();
	//massert_retval(db_mgr != NULL, ERR_INVALID_PARAM);


	ActorDB db_actor;
	GameSvrDbMgr *db_mgr = GameSvrDbMgr::GetInstance();
	int ret = db_mgr->QueryActor(id, &db_actor);
	if (ret != 0)
	{
		printf("actor %llu not in db\n", id);
		massert_noeffect(ret == ERR_NOT_FOUND);
		return ERR_NOT_FOUND;
	}
	Actor *actor = actor_mgr->AllocActor(id);
	massert_retval(actor != NULL, ERR_BAD_ALLOC);

	actor->InitFromDb(&db_actor);

	actor->SetConnId(conn_id);

	//从db里读取
	/*
	//初始化属性
	UnitAttr* attr = actor->GetMutableUnitAttr();
	attr->SetAttr(ACTOR_ATTR_HP, 10000); //默认10000血
	attr->SetAttr(ACTOR_ATTR_MP, 10000); //默认10000魔
	attr->SetAttr(ACTOR_ATTR_DAMAGE, 100); //默认100伤害

	//初始化状态
	UnitState* state = actor->GetMutableUnitState();
	state->SetUnitState(UV_UNIT_STATE_ALIVE);
	*/

	//enter instance
	EnterInstanceParam enter_param;
	enter_param.SetMapId(10000);
	InstanceMgr *instanceMgr = InstanceMgr::GetSingleInstance();
	massert_retval(instanceMgr != NULL, ERR_BAD_ALLOC);
	instanceMgr->EnterActor(actor, &enter_param);

	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProcesser != NULL, ERR_UNKNOWN);
	msgProcesser->SendActorLoginRsp(conn_id, id, 0);

	printf("actor %llu login success\n", id);

	return 0;
}

int ActorReqHandle::ActorLogoutReq(u64 conn_id, u64 id)
{
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	massert_retval(actor_mgr != NULL, ERR_INVALID_PARAM);

	Actor *actor = actor_mgr->GetActorById(id);
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	InstanceMgr *instanceMgr = InstanceMgr::GetSingleInstance();
	massert_retval(instanceMgr != NULL, ERR_BAD_ALLOC);
	instanceMgr->LeaveActor(actor);

	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProcesser != NULL, ERR_UNKNOWN);
	msgProcesser->SendActorLogoutRsp(conn_id, id, 0);

	GameSvrDbMgr *db_mgr = GameSvrDbMgr::GetInstance();
	ActorDB db_actor;
	actor->DumpToDb(&db_actor);
	db_mgr->UpdateActor(&db_actor);

	actor_mgr->FreeActor(id);

	printf("actor %llu logout success\n", id);

	return 0;
}

int ActorReqHandle::ActorGetFullData(u64 conn_id, u64 id)
{
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	massert_retval(actor_mgr != NULL, ERR_INVALID_PARAM);

	Actor *actor = actor_mgr->GetActorById(id);
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProcesser != NULL, ERR_UNKNOWN);
	msgProcesser->SendActorFullDataRsp(conn_id, actor);

	printf("actor %llu get full data success\n", id);

	return 0;
}

int ActorReqHandle::ActorMoveReq(u64 conn_id, u64 id, Pos *pos, int pos_cnt)
{
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	massert_retval(actor_mgr != NULL, ERR_INVALID_PARAM);

	Actor *actor = actor_mgr->GetActorById(id);
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	MovePath move_path;
	move_path.path_count = 0;
	for (int i = 0; i < pos_cnt; ++i)
	{
		move_path.paths[i] = pos[i];
		++move_path.path_count;
	}
	actor->SetCurPathIndex(0);
	actor->SetMovePath(&move_path);

	{
		//debug code
		const MovePath *movePath = actor->GetMovePath();
		printf("move path cnt:%d\n", movePath->path_count);
	}

	AddActorMoveTimer(actor, 1);

	return 0;
}

int ActorReqHandle::ActorChatReq(u64 connId, u64 srcActorid, u64 dstActorId, ChatType chatType, const char *content)
{
	massert_retval(content != NULL, ERR_INVALID_PARAM);
	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProcesser != NULL, ERR_UNKNOWN);
	
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	massert_retval(actor_mgr != NULL, ERR_INVALID_PARAM);
	Actor *dst_actor = actor_mgr->GetActorById(dstActorId);
	massert_retval(dst_actor != NULL, ERR_INVALID_PARAM);

	int ret = msgProcesser->SendForwardChatInfo(dst_actor->GetConnId()
									, chatType
									, srcActorid
									, dstActorId
									, content);
	massert_retval(ret == 0, ret);
	return 0;
}

int ActorReqHandle::ActorLearnSpellReq(u64 connId, u64 actor_rid, int spellid)
{
	printf("actor %llu learn spell %d\n", actor_rid, spellid);
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	Actor *actor = actor_mgr->GetActorById(actor_rid);
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	int ret = 0;
	do
	{
		UnitSpellBook *spell_book = actor->GetMutableSpellBook();
		if (spell_book->GetUnitSpellInfoById(spellid) != NULL)
		{
			printf("already learned spell %d\n", spellid);
			ret = ERR_SPELL_ALREADY_LEARNED;
			break;
		}

		UnitSpellInfo spl_info;
		spl_info.SetSpellId(spellid);
		spl_info.SetLastCastTime(0);
		ret = spell_book->AddUnitSpellInfo(&spl_info);
		massert_noeffect(ret == 0);
		printf("actor %llu add spell %d to spellbook ret:%d\n", actor_rid, spellid, ret);
	} while (0);

	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	msgProcesser->SendLearnSpellRsp(connId, spellid, ret);

	return 0;
}

int ActorReqHandle::ActorCastSpellReq(u64 connId, u64 actor_rid, u64 target_rid, int spellid)
{
	//int ret = 0;
	printf("cast spell caster:%llu target:%llu spell:%d\n", actor_rid, target_rid, spellid);
	ActorMgr *actor_mgr = ActorMgr::GetSingleInstance();
	Actor *caster = actor_mgr->GetActorById(actor_rid);
	massert_retval(caster != NULL, ERR_INVALID_PARAM);
	Actor *target = actor_mgr->GetActorById(target_rid);
	massert_retval(target != NULL, ERR_INVALID_PARAM);

	SpellMgr *spl_mgr = SpellMgr::GetInstance();
	return spl_mgr->UvBattleCastSpell(caster, target, spellid);
}