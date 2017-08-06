/*
class Trigger
{

};


class ActorTrigger
{

};

quest_trigger_call_back()
{

}

enum TRIGGER_TYPE
{
	TRIGGER_TYPE_NONE           = 0,
	TRIGGER_TYPE_KILL_MONSTER   = 1,
	TRIGGER_TYPE_COLLECT_ENTITY = 2,
	TRIGGER_TYPE_TALK_NPC       = 3,
	TRIGGER_TYPE_MAX            = 4,
};

enum TRIGGER_HANDLE_ID
{
	TRIGGER_HANDLE_NONE = 0,
	TRIGGER_HANDLE_QUEST = 1,
	TRIGGER_HANDLE_ACHEVEMENT = 2,
};

class TriggerData
{
private:
	TRIGGER_HANDLE_ID handle_id;

	FILTER_FUNCTION filter_function;
	HANDLE_FUNCTION handle_function;
	int filter_target_value;  //过滤条件的参考值,eg 怪物ID entity ID等

	int handle_target_value1;  //quest, counteridx
	int handle_target_value2;
};

class TriggerList
{
	std::vector<TriggerData> triggers;
};
class ActorTriggerData
{
	TriggerList trigger_lists[TRIGGER_TYPE_MAX];
};
//TRIGGER_TYPE_KILL_MONSTER   (filter: monster id)(filter function)    (handle function)
//TRIGGER_TYPE_COLLECT_ENTITY (filter: entity id) (filter function)    (handle function)
//TRIGGER_TYPE_TALK_NPC       (filter: entity id) (filter function)    (handle function)

int OnEventKillActor(Actor *killer, Actor *victim)
{
	//quest-> couter (KillActorType) 加1

	//增加quester counter:
	// questid counter id
}



int quest_trigger_filter_function(TRIGGER_TYPE trigger_type, int value, int target_value)
{
	switch (trigger_type)
	{
	case TRIGGER_TYPE_KILL_MONSTER:
	case TRIGGER_TYPE_COLLECT_ENTITY:
	case TRIGGER_TYPE_TALK_NPC:
		if (value == target_value)
		{
			return true;
		}
		break;
	default:
		return true;
		break;
	}
	return false;
}

int quest_trigger_handle_function(u64 actor_rid, int quest id, int couter idx, int add_value)
{
	Actor *actor = ActorMgr::GetInstance()->GetActor(actor_rid);
	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	ActorQuest *actor_quest = actor->GetQuest(quest_id);
	massert_retbal(actor_quest != NULL, ERR_INVALID_PARAM);

	massert_retval(counter_idx >= 0 && counter_idx < actor_quest->inst_cond_cnt, ERR_INVALID_PARAM);
	actor_quest->inst_cond_counter[counter_idx] += add_value;

	return 0;
}


int OnTriggerEvent(Actor *actor, TRIGGER_TYPE trigger_type, int filter_target_value, int target_value)
{
	massert_retval(trigger_type > TRIGGER_TYPE_NONE && trigger_type < TRIGGER_TYPE_MAX, ERR_INVALID_PARAM);

	std::vector<TriggerData> &triggers = actor->trigger_data.trigger_lists[trigger_type];
	for (int i = 0; i < triggers.size(); ++i)
	{
		//WCCTODO:遍历中删除trigger,是很危险的
		TriggerData *trigger_data = triggers.at(i);
		if (0 == trigger_data->filter_function(filter_target_value, trigger_data->filter_target_value)) //过滤成功
		{
			trigger_data->handle_function(actor->GetId(), trigger_data->handle_target_value1, trigger_data->handle_target_value2);
		}
	}

	//WCCTODO: trigger在什么时候该删除? (ANNSER: trigger中 value=target时可以删除)
}

//example 
int KillMonsterNotiy(Actor *actor, int monster_id)
{
	OnTiggerEvent(actor, TRIGGER_TYPE_KILL_MONSTER, monster_id, 1);
}

*/