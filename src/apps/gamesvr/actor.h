#ifndef __H_ACTOR_H__
#define __H_ACTOR_H__

#include "../../common/base_type.h"
#include "gamesvr_def.h"
static const int MAX_MOVE_PATH_COUNT = 30;

struct Pos;
struct MovePath
{
	int path_count;
	Pos paths[MAX_MOVE_PATH_COUNT];
};

class Actor
{
public:
	Actor();
	int SetName(const char *name);
	const char* GetName() const;
	int SetId(u64 id);
	u64 GetId() const;
	int SetConnId(u64 connId);
	u64 GetConnId() const;
	const Pos* GetPos() const;
	int SetPos(const Pos *pos);
	int GetSpeed()const;
	int SetSpeed(int s);
	int SetInstanceId(u64 id);
	u64 GetInstanceId();

	int GetCurPathIndex();
	int SetCurPathIndex(int index);

	const MovePath* GetMovePath() const
	{
		return &move_path;
	}
	int SetMovePath(const MovePath *path)
	{
		move_path = *path;
		return 0;
	}
private:
	//��������  //WCC_TODO:��id,nameҲ�Ƶ�unit��
	u64 _id;
	u64 _connId;
	char _name[128];

	//λ���������
	Pos cur_pos;    //actor current pos;
	int speed;      //�ƶ��ٶ�

	MovePath move_path;
	int cur_path_index; //��ǰ�ƶ�����һ����
	u64 move_timerid;   //�ƶ���ʱ��

	u64 instance_id; //��ҵ�ǰ����instance

	//ս��ģ������
};


#endif // !__H_ACTOR_H__