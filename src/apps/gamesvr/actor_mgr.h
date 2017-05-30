#ifndef __ACTOR_MGR_H__
#define __ACTOR_MGR_H__
#include <map>
#include "../../common/base_type.h"

class Actor;

class ActorMgr
{
public:
	static ActorMgr* GetSingleInstance();
	Actor* GetActorById(u64 id);
	Actor* AllocActor(u64 id);
	void   FreeActor(u64 id);
private:
	std::map<u64, Actor*> actorMap;
};


#endif // !__ACTOR_MGR_H__