#include "universe_rank.pb.h"

int RANKER::update_rank(const RankRecord &record)
{

	//WCCTODO: set key-value怎么区分表; 例如不同的排行榜数据在不同的表里
	//针对排行榜到是很容易  ZADD 可以指定 rank_type
	//但是普通的key-value呢, 每种key-value必须对应一个redis-server?那我们得开几十上百个redis-server??
	reply = (redisReply *)redisCommand(c, "ZADD %s %s %s"
		, rank_name, score, member");
	printf("SET: %s\n", reply->str);
	freeReplyObject(reply);

}

int RANKER::delete_rank(int rank_type, u64 key)
{

}
int RANKER::get_rank(int rank_type, u64 key)
{

}
int RANKER::get_rank_data(int rank_type, int start_rank, int size)
{

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
}

int RANKER::fini_redis()
{
	redisFree(c);
	return 0;
}