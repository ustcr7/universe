#ifndef  __UV_BATTLE_RES_H__
#define  __UV_BATTLE_RES_H__

#include "battle_const.h"
#include <map>

class SpellRes
{
	SpellRes()
	{
		::SpellRes(0, UV_SPELL_EFFECT_DEC_INVALID, 0, 0, 0);
	}
	SpellRes(int param_spell_id, UV_SPELL_EFFECT_TYPE param_effect_type, int param_cd_time, int param_cost_mp, int param_distance)
	{
		spell_id = param_spell_id;
		effect_type = param_effect_type;
		cd_time = param_cd_time;
		cost_mp = param_cost_mp;
		distance = param_distance;
	}
	int GetSpell_id()
	{
		return spell_id;
	}
	UV_SPELL_EFFECT_TYPE GetEffectType()
	{
		return effect_type;
	}
	int GetCdTime()
	{
		return cd_time;
	}
	int GetCostMp()
	{
		return cost_mp;
	}
	int GetDistance()
	{
		return distance;
	}

private:
	int spell_id;
	UV_SPELL_EFFECT_TYPE effect_type;  //技能效果类型
	int cd_time; //冷却时间
	int cost_mp; //消耗魔法值
	int distance; //攻击范围
};
class SpellResMgr
{
public:
	static SpellResMgr* GetInstance();
	int InitSpellRes(); //暂时写死几个技能,不在资源中读取了
	const SpellRes* GetSpellResById(int spell_id) const;
private:
	std::map<int, SpellRes> spellRes;
};


#endif // ! __UV_BATTLE_RES_H__
