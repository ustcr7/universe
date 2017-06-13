#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "actor_mgr.h"
#include "actor.h"


static ActorMgr* gs_actor_mgr = NULL;

ActorMgr* ActorMgr::GetSingleInstance()
{
	if (gs_actor_mgr != NULL)
	{
		return gs_actor_mgr;
	}
	gs_actor_mgr = new ActorMgr();
	massert_retval(gs_actor_mgr != NULL, NULL);

	return gs_actor_mgr;
}

Actor* ActorMgr::GetActorById(u64 id)
{
	std::map<u64, Actor*>::iterator iter = actorMap.find(id);
	if (iter == actorMap.end())
	{
		return NULL;
	}

	return iter->second;
}

Actor* ActorMgr::AllocActor(u64 id)
{
	massert_retval(GetActorById(id) == NULL, NULL);
	Actor *actor = new Actor();
	massert_retval(actor != NULL, NULL);
	actor->SetId(id);

	actorMap.insert(std::make_pair(id, actor));

	return actor;
}
void   ActorMgr::FreeActor(u64 id)
{
	Actor *actor = ActorMgr::GetActorById(id);
	massert_retnone(actor != NULL);

	delete actor;
	actorMap.erase(id);
}

int ActorMgr::DumpAllActors(Actor **actorArr, int *actorCnt)
{
	int ret = 0;
	massert_retval(actorArr != nullptr && actorCnt != nullptr, ERR_INVALID_PARAM);

	int maxCnt = *actorCnt;
	*actorCnt = 0;
	for (std::map<u64, Actor*>::iterator iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		if (*actorCnt >= maxCnt)
		{
			break;
		}
		actorArr[*actorCnt] = iter->second;
		*actorCnt = *actorCnt + 1;
	}

	return 0;
}