#include "uv_unit.h"
#include <stdio.h>
#include <string.h>
#include "../../common/massert.h"
#include "../../common/errcode.h"

Unit::Unit()
{
	memset(_name, 0, sizeof(_name));
	_id = 0;
	instance_id = 0;
}

int Unit::SetName(const char *name)
{
	strncpy(_name, name, sizeof(_name));

	return 0;
}

const char* Unit::GetName() const
{
	return _name;
}

int Unit::SetId(u64 id)
{
	_id = id;
	return 0;
}

u64 Unit::GetId() const
{
	return _id;
}

UnitAttr* Unit::GetMutableUnitAttr()
{
	return &attr;
}
const UnitAttr* Unit::GetUnitAttr() const
{
	return &attr;
}
UnitSpellBook* Unit::GetMutableSpellBook()
{
	return &spellBook;
}
const UnitSpellBook* Unit::GetSpellBook() const
{
	return &spellBook;
}
UnitState* Unit::GetMutableUnitState()
{
	return &unitState;
}
const UnitState* Unit::GetUnitState() const
{
	return &unitState;
}

int Unit::SetInstanceId(u64 id)
{
	instance_id = id;
	printf("actor send instance id:%llu\n", id);
	return 0;
}

u64 Unit::GetInstanceId() const
{
	return instance_id;
}


const Pos* Unit::GetPos() const
{
	return &cur_pos;
}

int Unit::SetPos(const Pos *pos)
{
	massert_retval(pos != NULL, ERR_INVALID_PARAM);
	cur_pos = *pos;
	printf("unit set new pos x:%d y:%d\n", pos->GetX(), pos->GetY());
	return 0;
}