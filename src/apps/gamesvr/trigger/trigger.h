class Trigger
{

};


class ActorTrigger
{

};

quest_trigger_call_back()
{

}

enum TRIGGER_EVENT_TYPE
{
	TRIGGER_EVENT_NONE           = 0,
	TRIGGER_EVENT_KILL_MONSTER   = 1,
	TRIGGER_EVENT_COLLECT_ENTITY = 2,
	TRIGGER_EVENT_TALK_NPC       = 3,
	TRIGGER_EVENT_MAX            = 4,
};

enum TRIGGER_MODULE_TYPE
{
	TRIGGER_MODULE_NONE         = 0,
	TRIGGER_MODULE_QUEST        = 1,
	TRIGGER_MODULE_MAX          = 2,
};

class TriggerEventKillMonsterData : public TriggerEventData
{
public:
	bool IsMatch(const TriggerParamData *trigger_param) const 
	{
		const KillMonsterTriggerParamData *kill_monster_param = (const KillMonsterTriggerParamData *)trigger_param;
		massert_retval(kill_monster_param != NULL, false);

		return kill_monster_param->GetMonsterId() == monster_id;
	}
	void SetMonsterId(int id)
	{
		monster_id = id;
	}
private:
	int monster_id;
};
class TriggerEventCollectNpcData : public TriggerEventData
{
	int entity_id;
};
class TriggerEventData
{
public:
	bool IsMatch(const TriggerParamData *trigger_param) virtual = 0;
};

class QuestCounterCallbackData : public TriggerCallbackData
{
	int quest_mid;
	int counter_idx;
	int counter_target_value;
};
class TriggerCallbackData
{
};

class TriggerData
{
public:
	TriggerData()
	{
		event_type  = TRIGGER_EVENT_NONE;
		module_type = TRIGGER_MODULE_NONE;
	}
	TRIGGER_EVENT_TYPE GetEventType()
	{
		return event_type;
	}
	void SetEventType(TRIGGER_EVENT_TYPE etype)
	{
		event_type = etype;
	}
	bool IsMatch(const TriggerParamData *trigger_param) const
	{
		return event_data.IsMatch(const TriggerParamData *trigger_param);
	}
private:
	TRIGGER_EVENT_TYPE       event_type;
	TRIGGER_MODULE_TYPE      module_type;
	TriggerEventData         *event_data;
	TriggerCallbackData      *calllback_data;
};

class TriggerList
{
	std::vector<TriggerData> triggers;
};
class ActorTriggerData
{
	TriggerList trigger_lists[TRIGGER_EVENT_MAX];
};
//TRIGGER_TYPE_KILL_MONSTER   (filter: monster id)(filter function)    (handle function)
//TRIGGER_TYPE_COLLECT_ENTITY (filter: entity id) (filter function)    (handle function)
//TRIGGER_TYPE_TALK_NPC       (filter: entity id) (filter function)    (handle function)

int OnEventKillActor(Actor *killer, Actor *victim)
{
	//quest-> couter (KillActorType) ��1

	//����quester counter:
	// questid counter id
}

class CollectEntityTriggerParamData : public TriggerParamData
{
	int entity_id;
};

class KillMonsterTriggerParamData : public TriggerParamData
{
public:
	int GetMonsterId()
	{
		return monster_id;
	}
private:	
	int monster_id;
};

class TriggerParamData
{
	u64 actor_rid;
};

