#include "gamesvr_db.h"
#include "../../../common/massert.h"
#include "../../../common/errcode.h"


static GameSvrDbMgr* gs_gamesvr_db_mgr;


u64 ActorDB::GetActorRid() const
{
	return actor_rid;
}
const char* ActorDB::GetActorName() const
{
	return name;
}
int ActorDB::GetGameDataBolbSize() const
{
	return game_data_size;
}
const char * ActorDB::GetGameDataBlob() const
{
	return game_data;
}

GameSvrDbMgr::GameSvrDbMgr()
{

}

GameSvrDbMgr* GameSvrDbMgr::GetInstance()
{
	if (gs_gamesvr_db_mgr == NULL)
	{
		gs_gamesvr_db_mgr = new GameSvrDbMgr;
		masssert_retval(gs_gamesvr_db_mgr != NULL, NULL);
	}
	return gs_gamesvr_db_mgr;
}

int GameSvrDbMgr::QueryActor(u64 actor_rid, const ActorDB *actor)
{
	//select from db;
	const char sql_str[] = "select actor_id, name, game_data_size, game_data_blob from actor_db where actor_id = 123123";

	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	return 0;
}

//WCC_TODO:ÓÃprepare statement (eg:https://dev.mysql.com/doc/refman/5.7/en/mysql-stmt-execute.html)
int GameSvrDbMgr::InsertActor(ActorDB *actor)
{
	
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[4];
	stmt = mysql_stmt_init(mysql);
	massert_retval(stmt != NULL, ERR_UNKNOWN);
	const char sql_str[] = "INSERT INTO actor_db(actor_id, name, game_data_size, game_data_blob) VALUES(?,?,?,?)";
	if (mysql_stmt_prepare(stmt, sql_str, strlen(sql_str)) != 0)
	{
		printf("mysql_stmt_prepare(), INSERT failed for:%s\n", mysql_stmt_error(stmt));
	}

	//actorid
	u64 actor_rid = actor->GetActorRid();
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_LONGLONG;
	bind[0].buffer = (char *)&actor_rid;
	bind[0].is_null = 0;
	bind[0].length = 0;

	//name 
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char *)actor->name().c_str();
	bind[1].buffer_length = 128;
	bind[1].is_null = 0;
	bind[1].length = &strlen(actor->name());

	//game data blob size
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_LONG;
	bind[0].buffer = (char *)&actor->game_data_blob_size;
	bind[0].is_null = 0;
	bind[0].length = 0;

	return 0;
}

int GameSvrDbMgr::UpdateActor(ActorDB *actor)
{

}

int GameSvrDbMgr::DeleteActor(u64 actor_rid)
{

}