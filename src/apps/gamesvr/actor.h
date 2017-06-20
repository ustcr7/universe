#ifndef __H_ACTOR_H__
#define __H_ACTOR_H__

#include "../../common/base_type.h"
#include "gamesvr_def.h"
#include "uv_unit.h"
static const int MAX_MOVE_PATH_COUNT = 30;

struct Pos;
struct MovePath
{
	int path_count;
	Pos paths[MAX_MOVE_PATH_COUNT];
};

class Actor : public Unit
{
public:
	Actor();
	int SetConnId(u64 connId);
	u64 GetConnId() const;
	int GetSpeed()const;
	int SetSpeed(int s);

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
	u64 _connId;


	int speed;      //�ƶ��ٶ�

	MovePath move_path;
	int cur_path_index; //��ǰ�ƶ�����һ����
	u64 move_timerid;   //�ƶ���ʱ��

	//ս��ģ������
};


#endif // !__H_ACTOR_H__