#ifndef  __UNIVERSE_BATTLE_H__
#define  __UNIVERSE_BATTLE_H__

//���״̬ :alive, dead
//������� : hp, mp, damage, defend   (Ѫ��,ħ��,�˺�ֵ,����ֵ)
//�����б� : [spellid, last_cast_time]    ��Դ:[spellid, effect_type:(�˺�/����), effect_value, ����mpֵ, ��ȴʱ��, ������Χ]
#include "../../../common/base_type.h"
#include "battle_const.h"
#include "../uv_unit.h"
class Actor;

//-------------------��������---------------------

class UnitAttr
{
public:
	int SetAttr(ACTOR_ATTR_TYPE type, u64 value);
	int DecAttr(ACTOR_ATTR_TYPE type, u64 value);
	int IncAttr(ACTOR_ATTR_TYPE type, u64 value);
	u64 GetAttr(ACTOR_ATTR_TYPE type) const;
private:
	u64 attrs[ACTOR_ATTR_TYPE_MAX]; //WCC_TODO�����ǲ���Ӧ�øĳ��޷���ֵ�ȽϺ���,�����߼������һ����ֵ�޷��ж�
};

//-------------------��������---------------------
class UnitSpellInfo
{
public:
	int GetSpellId();
	void SetSpellId(int spell_id);
	u32 GetLastCastTime();
	void SetLastCastTime(u32 tm);
	bool IsSpellInCd(u32 cur_time);
private:
	int spell_id;
	u32 last_cast_time;
	//WCC_TODO ���ܵȼ�...
};
static const int MAX_UNIT_SPELL_COUNT = 128;
class UnitSpellBook
{
public:
	UnitSpellInfo* GetUnitSpellInfoById(int spl_id);
private:
	int spell_cnt;
	UnitSpellInfo unit_spells[MAX_UNIT_SPELL_COUNT];
};

//-------------------״̬����---------------------

class UnitState
{
public:
	UnitState()
	{
		unitState = UV_UNIT_STATE_INVALID;
	}
	int SetUnitState(UV_UNIT_STATE_TYPE state)
	{
		unitState = state;
	}
	UV_UNIT_STATE_TYPE GetUnitState()
	{
		return unitState;
	}
private:
	UV_UNIT_STATE_TYPE unitState;
};



//-----------------------�����ͷŹ���-------------------
class SpellMgr
{
public:
	static SpellMgr* GetInstance();
    int UvBattleCastSpell(Unit *caster, Unit *target, int spell_id); //Ŀǰ��֧����Ե���ļ���
	int UvBattleExecuteSpellEffect(Unit *target, UV_SPELL_EFFECT_TYPE effect_type, u64 effect_value);

private:
	int UvBattleExecuteDecHp(Unit *target, u64 dec_value);
	int UvBattleExecuteIncHp(Unit *target, u64 inc_value);
	int UvBattleExecuteDecMp(Unit *target, u64 dec_value);
	int UvBattleExecuteIncMp(Unit *target, u64 inc_value);
};


#endif // ! __UNIVERSE_BATTLE_H__
