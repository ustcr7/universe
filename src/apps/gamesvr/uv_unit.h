#ifndef  __UV_UNIT_H__
#define  __UV_UNIT_H__

#include "battle/battle.h"
#include "gamesvr_def.h"
class  Pos;

class Unit
{
public:
	Unit();

	int SetName(const char *name);
	const char* GetName() const;
	int SetId(u64 id);
	u64 GetId() const;

	UnitAttr* GetMutableUnitAttr();
	const UnitAttr* GetUnitAttr()const;
	UnitSpellBook* GetMutableSpellBook();
	const UnitSpellBook* GetSpellBook()const;
	UnitState* GetMutableUnitState();
	const UnitState* GetUnitState()const;

	int SetInstanceId(u64 id);
	u64 GetInstanceId() const;

	const Pos* GetPos() const;
	int SetPos(const Pos *pos);
private:
	u64 _id;
	char _name[128];

	UnitAttr attr;
	UnitSpellBook spellBook;
	UnitState unitState;

	u64 instance_id; //当前所在instance

	//位置相关数据
	Pos cur_pos;    //actor current pos;
};

#endif // ! __UV_UNIT_H__
