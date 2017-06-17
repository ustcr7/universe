#include "battle_res.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"

static SpellResMgr *gs_spell_res_mgr = NULL;
SpellResMgr* SpellResMgr::GetInstance()
{
	if (gs_spell_res_mgr == NULL)
	{
		gs_spell_res_mgr = new SpellResMgr();
		massert_retval(gs_spell_res_mgr != NULL, NULL);
	}
	return gs_spell_res_mgr;
}

int SpellResMgr::InitSpellRes() 
{
}

const SpellRes* SpellResMgr::GetSpellResById(int spell_id) const
{

}