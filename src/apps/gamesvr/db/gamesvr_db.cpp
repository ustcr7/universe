#include "gamesvr_db.h"
#include "../../../common/massert.h"
#include "../../../common/errcode.h"
#include "../../../common/db/db_util.h"
#include <string.h>
#include "../actor.h"


static GameSvrDbMgr* gs_gamesvr_db_mgr;

int ActorDB::SetActorRid(u64 rid)
{
	actor_rid = rid;
	return 0;
}

u64 ActorDB::GetActorRid() const
{
	return actor_rid;
}

int ActorDB::SetActorName(const char *param_name)
{
	massert_retval(param_name != NULL, ERR_INVALID_PARAM);

	strncpy(name, param_name, sizeof(name));
	return 0;
}

const char* ActorDB::GetActorName() const
{
	return name;
}

char * ActorDB::GetMutableActorName()
{
	return name;
}

int ActorDB::SetGameDataBlob(const char *blob_data, int blob_size)
{
	massert_retval(blob_size < MAX_ACTOR_GAME_DATA_BLOB_SIZE, ERR_INVALID_PARAM);
	game_data_size = blob_size;
	memcpy(game_data, blob_data, blob_size);
	return 0;
}
int ActorDB::GetGameDataBolbSize() const
{
	return game_data_size;
}
const char * ActorDB::GetGameDataBlob() const
{
	return game_data;
}

char *  ActorDB::GetMutableGameDataBlob()
{
	return game_data;
}

int  ActorDB::InitFromRuntimeActor(const Actor *rt_actor)
{
	SetActorRid(rt_actor->GetId());
	SetActorName(rt_actor->GetName());
	
	//save blob
	//SetGameDataBlob()

	return 0;
}

GameSvrDbMgr::GameSvrDbMgr()
{

}

GameSvrDbMgr* GameSvrDbMgr::GetInstance()
{
	if (gs_gamesvr_db_mgr == NULL)
	{
		gs_gamesvr_db_mgr = new GameSvrDbMgr;
		massert_retval(gs_gamesvr_db_mgr != NULL, NULL);
	}
	return gs_gamesvr_db_mgr;
}

int GameSvrDbMgr::QueryActor(u64 actor_rid, const ActorDB *actor)
{
	//select from db;
	//const char sql_str[] = "select actor_id, name, game_data_size, game_data_blob from actor_db where actor_id = 123123";
	//massert_retval(actor != NULL, ERR_INVALID_PARAM);
	return 0;
}

int GameSvrDbMgr::InsertActor(ActorDB *actor)
{
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[4];

	UniverseDbUtil *db_util = UniverseDbUtil::GetInstance();
	stmt = mysql_stmt_init(db_util->GetMysqlInstance());
	massert_retval(stmt != NULL, ERR_UNKNOWN);
	const char sql_str[] = "INSERT INTO actor_db(actor_id, name, game_data_size, game_data_blob) VALUES(?,?,?,?)";
	if (mysql_stmt_prepare(stmt, sql_str, strlen(sql_str)) != 0)
	{
		printf("mysql_stmt_prepare(), INSERT failed for:%s\n", mysql_stmt_error(stmt));
	}

	/*MYSQL_RES *prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (prepare_meta_result == NULL)
	{
		printf(" mysql_stmt_result_metadata fail:%s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	int column_count = mysql_num_fields(prepare_meta_result);
	massert_retval(column_count == 4, ERR_INVALID_PARAM); */

	int param_count = mysql_stmt_param_count(stmt);
	massert_retval(param_count == 4, -1);
	//actorid
	u64 actor_rid = actor->GetActorRid();
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_LONGLONG;
	bind[0].buffer = (char *)&actor_rid;
	bind[0].is_null = 0;
	bind[0].length = 0;

	//name 
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = actor->GetMutableActorName();
	bind[1].buffer_length = 128;
	bind[1].is_null = 0;
	unsigned long name_len = strlen(actor->GetActorName());
	bind[1].length = &name_len;

	//game data blob size
	int blob_size = actor->GetGameDataBolbSize();
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = (char *)&blob_size;
	bind[2].is_null = 0;
	bind[2].length = 0;

	//game data blob size
	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer = actor->GetMutableGameDataBlob();  //FUCK 为什么不是const
	bind[3].is_null = 0;
	bind[3].length = (unsigned long*)&blob_size;

	if (mysql_stmt_bind_param(stmt, bind))
	{
		printf(" bind failed: %s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	/*if (mysql_stmt_bind_result(stmt, bind) != 0)
	{
		printf(" mysql_stmt_bind_result() failed :%s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}*/

	if (mysql_stmt_execute(stmt) != 0)
	{
		printf("execute failed: %s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	//mysql_free_result(prepare_meta_result);

	if (mysql_stmt_close(stmt))
	{
		printf(" failed while closing the statement for %s\n", mysql_error(db_util->GetMysqlInstance()));
		massert_retval(0, -1);
	}

	return 0;
}

int GameSvrDbMgr::UpdateActor(ActorDB *actor)
{
	return 0;
}

int GameSvrDbMgr::DeleteActor(u64 actor_rid)
{
	return 0;
}