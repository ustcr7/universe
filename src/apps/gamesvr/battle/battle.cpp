#include "battle.h"
#include "../../../common/massert.h"
#include "../../../common/errcode.h"
#include "../../../common/base_type.h"
#include "../../../common/timer/uv_timer.h"
#include "../instance_mgr.h"
#include "../gamesvr_def.h"
#include "battle_res.h"
#include <algorithm>
#include "../uv_unit.h"
#include <string.h>
SpellMgr* gs_spell_mgr = NULL;

UnitAttr::UnitAttr()
{
	memset(attrs, 0, sizeof(attrs));
}

int UnitAttr::SetAttr(ACTOR_ATTR_TYPE type, u64 value)
{
	massert_retval(type >= 0 && type < ACTOR_ATTR_TYPE_MAX, ERR_INVALID_PARAM);
	attrs[type] = value;
	return 0;
}
u64 UnitAttr::GetAttr(ACTOR_ATTR_TYPE type) const
{
	massert_retval(type >= 0 && type < ACTOR_ATTR_TYPE_MAX, ERR_INVALID_PARAM);
	return attrs[type];
}

int UnitAttr::DecAttr(ACTOR_ATTR_TYPE type, u64 value)
{
	massert_retval(type >= 0 && type < ACTOR_ATTR_TYPE_MAX, ERR_INVALID_PARAM);
	u64 cur_value = attrs[type];
	u64 new_value = cur_value - std::min(cur_value, value);
	attrs[type] = new_value;
	return 0;
}
int UnitAttr::IncAttr(ACTOR_ATTR_TYPE type, u64 value)
{
	massert_retval(type >= 0 && type < ACTOR_ATTR_TYPE_MAX, ERR_INVALID_PARAM);
	attrs[type] = attrs[type] + value;
	return 0;
}

int UnitSpellInfo::GetSpellId() const
{
	return spell_id;
}
void UnitSpellInfo::SetSpellId(int param_spell_id)
{
	spell_id = param_spell_id;
}
u32 UnitSpellInfo::GetLastCastTime() const
{
	return last_cast_time;
}
void UnitSpellInfo::SetLastCastTime(u32 tm)
{
	last_cast_time = tm;
}

bool UnitSpellInfo::IsSpellInCd(u32 cur_time) const
{
	const SpellRes *spl_res = SpellResMgr::GetInstance()->GetSpellResById(spell_id);
	massert_retval(spl_res != NULL, true);

	int cd_time = spl_res->GetCdTime();
	if (last_cast_time + cd_time < cur_time)
	{
		return true;
	}
	return false;
}


UnitSpellInfo* UnitSpellBook::GetUnitSpellInfoById(int spl_id)
{
	for (int i = 0; i < spell_cnt; ++i)
	{
		if (unit_spells[i].GetSpellId() == spl_id)
		{
			return &unit_spells[i];
		}
	}
	return NULL;
}

int UnitSpellBook::AddUnitSpellInfo(const UnitSpellInfo* spellInfo)
{
	massert_retval(spellInfo != NULL, ERR_INVALID_PARAM);
	if (GetUnitSpellInfoById(spellInfo->GetSpellId()) != NULL)
	{
		return ERR_ALREADY_EXISTS;
	}

	if (IsBookFull())
	{
		return ERR_NOT_ENOUGH;
	}

	unit_spells[spell_cnt] = *spellInfo;
	spell_cnt++;

	return 0;
}

