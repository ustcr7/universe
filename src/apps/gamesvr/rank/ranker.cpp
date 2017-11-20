#include "universe_rank.pb.h"

int RANKER::update_rank(const RankRecord &record)
{

	//WCCTODO: set key-value��ô���ֱ�; ���粻ͬ�����а������ڲ�ͬ�ı���
	//������а��Ǻ�����  ZADD ����ָ�� rank_type
	//������ͨ��key-value��, ÿ��key-value�����Ӧһ��redis-server?�����ǵÿ���ʮ�ϰٸ�redis-server??
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