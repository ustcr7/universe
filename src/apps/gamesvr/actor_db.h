/*
#ifndef __H_ACTOR_DB_H__
#define __H_ACTOR_DB_H__


//WCC_TODO: just in memory now,  to be saved in real db

class Actor;

class ActorDbMgr
{
public:
	ActorDbMgr();
	static ActorDbMgr* GetSingleInstance();
	int InsertActorData(const Actor *actor);
	int LoadActorData(u64 id, Actor *actor);
	int UpdateActorData(const Actor *actor);
};

#endif
*/