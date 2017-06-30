#include "gamesvr_db.h"
#include "../../../common/massert.h"
#include "../../../common/errcode.h"
#include "../../../common/db/db_util.h"
#include "universe_db.pb.h"
#include <string.h>
#include "../actor.h"
#include <sstream>


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

int ActorDB::SetGameDataBlobSize(int blob_size)
{
	massert_retval(blob_size < MAX_ACTOR_GAME_DATA_BLOB_SIZE, ERR_INVALID_PARAM);
	game_data_size = blob_size;
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

int GameSvrDbMgr::QueryActor(u64 actor_rid, ActorDB *actor)
{
	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	int BIND_PARAM_COUNT = 4;
	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[BIND_PARAM_COUNT];

	UniverseDbUtil *db_util = UniverseDbUtil::GetInstance();
	stmt = mysql_stmt_init(db_util->GetMysqlInstance());
	massert_retval(stmt != NULL, ERR_UNKNOWN);

	std::stringstream sstr;
	sstr << "select actor_id,name,game_data_size,game_data_blob from actor_db where actor_id = " << actor_rid << ";";
	printf("query sql:%s\n", sstr.str().c_str());
	if (mysql_stmt_prepare(stmt, sstr.str().c_str(), strlen(sstr.str().c_str())) != 0)
	{
		printf("mysql_stmt_prepare(), INSERT failed for:%s\n", mysql_stmt_error(stmt));
	}

	int param_count = mysql_stmt_param_count(stmt);
	massert_retval(param_count == 0, -1);

	MYSQL_RES     *prepare_meta_result = mysql_stmt_result_metadata(stmt);
	if (prepare_meta_result == NULL)
	{
		printf("prepare_meta_result failed:%s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	int column_count = mysql_num_fields(prepare_meta_result);
	printf("column count:%d\n", column_count);
	massert_retval(column_count == BIND_PARAM_COUNT, ERR_INVALID_PARAM);

	if (mysql_stmt_execute(stmt))
	{
		printf("mysql_stmt_execute failed: %s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	my_bool       is_null[4] = { 0 };
	my_bool       error[4] = { 0 };
	unsigned long length[4] = { 0 };

	u64 resutl_actor_rid;
	char resutl_actor_name[128];

	//actorid
	memset(bind, 0, sizeof(bind));
	bind[0].buffer_type = MYSQL_TYPE_LONGLONG;
	bind[0].buffer = (char *)&resutl_actor_rid;
	bind[0].is_null = &is_null[0];
	bind[0].length = &length[0];
	bind[0].error = &error[0];

	//name 
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = (char*)resutl_actor_name;
	bind[1].buffer_length = 128;
	bind[1].is_null = &is_null[1];
	bind[1].length = &length[1];
	bind[1].error = &error[1];

	//game data blob size
	unsigned long blob_size = (unsigned long)actor->GetGameDataBolbSize();
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = (char *)&blob_size;
	bind[2].is_null = 0;
	bind[2].length = 0;

	//game data blob size
	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer = actor->GetMutableGameDataBlob();  //FUCK 为什么不是const
	bind[3].is_null = 0;
	bind[3].length = (unsigned long*)&blob_size;

	if (mysql_stmt_bind_result(stmt, bind))
	{
		printf(" mysql_stmt_bind_result failed:%s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	if (mysql_stmt_store_result(stmt))
	{
		printf("mysql_stmt_store_result failed: %s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	//fetch
	int row_count = 0;
	while (!mysql_stmt_fetch(stmt))
	{
		row_count++;
		fprintf(stdout, "  row %d\n", row_count);
		massert_continue(!is_null[0]);
		massert_continue(!is_null[1]);
		massert_continue(!is_null[2]);
		massert_continue(!is_null[3]);
		printf("select result: actor_rid:%llu name:%s\n", resutl_actor_rid, resutl_actor_name);

		actor->SetActorRid(resutl_actor_rid);
		actor->SetActorName(resutl_actor_name);
		actor->SetGameDataBlobSize(blob_size);
		//blob是直接写入缓存的

		break; //WCC_TODO 只能query到一个否则就有问题了
	}

	//finish
	mysql_free_result(prepare_meta_result);
	if (mysql_stmt_close(stmt))
	{
		printf("mysql_stmt_close failed: %s\n", mysql_error(db_util->GetMysqlInstance()));
		massert_retval(0, -1);
	}

	if (row_count <= 0)
	{
		return ERR_NOT_FOUND;
	}

	printf("query from db finish\n");

	return 0;
}

int GameSvrDbMgr::InsertActor(ActorDB *actor)
{	
	massert_retval(actor != NULL, ERR_INVALID_PARAM);

	printf("insert db actor %llu\n", actor->GetActorRid());

	int BIND_PARAM_COUNT = 4;

	MYSQL_STMT    *stmt;
	MYSQL_BIND    bind[BIND_PARAM_COUNT];

	UniverseDbUtil *db_util = UniverseDbUtil::GetInstance();
	massert_retval(db_util != NULL, ERR_INVALID_PARAM);
	stmt = mysql_stmt_init(db_util->GetMysqlInstance());
	massert_retval(stmt != NULL, ERR_UNKNOWN);
	const char sql_str[] = "INSERT INTO actor_db(actor_id, name, game_data_size, game_data_blob) VALUES(?,?,?,?)";
	if (mysql_stmt_prepare(stmt, sql_str, strlen(sql_str)) != 0)
	{
		printf("mysql_stmt_prepare(), INSERT failed for:%s\n", mysql_stmt_error(stmt));
	}

	int param_count = mysql_stmt_param_count(stmt);
	massert_retval(param_count == BIND_PARAM_COUNT, -1);
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
	unsigned long blob_size = (unsigned long)actor->GetGameDataBolbSize();
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer = (char *)&blob_size;
	bind[2].is_null = 0;
	bind[2].length = 0;
	printf("game data blob size:%d \n", (int)blob_size);

	
	//game data blob size
	bind[3].buffer_type = MYSQL_TYPE_BLOB;
	bind[3].buffer = actor->GetMutableGameDataBlob();  //FUCK 为什么不是const
	bind[3].is_null = 0;
	bind[3].length = (unsigned long*)&blob_size;
	
	if (mysql_stmt_bind_param(stmt, bind))
	{
		printf(" bind failed: %s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

	printf("begin exexute sql\n");
	if (mysql_stmt_execute(stmt) != 0)  //WCC_TODO CORE DUMP
	{
		printf("execute failed: %s\n", mysql_stmt_error(stmt));
		massert_retval(0, -1);
	}

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