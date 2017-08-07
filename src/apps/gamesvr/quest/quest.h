#ifndef __UNIVERSE_QUEST_H__
#define __UNIVERSE_QUEST_H__

//version1 

//条件(接受、完成)
//事件
//奖励
//计数

//accept --> inc_counter --> commit
//


//任务实例
//eg1: 等级达到10级,20级,30级   战力达到 1000,10000,100000
//eg2: 杀怪物A   10只
//eg3: 采集物件A 5个

//奖励
//金钱, 道具


//完成条件:
//静态数据 const_cond
//动态数据 inst_cond 

//---------------------------接受条件---------------------------
class QuestAcceptCond
{
	QUEST_ACCEPT_COND_TYPE type;
};

//接受等级限制
class QuestAcceptActorLevelCond : public QuestAcceptCond
{
};
//接受战力限制
class QuestAcceptActorGsCond : public QuestAcceptCond
{
};
//---------------------------完成条件---------------------------
enum QUEST_FINISH_COND_TYPE
{
	QUEST_FINISH_COND_NONE               = 0,
	QUEST_FINISH_COND_KILL_MONSTER       = 1,
	QUEST_FINISH_COND_COLLECT_ENTITY     = 2,
};
class QuestCommitKillMonsterCond : public QuestCommitInstCond
{
	public GetCondType()
	{
		return QUEST_FINISH_COND_KILL_MONSTER;
	}
	public int GetTargetValue()
	{
		return monster_cnt;
	}
	public TriggerEventData *BuildTriggerEventData()
	{
		TriggerEventKillMonsterData *kill_monster_data = new TriggerEventKillMonsterData();
		massert_retval(data != NULL, NULL);
		kill_monster_data->SetMonsterId(monster_id);
		return kill_monster_data;
	}
	public TRIGGER_EVENT_TYPE GetTriggerEventType()
	{
		return TRIGGER_EVENT_KILL_MONSTER;
	}

	public void SetMonsterId(int id)
	{
		monster_id = id;
	}
	public void SetMonsterCnt(int cnt)
	{
		monster_cnt = cnt;
	}
	int monster_id;
	int monster_cnt;
};
class QuestCommitCollectEntityCond : public QuestCommitInstCond
{
	public GetCondType()
	{
		return QUEST_FINISH_COND_COLLECT_ENTITY;
	}
	public int GetTargetValue()
	{
		return entity_cnt;
	}
	public TriggerEventData *BuildTriggerEventData()
	{
		massert_retval(0, NULL); //WCCTODO: not implement
		return NULL;
	}
	public TRIGGER_EVENT_TYPE GetTriggerEventType()
	{
		return TRIGGER_EVENT_COLLECT_ENTITY;
	}
	int entity_id;
	int entity_cnt;
};
class QuestCommitInstCond
{
	public virtual QUEST_FINISH_COND_TYPE GetCondType() = 0;
	public virtual int GetTargetValue() = 0;
	public virtual TriggerEventData *BuildTriggerEventData() = 0;
	public virtual TRIGGER_EVENT_TYPE GetTriggerEventType() = 0;
};
class QuestCommitConstCond
{
public :
	virtual int ValidCondSatify(Actor *actor); //条件是否满足
};
class QuestCommitReachLevelCond : public QuestCommitConstCond
{
	int ValidCondSatify(Actor *actor)
	{
		return actor->GetLevel() >= target_level;
	}
private:
	int target_level;
};
class QuestCommitReachLevelCond : public QuestCommitConstCond
{
	int ValidCondSatify(Actor *actor)
	{
		return (actor->GetPos()->GetX() == pos_x && actor->GetPos()->GetY());
	}
private:
	int pos_x;
	int pos_y;
};

public class QuestCommitEvent
{
	public virtual int ExecuteEvent(Actor *actor) = 0;
}

public class QuestCommitGiveItem : public QuestCommitEvent
{
public:
	int ExecuteEvent(Actor *actor)
	{
		//add item;
	}
private:
	int item_id;
	int item_num;
};
public class QuestCommitAddMoney : public QuestCommitEvent
{
public:
	int ExecuteEvent(Actor *actor)
	{
		//add money;
	}
private:
	int money_type;
	int money_num;
};

