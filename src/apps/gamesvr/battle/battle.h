#ifndef  __UNIVERSE_BATTLE_H__
#define  __UNIVERSE_BATTLE_H__

//玩家状态 :alive, dead
//玩家属性 : hp, mp, damage, defend   (血量,魔法,伤害值,防御值)
//技能列表 : [spellid, last_cast_time]    资源:[spellid, effect_type:(伤害/治疗), effect_value, 消耗mp值, 冷却时间, 攻击范围]
#include "../../../common/base_type.h"
#include "battle_const.h"
class Actor;
class Unit;
//-------------------属性数据---------------------
class UnitAttr
{
public:
	UnitAttr();
	int SetAttr(ACTOR_ATTR_TYPE type, u64 value);
	int DecAttr(ACTOR_ATTR_TYPE type, u64 value);
	int IncAttr(ACTOR_ATTR_TYPE type, u64 value);
	u64 GetAttr(ACTOR_ATTR_TYPE type) const;
private:
	u64 attrs[ACTOR_ATTR_TYPE_MAX]; //WCC_TODO属性是不是应该改成无符号值比较合适,否则逻辑计算出一个负值无法判断
};

//-------------------技能数据---------------------
class UnitSpellInfo
{
public:
	UnitSpellInfo()
	{
		spell_id = 0;
		last_cast_time = 0;
	}
	int GetSpellId() const;
	void SetSpellId(int spell_id);
	u32 GetLastCastTime() const;
	void SetLastCastTime(u32 tm);
	bool IsSpellInCd(u32 cur_time) const;
private:
	int spell_id;
	u32 last_cast_time;
	//WCC_TODO 技能等级...
};
static const int MAX_UNIT_SPELL_COUNT = 128;
class UnitSpellBook
{
public:
	UnitSpellBook()
	{
		spell_cnt = 0;
	}
	UnitSpellInfo* GetUnitSpellInfoById(int spl_id);
	int AddUnitSpellInfo(const UnitSpellInfo* spellInfo);
private:
	int spell_cnt;
	UnitSpellInfo unit_spells[MAX_UNIT_SPELL_COUNT];

	bool IsBookFull();
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
		return 0;
	}
	UV_UNIT_STATE_TYPE GetUnitState() const 
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
	static SpellMgr* GetInstance();
    int UvBattleCastSpell(Unit *caster, Unit *target, int spell_id); //目前仅支持针对单体的技能
	int UvBattleExecuteSpellEffect(Unit *target, UV_SPELL_EFFECT_TYPE effect_type, u64 effect_value);

private:
	int UvBattleExecuteDecHp(Unit *target, u64 dec_value);
	int UvBattleExecuteIncHp(Unit *target, u64 inc_value);
	int UvBattleExecuteDecMp(Unit *target, u64 dec_value);
	int UvBattleExecuteIncMp(Unit *target, u64 inc_value);
};


#endif // ! __UNIVERSE_BATTLE_H__
