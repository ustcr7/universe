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
	//基础数据  //WCC_TODO:把id,name也移到unit中
	u64 _connId;


	int speed;      //移动速度

	MovePath move_path;
	int cur_path_index; //当前移动到哪一段了
	u64 move_timerid;   //移动定时器

	//战斗模块数据
};


#endif // !__H_ACTOR_H__