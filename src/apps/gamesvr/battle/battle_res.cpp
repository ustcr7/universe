#include "battle_res.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"

static SpellResMgr *gs_spell_res_mgr = NULL;


SpellRes::SpellRes()
{
	::SpellRes(0, UV_SPELL_EFFECT_DEC_INVALID, 0, 0, 0);
}
SpellRes::SpellRes(int param_spell_id, UV_SPELL_EFFECT_TYPE param_effect_type, int param_cd_time, int param_cost_mp, int param_distance)
{
	spell_id = param_spell_id;
	effect_type = param_effect_type;
	cd_time = param_cd_time;
	cost_mp = param_cost_mp;
	distance = param_distance;
}
int SpellRes::GetSpell_id()
{
	return spell_id;
}
UV_SPELL_EFFECT_TYPE SpellRes::GetEffectType()
{
	return effect_type;
}
int SpellRes::GetCdTime()
{
	return cd_time;
}
int SpellRes::GetCostMp()
{
	return cost_mp;
}
int SpellRes::GetDistance()
{
	return distance;
}



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
	//先手动加几个技能吧,不从资源里读取了
	SpellRes jiaxue(1001, UV_SPELL_EFFECT_INC_HP, 5, 100, 500); //加血技能
	SpellRes jiamo(1002, UV_SPELL_EFFECT_INC_MP, 3, 40, 100);   //加魔法技能
	SpellRes damage(1003, UV_SPELL_EFFECT_DEC_MP, 2, 30, 400);  //攻击技能

	spellRes.insert(std::make_pair(1001, jiaxue));
	spellRes.insert(std::make_pair(1002, jiamo));
	spellRes.insert(std::make_pair(1003, damage));
	return 0;
}

const SpellRes* SpellResMgr::GetSpellResById(int spell_id) const
{
	std::map<int, SpellRes>::const_iterator iter = spellRes.find(spell_id);
	if (iter == spellRes.end())
	{
		return NULL;
	}

	return &iter->second;
}