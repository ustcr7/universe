#include "quest.h"

int ActorQuest::InitByRes(QuestRes *res_quest)
{
	quest_id = res_quest->quest_id;
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

ActorQuestMgr::AccepQuest(Actor *actor, int quest_id)
{
	massert_retval(actor != NULL, ERR_INVALID_ARG);

	ActorQuest *actor_quest = actor->GetQuest(quest_id);
	if (actor_quest != NULL)
	{
		return ERR_QUEST_ALREADY_ACCEPTED;
	}

	actor_quest = AllocActorQuest()
	xy_assert_retval(actor_quest != NULL, ERR_BAD_ALLOC);

	QuestRes *quest_res = QuestResMgr::GetInstance()->GetQuest(quest_id);
	actor_quest->InitByRes(res_quest);

	return 0;
}
ActorQuestMgr::CommitQuest(Actor *actor, int quest_id)
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


static ActorQuestMgr gs_actor_quest_mgr = NULL;
static ActorQuestMgr* ActorQuestMgr::GetInstance()
{
	if (gs_actor_quest_mgr == NULL)
	{
		gs_actor_quest_mgr = new ActorQuestMgr;
	}
	return gs_actor_quest_mgr;
}

ActorQuest* ActorQuestMgr::AllocActorQuest()
{
	ActorQuest *quest = new ActorQuest();
	massert_retval(quest != NULL, ERR_BAD_ALLOC);

	quest->mid = quest_mid++;

	return quest;
}
void ActorQuestMgr::FreeActorQuest()
{

}
const ActorQuest* 	ActorQuestMgr::GetActorQuest(u64 quest_mid) const
{

}


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

