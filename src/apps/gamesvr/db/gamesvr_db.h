#ifndef  __GAMESVR_DB_H__
#define  __GAMESVR_DB_H__
#include "../../common/base_type.h"
//WCC_TODO: 接口均为阻塞操作,  后续改为异步操作

static const int MAX_ACTOR_NAME_SIZE = 128;
static const int MAX_ACTOR_GAME_DATA_BLOB_SIZE = 1024;
class Actor;
//
//actor_db:
//actorid, name, level, gs, blob_data

//玩家数据
class ActorDB //玩家存盘数据
{
public:
	int SetActorRid(u64 rid);
	u64 GetActorRid() const;
	int SetActorName(const char *param_name);
	const char* GetActorName() const;
	char * GetMutableActorName();
	int SetGameDataBlobSize(int blob_size);
	int GetGameDataBolbSize () const ;
	const char *GetGameDataBlob () const ;
	char *GetMutableGameDataBlob();

private:
	//基础数据单独成列,其他数据放在一个blob里
	u64 actor_rid;
	char name[MAX_ACTOR_NAME_SIZE];

	int game_data_size;
	char game_data[MAX_ACTOR_GAME_DATA_BLOB_SIZE];
};

class GameSvrDbMgr
{
public:
	GameSvrDbMgr();
	static GameSvrDbMgr* GetInstance();
	int QueryActor(u64 actor_rid, const ActorDB *actor);
	int InsertActor(ActorDB *actor);
	int UpdateActor(ActorDB *actor);
	int DeleteActor(u64 actor_rid);
};



//系统数据

#endif // ! __GAMESVR_DB_H__