static const int MAX_ACCEPT_COND_SIZE = 10;
static const int MAX_COMMIT_COND_SIZE = 10;
static const int MAX_COMMIT_EVENT_SIZE = 10;
class QuestRes
{
	int quest_id;

	int             accept_cond_cnt;
	QuestAcceptCond* accept_conds[MAX_ACCEPT_COND_SIZE];

	int                  inst_cond_cnt;
	QuestCommitInstCond*  commit_inst_conds[MAX_COMMIT_COND_SIZE];      //需要通过counter来记录发生时刻

	int                  const_cond_cnt;
	QuestCommitConstCond* commit_const_conds[MAX_COMMIT_COND_SIZE];     //永久性条件,在提交任务时校验一下即可

	int                  commit_event_cnt;
	QuestCommitEvent*     commit_event[MAX_COMMIT_EVENT_SIZE];

};

class QuestResMgr
{
	//测试用在内存中随便构造几个qust
	int InitByTest()
	{
		{
			QuestRes kill_monster_quest;
			kill_monster_quest.quest_id = 100001;
			kill_monster_quest.accept_cond_cnt = 0;
			kill_monster_quest.const_cond_cnt = 0;

			kill_monster_quest.inst_cond_cnt = 1;
			QuestCommitKillMonsterCond *kill_monster_cond = new QuestCommitKillMonsterCond();
			kill_monster_cond->SetMonsterId(2000000);
			kill_monster_cond->SetMonsterCnt(10);
			kill_monster_quest.commit_inst_conds[0] = kill_monster_cond;
			AddQuestRes(&kill_monster_quest);
		}
		{
			QuestRes collect_entity_quest;
			collect_entity_quest.quest_id = 100002;
			collect_entity_quest.accept_cond_cnt = 0;
			collect_entity_quest.const_cond_cnt = 0;

			collect_entity_quest.inst_cond_cnt = 1;
			QuestCommitCollectEntityCond *collect_entity_cond = new QuestCommitCollectEntityCond();
			collect_entity_quest->SetEntityId(2000000);
			collect_entity_quest->SetEntityCnt(10);
			collect_entity_quest.commit_inst_conds[0] = collect_entity_cond;
			AddQuestRes(&collect_entity_quest);
		}
	}
	//WCCTODO:从文件中读取
	int InitByQuestFile()
	{
		massert_retval(0, -1);
		return 0;
	}

	int AddQuestRes(QuestRes *res_quest)
	{
		if (GetQuestResById(res_quest->GetId()) != NULL)
		{
			return ERR_ALREADY_EXISTS;
		}
		res_quest_map.insert(std::make_pair(res_quest->GetId(), res_quest));
	}

	QuestRes* GetQuestResById(int quest_id)
	{
		std::map<int, QuestRes*>::iterator iter = res_quest_map.find(quest_id);
		if (iter == res_quest_map.end())
		{
			return NULL;
		}
		return iter->second;
	}

private:
	std::map<int, QuestRes*> res_quest_map;
};

static const int ACTOR_MAX_COUNTER_SIZE = 16; //最多16个条件
static u64 quest_mid = 0;
class ActorQuest
{
public:
	int InitByRes(QuestRes *res_quest)
	{
		mid = quest_mid++;
		//init counter
		quest_id = res_quest->quest_id;
		inst_cond_cnt = res_quest->const_cond_cnt;
		memset(inst_cond_counter, 0, sizeof(inst_cond_counter));

		//WCCTODO: parse accept/commit cond

		//quest commit cond --> counter --> init Trigger;
		inst_cond_cnt = 0;
		for (int i = 0; i < res_quest->commit_cond_cnt; ++i)
		{
			QuestCommitInstCond *cond = &res_quest->commit_inst_conds[i];

			TriggerData trigger;

			QuestCounterCallbackData *cb_data = new QuestCounterCallbackData();
			cb_data->quest_mid = mid;
			cb_data->counter_idx = inst_cond_cnt;
			cb_data->counter_target_value = cond->GetTargetValue();
			
			trigger.module_type = TRIGGER_MODULE_QUEST;
			trigger.calllback_data = cb;
			trigger.event_type = cond->GetTriggerEventType();
			trigger.event_data = cond->BuildTriggerEventData();

			inst_cond_cnt++;
		}
		return 0;
	}
private:
	int quest_id;
	int mid; //内存ID
	//counter
	int inst_cond_cnt;
	int inst_cond_counter[MAX_COMMIT_COND_SIZE];
};

