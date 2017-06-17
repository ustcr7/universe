#ifndef  __UNIVERSE_BATTLE_H__
#define  __UNIVERSE_BATTLE_H__

//玩家状态 :alive, dead
//玩家属性 : hp, mp, damage, defend   (血量,魔法,伤害值,防御值)
//技能列表 : [spellid, last_cast_time]    资源:[spellid, effect_type:(伤害/治疗), effect_value, 消耗mp值, 冷却时间, 攻击范围]
#include "../../common/base_type.h"
#include "battle_const.h"
class Actor;

//-------------------属性数据---------------------

class UnitAttr
{
public:
	int SetAttr(ACTOR_ATTR_TYPE type, u64 value);
	u64 GetAttr(ACTOR_ATTR_TYPE type);

	//常用属性单独封装接口
	int DecHp(Actor *actor, int dec_value);
	int IncHp(Actor *actor, int inc_value);
	int DecMp(Actor *actor, int dec_value);
	int IncMp(Actor *actor, int inc_value);
private:
	int attrs[ACTOR_ATTR_TYPE_MAX];
};

//-------------------技能数据---------------------
class SpellBookInfo
{
private:
	int spell_id;
	u32 last_cast_time;
	//WCC_TODO 技能等级...
};
static const int MAX_UNIT_SPELL_COUNT = 128;
class UnitSpellBook
{
private:
	int spell_cnt;
	SpellBookInfo unit_spells[MAX_UNIT_SPELL_COUNT];
};

//-------------------状态数据---------------------

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



//-----------------------技能释放管理-------------------
class SpellMgr
{
public:
    int UvBattleCastSpell(Actor *caster, Actor *target, int spell_id); //目前仅支持针对单体的技能

};


#endif // ! __UNIVERSE_BATTLE_H__
