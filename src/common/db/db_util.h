#ifndef __UNIVERSE_DB_UTIL_H_
#define __UNIVERSE_DB_UTIL_H_



class UniverseDbUtil
{
public:
	UniverseDbUtil();
	static UniverseDbUtil* GetInstance();
    int Init();
    int Fini();	

private:
	char dblUser[128];
	char dbPasswd[128];
	int dbPort;
};

#endif

