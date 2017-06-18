#include "uv_unit.h"


const UnitAttr* Unit::GetMutableUnitAttr()
{
	return &attr;
}
UnitAttr* Unit::GetUnitAttr()
{
	return &attr;
}
const SpellBookInfo* Unit::GetMutableSpellBookInfo()
{
	return &spellBook;
}
SpellBookInfo* Unit::GetSpellBookInfo()
{
	return &spellBook;
}
const UnitState* Unit::GetMutableUnitState()
{
	return &unitState;
}
UnitState* Unit::GetUnitState()
{
	return &unitState;
}