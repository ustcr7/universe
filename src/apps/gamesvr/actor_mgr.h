#ifndef __ACTOR_MGR_H__
#define __ACTOR_MGR_H__
#include <map>
#include "../../common/base_type.h"

class Actor;

int SaveActorTimeout(const char *callback_data, u32 callback_data_len);

class ActorMgr
{
public:
	ActorMgr();
	static ActorMgr* GetSingleInstance();
	Actor* GetActorById(u64 id);
	Actor* AllocActor(u64 id);
	void   FreeActor(u64 id);

	//也可以提供一个foreach函数,但是foreach不能删除actor,所以这里提供一个全量dump函数
	int    DumpAllActors(Actor **actorArr, int *actorCnt); 

	int    SaveAllActors();
	
private:
	std::map<u64, Actor*> actorMap;

private:

};


#endif // !__ACTOR_MGR_H__