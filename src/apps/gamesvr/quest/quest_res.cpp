#include "quest_res.h"

public QuestCommitKillMonsterCond::GetCondType()
{
	return QUEST_FINISH_COND_KILL_MONSTER;
}
public int QuestCommitKillMonsterCond::GetTargetValue()
{
	return monster_cnt;
}
public TriggerEventData * QuestCommitKillMonsterCond::BuildTriggerEventData()
{
	TriggerEventKillMonsterData *kill_monster_data = new TriggerEventKillMonsterData();
	massert_retval(data != NULL, NULL);
	kill_monster_data->SetMonsterId(monster_id);
	return kill_monster_data;
}
public TRIGGER_EVENT_TYPE QuestCommitKillMonsterCond::GetTriggerEventType()
{
	return TRIGGER_EVENT_KILL_MONSTER;
}

public void QuestCommitKillMonsterCond::SetMonsterId(int id)
{
	monster_id = id;
}
public void QuestCommitKillMonsterCond::SetMonsterCnt(int cnt)
{
	monster_cnt = cnt;
}

int QuestCommitCollectEntityCond::GetCondType()
{
	return QUEST_FINISH_COND_COLLECT_ENTITY;
}
int QuestCommitCollectEntityCond::GetTargetValue()
{
	return entity_cnt;
}
TriggerEventData *  QuestCommitCollectEntityCond::BuildTriggerEventData()
{
	massert_retval(0, NULL); //WCCTODO: not implement
	return NULL;
}
TRIGGER_EVENT_TYPE QuestCommitCollectEntityCond::GetTriggerEventType()
{
	return TRIGGER_EVENT_COLLECT_ENTITY;
}

int QuestCommitReachLevelCond::ValidCondSatify(Actor *actor)
{
	return actor->GetLevel() >= target_level;
}

int QuestCommitReachLevelCond::ValidCondSatify(Actor *actor)
{
	return (actor->GetPos()->GetX() == pos_x && actor->GetPos()->GetY());
}


int QuestCommitGiveItem::ExecuteEvent(Actor *actor)
{
	//add item;
}

int QuestCommitAddMoney::ExecuteEvent(Actor *actor)
{
	//add money;
}


static QuestResMgr gs_quest_res_mgr = NULL;
static QuestResMgr* QuestResMgr::GetInstance()
{
	if (gs_quest_res_mgr == NULL)
	{
		gs_quest_res_mgr = new;
	}
	massert_retval(gs_quest_res_mgr != NULL, NULL);
	return gs_quest_res_mgr;
}

//测试用在内存中随便构造几个qust
int QuestResMgr::InitByTest()
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

	return 0;
}

int QuestResMgr::InitByQuestFile()
{
	massert_retval(0, -1); //not implement
	return 0;
}

int QuestResMgr::AddQuestRes(QuestRes *res_quest)
{
	if (GetQuestResById(res_quest->GetId()) != NULL)
	{
		return ERR_ALREADY_EXISTS;
	}
	res_quest_map.insert(std::make_pair(res_quest->GetId(), res_quest));
}

QuestRes* QuestResMgr::GetQuestResById(int quest_id)
{
	std::map<int, QuestRes*>::iterator iter = res_quest_map.find(quest_id);
	if (iter == res_quest_map.end())
	{
		return NULL;
	}
	return iter->second;
}
