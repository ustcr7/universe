#ifndef __H_ACTOR_H__
#define __H_ACTOR_H__

#include "../../common/base_type.h"
#include "gamesvr_def.h"
static const int MAX_MOVE_PATH_COUNT = 30;

struct Pos;
class MovePath
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
	const Pos* GetPos() const;
	int SetPos(Pos *pos);
	int GetSpeed()const;
	int SetSpeed(int s);
	int SetInstanceId(u64 id);
	u64 GetInstanceId();
private:
	u64 _id;
	char _name[128];
	Pos cur_pos;    //actor current pos;
	int speed;      //移动速度
	MovePath move_path;
	int cur_path_index; //当前移动到哪一段了


	u64 instance_id; //玩家当前所在instance
};


#endif // !__H_ACTOR_H__