#ifndef  __UV_UNIT_H__
#define  __UV_UNIT_H__

#include "battle/battle.h"

class Unit
{
public:
	const UnitAttr* GetMutableUnitAttr();
	UnitAttr* GetUnitAttr();
	const SpellBookInfo* GetMutableSpellBookInfo();
	SpellBookInfo* GetSpellBookInfo();
	const UnitState* GetMutableUnitState();
	UnitState* GetUnitState();
private:
	UnitAttr attr;
	SpellBookInfo spellBook;
	UnitState unitState;
};

#endif // ! __UV_UNIT_H__
