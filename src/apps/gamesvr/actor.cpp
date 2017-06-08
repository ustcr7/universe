#include "gamesvr_def.h"
#include "actor.h"
#include <string.h>
#include "../../common/massert.h"
#include "../../common/errcode.h"
Actor::Actor()
{
	memset(_name, 0, sizeof(_name));
	_id = 0;
}
int Actor::SetName(const char *name)
{
	strncpy(_name, name, sizeof(_name));

	return 0;
}

const char* Actor::GetName() const
{
	return _name;
}

int Actor::SetId(u64 id)
{
	_id = id;
	return 0;
}

u64 Actor::GetId() const
{
	return _id;
}

const Pos* Actor::GetPos() const
{
	return &cur_pos;
}

int Actor::SetPos(const Pos *pos)
{
	massert_retval(pos != NULL, ERR_INVALID_PARAM);
	cur_pos = *pos;
	printf("actor set new pos x:%d y:%d\n", pos->GetX(), pos->GetY());
	return 0;
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

int Actor::SetInstanceId(u64 id)
{
	instance_id = id;
	printf("FOR DEBUG::::::::::::::::;actor send instance id:%llu\n", id);
	return 0;
}

u64 Actor::GetInstanceId()
{
	return instance_id;
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