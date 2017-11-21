#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "ranker.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>

RANKER *gs_ranker = NULL;

RANKER* RANKER::get_instance()
{
	if (gs_ranker == NULL)
	{
		gs_ranker = new RANKER();
		gs_ranker->init_redis();
	}
	massert_retval(gs_ranker != NULL, NULL);

	return gs_ranker;
}

int RANKER::update_rank(const RankRecord &record)
{

	//WCCTODO: set key-value怎么区分表; 例如不同的排行榜数据在不同的表里
	//针对排行榜到是很容易  ZADD 可以指定 rank_type
	//但是普通的key-value呢, 每种key-value必须对应一个redis-server?那我们得开几十上百个redis-server??
	reply = (redisReply *)redisCommand(c, "ZADD %s %d %d"
		, "my_rank", 100, 19883928);
	//printf("SET: %s\n", reply->str);
	freeReplyObject(reply);

	return 0;
}

int RANKER::delete_rank(int rank_type, u64 key)
{
	return 0;
}
int RANKER::get_rank(int rank_type, u64 key)
{
	return 0;
}
int RANKER::get_rank_data(int rank_type, int start_rank, int size)
{
	return 0;
}


int RANKER::init_redis()
{
	const char *hostname = "127.0.0.1";
	int port = 6379;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	c = redisConnectWithTimeout(hostname, port, timeout);
	if (c == NULL || c->err) 
	{
		if (c) {
			printf("Connection error: %s\n", c->errstr);
			redisFree(c);
		}
		else {
			printf("Connection error: can't allocate redis context\n");
		}
		exit(1);
	}
	std::cout << "init rank success" << std::endl;

	//FOR DEBUG
	// 测试zadd
	const char* set_name = "actor_rank";
	static const int MAX_ACTOR_RID = 2000000;
	int test_case_num = 10;
	for (int i = 0; i<test_case_num; ++i)
	{
		std::cout << "......." << std::endl;
		int actor_score = 2000;
		int actor_rid = MAX_ACTOR_RID;
		reply = (redisReply *)redisCommand(c, "ZADD %s %d %d", set_name, actor_score, actor_rid);
		freeReplyObject(reply);
	}

	//测试zrank
	test_case_num = 10;
	for (int i = 0; i<test_case_num; ++i)
	{
		std::cout << "+++" << std::endl;
		int actor_rid = MAX_ACTOR_RID;
		reply = (redisReply *)redisCommand(c, "ZRANK %s %d", set_name, actor_rid);
		freeReplyObject(reply);
	}

	return 0;
}

int RANKER::fini_redis()
{
	redisFree(c);
	return 0;
}