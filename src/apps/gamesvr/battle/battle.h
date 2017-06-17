#ifndef  __UNIVERSE_BATTLE_H__
#define  __UNIVERSE_BATTLE_H__

//���״̬ :alive, dead
//������� : hp, mp, damage, defend   (Ѫ��,ħ��,�˺�ֵ,����ֵ)
//�����б� : [spellid, last_cast_time]    ��Դ:[spellid, effect_type:(�˺�/����), effect_value, ����mpֵ, ��ȴʱ��, ������Χ]
#include "../../common/base_type.h"
#include "battle_const.h"
class Actor;

//-------------------��������---------------------

class UnitAttr
{
public:
	int SetAttr(ACTOR_ATTR_TYPE type, u64 value);
	u64 GetAttr(ACTOR_ATTR_TYPE type);

	//�������Ե�����װ�ӿ�
	int DecHp(Actor *actor, int dec_value);
	int IncHp(Actor *actor, int inc_value);
	int DecMp(Actor *actor, int dec_value);
	int IncMp(Actor *actor, int inc_value);
private:
	int attrs[ACTOR_ATTR_TYPE_MAX];
};

//-------------------��������---------------------
class SpellBookInfo
{
private:
	int spell_id;
	u32 last_cast_time;
	//WCC_TODO ���ܵȼ�...
};
static const int MAX_UNIT_SPELL_COUNT = 128;
class UnitSpellBook
{
private:
	int spell_cnt;
	SpellBookInfo unit_spells[MAX_UNIT_SPELL_COUNT];
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
    int UvBattleCastSpell(Actor *caster, Actor *target, int spell_id); //Ŀǰ��֧����Ե���ļ���

};


#endif // ! __UNIVERSE_BATTLE_H__
