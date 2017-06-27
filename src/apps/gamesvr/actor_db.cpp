#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "actor.h"
#include "actor_db.h"
#include <map>
#include "db/gamesvr_db.h"

static ActorDbMgr *gs_actor_db_mgr = NULL;

static std::map<u64, Actor> memory_db; //just in memory, not in realy db


ActorDbMgr::ActorDbMgr()
{

}
ActorDbMgr* ActorDbMgr::GetSingleInstance()
{
	if (gs_actor_db_mgr != NULL)
	{
		return gs_actor_db_mgr;
	}
	gs_actor_db_mgr = new ActorDbMgr();
	massert_retval(gs_actor_db_mgr != NULL, NULL);
	return gs_actor_db_mgr;
}
int ActorDbMgr::InsertActorData(const Actor *actor)
{
	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	std::map<u64, Actor>::iterator iter = memory_db.find(actor->GetId());
	if (iter != memory_db.end())
	{
		return ERR_ALREADY_EXISTS;
	}
	Actor copy_actor = *actor;
	memory_db.insert(std::make_pair(copy_actor.GetId(), copy_actor));

	printf("ActorDbMgr:: actor db mgr insert actor:%llu success\n", copy_actor.GetId());

	ActorDB db_actor;
	db_actor.InitFromRuntimeActor(actor);
	GameSvrDbMgr *db_mgr = GameSvrDbMgr::GetInstance();
	db_mgr->InsertActor(&db_actor);

	return 0;
}
int ActorDbMgr::LoadActorData(u64 id, Actor *actor)
{
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	Actor rt_actor;
	ActorDB db_actor;
	GameSvrDbMgr *db_mgr = GameSvrDbMgr::GetInstance();
	int ret = db_mgr->QueryActor(id, &db_actor);
	if (ret != 0)
	{
		printf("actor %llu not in db\n", id);
		massert_noeffect(ret == ERR_NOT_FOUND);
		return ERR_NOT_FOUND;
	}
	db_actor.ConvertToRuntimeActor(&rt_actor);
	memory_db.insert(std::make_pair(id, rt_actor));
	

	std::map<u64, Actor>::iterator iter = memory_db.find(id);
	if (iter == memory_db.end())
	{
		actor = NULL;
		return ERR_NOT_FOUND;
	}
	*actor = iter->second;

	return 0;
}

int ActorDbMgr::UpdateActorData(const Actor *actor)
{
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	std::map<u64, Actor>::iterator iter = memory_db.find(actor->GetId());
	massert_retval(iter != memory_db.end(), ERR_INVALID_PARAM);

	iter->second = *actor;

	return 0;
}