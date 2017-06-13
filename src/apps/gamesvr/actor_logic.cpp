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
	ActorDbMgr *db_mgr = ActorDbMgr::GetSingleInstance();
	massert_retval(db_mgr != NULL, ERR_INVALID_PARAM);

	printf("actor %llu registe name:%s\n", id, name);

	Actor actor;
	actor.SetId(id);
	actor.SetConnId(conn_id);
	actor.SetName(name);
	int ret = 0;
	ret = db_mgr->InsertActorData(&actor);
	massert_retval(0 == ret, ret);

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

	ActorDbMgr *db_mgr = ActorDbMgr::GetSingleInstance();
	massert_retval(db_mgr != NULL, ERR_INVALID_PARAM);
	Actor db_actor;
	int ret = db_mgr->LoadActorData(id, &db_actor);
	if (ret != 0)
	{
		printf("load actor:%llu data failed;%d\n", id, ret);
		return ret;
	}

	Actor *actor = actor_mgr->AllocActor(id);
	massert_retval(actor != NULL, ERR_BAD_ALLOC);

	actor->SetConnId(conn_id);

	//set data
	actor->SetName(db_actor.GetName());
	actor->SetSpeed(100);  //1m/s

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
	ActorDbMgr *db_mgr = ActorDbMgr::GetSingleInstance();
	massert_retval(db_mgr != NULL, ERR_INVALID_PARAM);

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

	db_mgr->UpdateActorData(actor);
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