bool UnitSpellBook::IsBookFull()
{
	return spell_cnt >= (int)(sizeof(unit_spells)/sizeof(unit_spells[0]));
}

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
	int ret = 0;
	massert_retval(caster != NULL && target != NULL, ERR_INVALID_PARAM);
	printf("caster %s cast spell %d to target %s\n", caster->GetName(), spell_id, target->GetName());

	SpellResMgr *spl_res_mgr = SpellResMgr::GetInstance();
	const SpellRes *spl_res = spl_res_mgr->GetSpellResById(spell_id);
	massert_retval(spl_res != NULL, ERR_NOT_FOUND);

	//find spell info from actor spell_book
	UnitSpellBook *spl_book = caster->GetMutableSpellBook();
	massert_retval(spl_book != NULL, ERR_INVALID_PARAM);

	UnitSpellInfo* unit_spl_info = spl_book->GetUnitSpellInfoById(spell_id);
	if (unit_spl_info == NULL)
	{
		return ERR_SPELL_NOT_LEARNED;
	}

	//check cd
	u32 cur_time = UvTimer::GetSingleInstance()->GetCurTime();
	if (unit_spl_info->IsSpellInCd(cur_time))
	{
		return ERR_SPELL_IN_CD;
	}

	//check distance
	if (caster->GetInstanceId() != target->GetInstanceId())
	{
		return ERR_INST_NOT_IN_SAME_INSTANCEE;
	}
	Instance *cast_inst = InstanceMgr::GetSingleInstance()->GetInstanceById(caster->GetInstanceId());
	massert_retval(cast_inst != NULL, ERR_INST_NOT_IN_INSTANCE);

	int dist = caster->GetPos()->GetDistanceFromPos(target->GetPos());
	if (dist > spl_res->GetDistance())
	{
		return ERR_SPELL_TOO_FAR_AWAY;
	}

	//cost mp
	u64 caster_cur_mp = caster->GetUnitAttr()->GetAttr(ACTOR_ATTR_MP);
	int cost_mp = spl_res->GetCostMp();
	if (caster_cur_mp < (u64)cost_mp)
	{
		return ERR_SPELL_MP_NOT_ENOUGH;
	}
	caster->GetMutableUnitAttr()->DecAttr(ACTOR_ATTR_MP, cost_mp);

	//damage
	UV_SPELL_EFFECT_TYPE effect_type = spl_res->GetEffectType();
	u64 effect_value = spl_res->GetEffectValue();

	ret = UvBattleExecuteSpellEffect(target, effect_type, effect_value);
	massert_noeffect(ret == 0);

	//sync
	//log

	return 0;
}

int SpellMgr::UvBattleExecuteSpellEffect(Unit *target, UV_SPELL_EFFECT_TYPE effect_type, u64 effect_value)
{
	massert_retval(target != NULL, ERR_INVALID_PARAM);

	switch (effect_type)
	{
	case UV_SPELL_EFFECT_DEC_HP:
		UvBattleExecuteDecHp(target, effect_value);
		break;
	case UV_SPELL_EFFECT_INC_HP:
		massert_noeffect(0); 
		break;
	case UV_SPELL_EFFECT_DEC_MP:
		massert_noeffect(0);
		break;
	case UV_SPELL_EFFECT_INC_MP:
		massert_noeffect(0);
		break;
	default:
		massert_noeffect(0);
		break;
	}
	return 0;
}

int SpellMgr::UvBattleExecuteDecHp(Unit *target, u64 dec_value)
{
	massert_retval(target != NULL, ERR_INVALID_PARAM);
	return target->GetMutableUnitAttr()->DecAttr(ACTOR_ATTR_HP, dec_value);
}

int SpellMgr::UvBattleExecuteIncHp(Unit *target, u64 inc_value)
{
	massert_retval(target != NULL, ERR_INVALID_PARAM);

	target->GetMutableUnitAttr()->IncAttr(ACTOR_ATTR_HP, inc_value);
	return 0;
}
int SpellMgr::UvBattleExecuteDecMp(Unit *target, u64 dec_value)
{
	massert_retval(target != NULL, ERR_INVALID_PARAM);

	target->GetMutableUnitAttr()->DecAttr(ACTOR_ATTR_HP, dec_value);
	return 0;
}
int SpellMgr::UvBattleExecuteIncMp(Unit *target, u64 inc_value)
{
	massert_retval(target != NULL, ERR_INVALID_PARAM);
	target->GetMutableUnitAttr()->IncAttr(ACTOR_ATTR_HP, inc_value);
	return 0;
}