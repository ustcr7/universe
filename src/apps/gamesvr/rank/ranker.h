#pragma once

//WCCTODO:����һ��INTERFACE,����������ʵ�ַ�ʽʱ�Ͳ���Ҫ�Ķ����ô���
class RANKER
{
public:
	int update_rank(const RankRecord &record);
	int delete_rank(int rank_type, u64 key);
	int get_rank(int rank_type, u64 key);
	int get_rank_data(int rank_type, int start_rank, int size);

	int init_redis();
	int fini_redis();

	//WCCTODO:redis��ر����浽һ�������ĵط�,��������ģ��Ҳ���Ժܷ����ʹ����
private:

	//redis���:
	unsigned int j;
	redisContext *c;
	redisReply *reply;



};