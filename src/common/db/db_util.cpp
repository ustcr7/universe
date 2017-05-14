#include <stdlib.h>
#include <string.h>
#include "massert.h"
#include "db_util.h"
#include "mysql.h"

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

	ret = mysql_query(mysql, "select id,name from actor_name");
	massert_retval(0 == ret ,ret);

	MYSQL_RES *res = mysql_use_result(mysql);
    MYSQL_ROW row;

	while ((row = mysql_fetch_row(res)) != NULL) {
        printf("Name:%s \n\n", row[1]);
	}

	mysql_free_result(res);

    mysql_commit(mysql);

	mysql_close(mysql);
	
	mysql_library_end();

	return 0;
}

int UniverseDbUtil::Fini()
{
    return 0;
}
