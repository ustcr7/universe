#include "battle.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"

SpellMgr* gs_spell_mgr = NULL;

SpellMgr* SpellMgr::GetInstance()
{
	if (gs_spell_mgr == NULL)
	{
		gs_spell_mgr = new SpellMgr;
		massert_retval(gs_spell_mgr != NULL, NULL);
	}
	return gs_spell_mgr;
}

int SpellMgr::UvBattleCastSpell(Unit *caster, Unit *target, int spell_id)
{
	massert_retval(caster != target, ERR_INVALID_PARAM);

	//check cd
	//check distance

	//cost mp

	//damage

	return 0;
}