#ifndef __H_ACTOR_H__
#define __H_ACTOR_H__

#include "../../common/base_type.h"
#include "gamesvr_def.h"

struct Pos;

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
	int SetInstanceId(u64 id);
	u64 GetInstanceId();
private:
	u64 _id;
	char _name[128];
	Pos cur_pos;    //actor current pos;
	u64 instance_id; //玩家当前所在instance
};


#endif // !__H_ACTOR_H__