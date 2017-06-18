#ifndef  __UV_BATTLE_RES_H__
#define  __UV_BATTLE_RES_H__

#include "battle_const.h"
#include <map>

class SpellRes
{
public:
	SpellRes();
	SpellRes(int param_spell_id, UV_SPELL_EFFECT_TYPE param_effect_type, int param_cd_time, int param_cost_mp, int param_distance);
	int GetSpell_id();
	UV_SPELL_EFFECT_TYPE GetEffectType();
	int GetCdTime();
	int GetCostMp();
	int GetDistance();
private:
	int spell_id;
	UV_SPELL_EFFECT_TYPE effect_type;  //����Ч������
	int cd_time; //��ȴʱ��
	int cost_mp; //����ħ��ֵ
	int distance; //������Χ
};
class SpellResMgr
{
public:
	static SpellResMgr* GetInstance();
	int InitSpellRes();
	const SpellRes* GetSpellResById(int spell_id) const;
private:
	std::map<int, SpellRes> spellRes;
};


#endif // ! __UV_BATTLE_RES_H__
