#include "gamesvr_def.h"
#include "actor.h"
#include <string.h>
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "db/gamesvr_db.h"
#include "uv_unit.h"
#include "battle/battle.h"
#include "universe_db.pb.h"


Actor::Actor()
{
	_connId = 0;
}

int Actor::SetConnId(u64 connId)
{
	_connId = connId;
	return 0;
}
u64 Actor::GetConnId() const
{
	return _connId;
}


int Actor::GetSpeed()const
{
	return speed;
}

int Actor::SetSpeed(int s)
{
	speed = s;
	printf("actor set speed %d\n", speed);
	return 0;
}

int Actor::GetCurPathIndex()
{
	return cur_path_index;
}
int Actor::SetCurPathIndex(int index)
{
	cur_path_index = index;
	//printf("actor set new path index:%d\n", index);
	return 0;
}

int Actor::DumpToDb(ActorDB *db_actor) const
{
	printf("rt actor %llu dump to dp\n", GetId());
	massert_retval(db_actor != NULL, ERR_INVALID_PARAM);
	db_actor->SetActorRid(GetId());
	db_actor->SetActorName(GetName());

	DbActorGameData db_game_data;
	db_game_data.set_speed(GetSpeed());
	db_game_data.mutable_pos()->set_pos_x(GetPos()->GetX());
	db_game_data.mutable_pos()->set_pos_x(GetPos()->GetY());
	for (int attr_type = 0; attr_type < ACTOR_ATTR_TYPE_MAX; ++attr_type)
	{
		int attr_value = GetUnitAttr()->GetAttr((ACTOR_ATTR_TYPE)attr_type);
		db_game_data.add_attrs(attr_value);
	}
	db_game_data.set_instance_id(GetInstanceId());
	//DbUnitSpellBook *db_spell_book = db_game_data.mutable_spellbook();
	//WCC_TODO初始化一下

	DbUnitState *db_unit_state = db_game_data.mutable_unitstate();
	int state = (int)GetUnitState()->GetUnitState();
	db_unit_state->set_state(state);

	int data_size = db_game_data.ByteSizeLong();
	db_game_data.SerializeToArray(db_actor->GetMutableGameDataBlob(), data_size);
	db_actor->SetGameDataBlobSize(data_size);

	printf("dump success, game data bolb size:%d\n", data_size);

	return 0;
}


int Actor::InitFromDb(const ActorDB *db_actor)
{
	massert_retval(db_actor != NULL, ERR_INVALID_PARAM);

	SetId(db_actor->GetActorRid());
	SetName(db_actor->GetActorName());

	DbActorGameData db_game_data;
	db_game_data.ParseFromArray(db_actor->GetGameDataBlob(), db_actor->GetGameDataBolbSize());

	SetSpeed(db_game_data.speed());
	const DbPos& db_pos = db_game_data.pos();
	Pos pos;
	pos.SetX(db_pos.pos_x());
	pos.SetY(db_pos.pos_y());
	SetPos(&pos);

	for (int attr_type = 0; attr_type < db_game_data.attrs_size(); ++attr_type)
	{
		GetMutableUnitAttr()->SetAttr((ACTOR_ATTR_TYPE)attr_type, db_game_data.attrs(attr_type));
	}

	SetInstanceId(db_game_data.instance_id());

	UnitSpellBook *rt_spell_book = this->GetMutableSpellBook();
	const DbUnitSpellBook &db_spell_book = db_game_data.spellbook();
	for (int i = 0; i < db_spell_book.spellinfos_size(); ++i)
	{
		UnitSpellInfo rt_spellInfo;
		rt_spellInfo.SetSpellId(db_spell_book.spellinfos(i).spell_id());
		rt_spellInfo.SetLastCastTime(db_spell_book.spellinfos(i).last_cast_time());

		rt_spell_book->AddUnitSpellInfo(&rt_spellInfo);
	}

	GetMutableUnitState()->SetUnitState((UV_UNIT_STATE_TYPE)db_game_data.unitstate().state());

	printf("init from db success, id:%llu name:%s, pos:<%d-%d>\n", GetId(), GetName(), GetPos()->GetX(), GetPos()->GetY());
	for (int i = 0; i < db_game_data.attrs_size(); ++i)
	{
		printf("attr %d, value:%d\n", i, (int)GetUnitAttr()->GetAttr((ACTOR_ATTR_TYPE)i));
	}
	printf("spell count:%d\n", GetSpellBook()->GetSpellCount());

	return 0;
}