#ifndef __UNIVERSE_DB_UTIL_H_
#define __UNIVERSE_DB_UTIL_H_

enum DB_QUERY_TYPE
{
	DB_QUERY_INVALID = 0,
	DB_QUERY_INSERT,
	DB_QUERY_DELETE,
	DB_QUERY_UPDATE,
	DB_QUERY_SELECT,
};

class MYSQL_RES;
typedef int (*RESULT_HANDLE)(MYSQL_RES *res);

class UniverseDbUtil
{
public:
	UniverseDbUtil();
	static UniverseDbUtil* GetInstance();
    int Init();
    int Fini();

	int Query(const char *sql_str, DB_QUERY_TYPE query_type, RESULT_HANDLE *ret_handle=NULL);

	//insert
	//delete
	//update
	//select

private:
	char dblUser[128];
	char dbPasswd[128];
	int dbPort;
};

#endif