int quest_counter_trigger_filter_function(TriggerData *trigger_data, TriggerParamData *trigger_param)
{
	switch (trigger_data->event_type)
	{
	case TRIGGER_EVENT_NONE:
	{
		massert_retval(0, ERR_INVALID_PARAM);
		break;
	}
	case TRIGGER_EVENT_KILL_MONSTER:
	{
		break;
	}
	case TRIGGER_EVENT_COLLECT_ENTITY:
	case TRIGGER_EVENT_TALK_NPC:
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

int quest_counter_trigger_handle_function(TriggerData *trigger_data, TriggerParamData *trigger_param)
{
	KillMonsterTriggerParamData *praram_data = (KillMonsterTriggerParamData*)trigger_param;

	QuestCounterCallbackData *callback_data = *trigger_data->calllback_data;
	u64 actor_rid = praram_data->actor_rid;

	u64 quest_mid        = callback_data->quest_mid;
	int counter_idx = callback_data->counter_idx;
	int target_value = callback_data->counter_target_value;

	Actor *actor = ActorMgr::GetInstance()->GetActor(actor_rid);
	massert_retval(actor != NULL, ERR_INVALID_PARAM);
	ActorQuest *actor_quest = actor->GetQuest(quest_id);
	massert_retbal(actor_quest != NULL, ERR_INVALID_PARAM);

	massert_retval(counter_idx >= 0 && counter_idx < actor_quest->inst_cond_cnt, ERR_INVALID_PARAM);
	if (actor_quest->inst_cond_counter[counter_idx] >= target_value)
	{
		return 0; //WCCTODO:ʵ�������triggerӦ���ں���ɾ����,��Ϊ�������Ѿ����,û��Ҫ��ȥ��������
	}
	actor_quest->inst_cond_counter[counter_idx] += add_value;

	return 0;
}
typedef (*TRIGGER_FILTER_FUNC)(TriggerData *trigger_data, TriggerParamData *trigger_param);
typedef (*TRIGGER_HANDLE_FUNC)(TriggerData *trigger_data, TriggerParamData *trigger_param);
static TriggerMgr *trigger_mgr = NULL;
class TriggerMgr
{
	static TriggerMgr* GetInstance()
	{
		if (trigger_mgr == NULL)
		{
			trigger_mgr = new TriggerMgr();
			massert_retval(trigger_mgr != NULL, NULL);
			return trigger_mgr;
		}
	}
	const TRIGGER_FILTER_FUNC GetTriggerFilterFunc(TRIGGER_MODULE_TYPE module_type) const
	{
		massert_retval(module_type > TRIGGER_MODULE_NONE && module_type < TRIGGER_MODULE_MAX, NULL);
		return filter_funcs[module_type];
	}
	const TRIGGER_HANDLE_FUNC GetTriggerHandleFunc(TRIGGER_MODULE_TYPE module_type) const
	{
		massert_retval(module_type > TRIGGER_MODULE_NONE && module_type < TRIGGER_MODULE_MAX, NULL);
		return handle_funcs[module_type];
	}
	int RegisterTriggerFunction(TRIGGER_MODULE_TYPE module_type, TRIGGER_FILTER_FUNC filter_func, TRIGGER_HANDLE_FUNC handle_func)
	TRIGGER_FILTER_FUNC filter_funcs[TRIGGER_MODULE_MAX];
	TRIGGER_HANDLE_FUNC handle_funcs[TRIGGER_MODULE_MAX];
};

//WCC_TODO: trigger�п��Գ���һ��module_type
//��ͬ��moudle��trigger�Ĵ���һ��,eg:����ģ��������counter����, ���ɾ�ģ�����ʱ�޸�ĳЩ������ͳ��ֵ�ȵ�
int OnTriggerEvent(Actor *actor, TRIGGER_TYPE trigger_type, TriggerParamData *param_data)
{
	massert_retval(trigger_type > TRIGGER_TYPE_NONE && trigger_type < TRIGGER_TYPE_MAX, ERR_INVALID_PARAM);

	std::vector<TriggerData> &triggers = actor->trigger_data.trigger_lists[trigger_type];
	for (int i = 0; i < triggers.size(); ++i)
	{
		//WCCTODO:������ɾ��trigger,�Ǻ�Σ�յ�
		TriggerData *trigger_data = triggers.at(i);
		const TRIGGER_FILTER_FUNC filter_func = TriggerMgr::GetInstance()->GetTriggerFilterFunc(trigger_data->trigger_type);
		massert_retval(filter_func != NULL, ERR_INVALID_PARAM);
		if (0 == filter_func(trigger_data, param_data)) //���˳ɹ�
		{
			const TRIGGER_HANDLE_FUNC handle_func = TriggerMgr::GetInstance()->GetTriggerHandleFunc(trigger_data->trigger_type);
			massert_retval(handle_func != NULL, ERR_INVALID_PARAM);
			handle_func(trigger_data, param_data);
		}
	}

	//WCCTODO: trigger��ʲôʱ���ɾ��? (ANNSER: trigger�� value=targetʱ����ɾ��)
}

//example 
int KillMonsterNotiy(Actor *actor, int monster_id)
{
	KillMonsterTriggerParamBackData param_data;
	param_data.monster_id = monster_id;

	OnTiggerEvent(actor, TRIGGER_TYPE_KILL_MONSTER, &param_data);
}

