#pragma once

//WCCTODO:抽象一个INTERFACE,这样将来换实现方式时就不需要改动调用代码
class RANKER
{
public:
	int update_rank(const RankRecord &record);
	int delete_rank(int rank_type, u64 key);
	int get_rank(int rank_type, u64 key);
	int get_rank_data(int rank_type, int start_rank, int size);

	int init_redis();
	int fini_redis();

	//WCCTODO:redis相关变量存到一个公共的地方,这样其他模块也可以很方便的使用了
private:

	//redis相关:
	unsigned int j;
	redisContext *c;
	redisReply *reply;



};