class ActorQuestMgr
{
public:
	static ActorQuestMgr* GetInstance();
	//int AddActorQuest(ActorQuest *actor_quest);
	ActorQuest* AllocActorQuest();
	void        FreeActorQuest();
	const ActorQuest* GetActorQuest(u64 quest_mid) const;
private:
	std::map<u64, ActorQuest*> actor_quest_map;
};

static const int ACTTOR_MAX_QUEST_ARRAY_SIZE = 1024;
class ActorQuestArray
{
	int quest_count;
	ActorQuest quests[ACTTOR_MAX_QUEST_ARRAY_SIZE];
};

//---------------------------quest api---------------------------
int _commit_valid_const_quest_condition(Actor *actor, QuestRes *res_quest)
{
	for (int i = 0; i < res_quest->const_cond_cnt; ++i)
	{
		ret = res_quest->commit_const_conds[i].ValidCondSatify(actor);
		if (ret != 0)
		{
			return ERR_QUEST_TARGET_NO_FINISH;
		}
	}
}

int _commit_valid_inst_quest_condition(ActorQuest *actor_quest, QuestRes *res_quest)
{
	for (int i = 0; i < actor_quest->inst_cond_cnt; ++i)
	{
		if (actor_quest->inst_cond_counter[i] < res_quest->commit_inst_conds[i].target_value)
		{
			return ERR_QUEST_TARGET_NO_FINISH;
		}
	}
	return 0;
}

//杀怪类任务
//杀丛林小怪(id:10001)20只
//杀雪山银狐(id:20001)10只

//采集类任务
//采集鸟蛋(entity id:30001)1个
//采集药草(entity id:30001)5个

//对话内任务
//与戊阳子(npcid:12000)对话
//与老村长(npcid:22000)对话

int actor_accept_quest(Actor *actor, int quest_id)
{
	massert_retval(actor != NULL, ERR_INVALID_ARG);

	ActorQuest *actor_quest = actor->GetQuest(quest_id);
	if (actor_quest != NULL)
	{
		return ERR_QUEST_ALREADY_ACCEPTED;
	}

	actor_quest = new ActorQuest(quest_id);
	xy_assert_retval(actor_quest != NULL, ERR_BAD_ALLOC);

	QuestRes *quest_res = QuestResMgr::GetInstance()->GetQuest(quest_id);
	actor_quest->InitByRes(res_quest);

	return 0;
}

int actor_commit_quest(Actor *actor, int quest_id)
{
	massert_retval(actor != NULL, ERR_INVALID_ARG);
	ActorQuest *actor_quest = actor->GetQuest(quest_id);
	if (actor_quest == NULL)
	{
		return ERR_QUEST_NOT_ACCEPT;
	}

	QuestRes *quest_res = QuestResMgr::GetInstance()->GetQuest(quest_id);
	massert_retval(quest_res != NULL, ERR_INVALID_PARAM);

	//验证const conditions
	ret = _commit_valid_const_quest_condition(actor, res_quest);
	if (ret != 0)
	{
		return ret;
	}
	//验证inst conditions
	ret = _commit_valid_inst_quest_condition(actor_quest, res_quest);
	if (ret != 0)
	{
		return ret;
	}

	//给予奖励
	for (int i = 0; i < res_quest->commit_event_cnt; ++i)
	{
		res_quest->commit_event.ExecuteEvent(actor);
	}
	return 0;
}


#endif