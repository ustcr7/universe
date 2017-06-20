#include "gamesvr_def.h"
#include "actor.h"
#include <string.h>
#include "../../common/massert.h"
#include "../../common/errcode.h"
Actor::Actor()
{
	_connId = 0;
}

int Actor::SetConnId(u64 connId)
{
	_connId = connId;
	return 0;
}
u64 Actor::GetConnId() const
{
	return _connId;
}


int Actor::GetSpeed()const
{
	return speed;
}

int Actor::SetSpeed(int s)
{
	speed = s;
	printf("actor set speed %d\n", speed);
	return 0;
}

int Actor::GetCurPathIndex()
{
	return cur_path_index;
}
int Actor::SetCurPathIndex(int index)
{
	cur_path_index = index;
	//printf("actor set new path index:%d\n", index);
	return 0;
}