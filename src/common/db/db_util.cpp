#include <stdlib.h>
#include <string.h>
#include "massert.h"
#include "mysql.h"
#include "db_util.h"


static UniverseDbUtil *db_util = NULL;
MYSQL gs_mysql;

UniverseDbUtil::UniverseDbUtil()
{
    //WCC_TODOÐ´½øÅäÖÃÎÄ¼þ
    strncpy(dblUser, "wcc", sizeof(dblUser));
	strncpy(dbPasswd, "123456", sizeof(dbPasswd));
	dbPort = 3306;
}

UniverseDbUtil* UniverseDbUtil::GetInstance()
{
    if( db_util == NULL )
    {
        db_util = new UniverseDbUtil();
    }
	massert_retval(db_util != NULL, NULL);

	return db_util;
}

int UniverseDbUtil::Init()
{
    int ret = 0;
	if (mysql_library_init(0, NULL, NULL)) {
		printf("could not initialize MySQL library\n");
		exit(1);
	}

	MYSQL *mysql = &gs_mysql;
	mysql = mysql_init(mysql);
	massert_retval(mysql != NULL, -1);

	mysql = mysql_real_connect(mysql, "127.0.0.1", dblUser, dbPasswd, "test_db", dbPort, NULL, 0);
	massert_retval(mysql != NULL, -1);
	
	return 0;
}

int UniverseDbUtil::Fini()
{

	mysql_close(mysql);
	mysql_library_end();
    return 0;
}

int Query(const char *sql_str, DB_QUERY_TYPE query_type, RESULT_HANDLE *ret_handle = NULL)
{
	ret = mysql_query(mysql, sql_str);
	massert_retval(0 == ret, ret);
	mysql_commit(mysql);

	MYSQL_RES *res = mysql_use_result(mysql);

	switch (query_type)
	{
	case DB_QUERY_INSERT:
	case DB_QUERY_DELETE:
	case DB_QUERY_UPDATE:
	{
		break;
	}
	case DB_QUERY_SELECT:
	{
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res)) != NULL) {
			ret_handle(&row);
		}
		mysql_free_result(res);
		break;
	}
	}

	return 0;
}