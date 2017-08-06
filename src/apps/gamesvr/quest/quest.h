#ifndef __UNIVERSE_QUEST_H__
#define __UNIVERSE_QUEST_H__

//version1 

//����(���ܡ����)
//�¼�
//����
//����

//accept --> inc_counter --> commit
//


//����ʵ��
//eg1: �ȼ��ﵽ10��,20��,30��   ս���ﵽ 1000,10000,100000
//eg2: ɱ����A   10ֻ
//eg3: �ɼ����A 5��

//����
//��Ǯ, ����


//�������:
//��̬���� const_cond
//��̬���� inst_cond 

//---------------------------��������---------------------------
class QuestAcceptCond
{
	QUEST_ACCEPT_COND_TYPE type;
};

//���ܵȼ�����
class QuestAcceptActorLevelCond : public QuestAcceptCond
{
};
//����ս������
class QuestAcceptActorGsCond : public QuestAcceptCond
{
};
//---------------------------�������---------------------------
class QuestCommitInstCond
{
	QUEST_FINISH_COND_TYPE type;
	int target_value;
};
class QuestCommitConstCond
{
public :
	virtual int ValidCondSatify(Actor *actor); //�����Ƿ�����
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
	QuestAcceptCond accept_conds[MAX_ACCEPT_COND_SIZE];
	int                  inst_cond_cnt;
	QuestCommitInstCond  commit_inst_conds[MAX_COMMIT_COND_SIZE];      //��Ҫͨ��counter����¼����ʱ��
	int                  const_cond_cnt;
	QuestCommitConstCond commit_const_conds[MAX_COMMIT_COND_SIZE];     //����������,���ύ����ʱУ��һ�¼���
	int                  commit_event_cnt;
	QuestCommitEvent     commit_event[MAX_COMMIT_EVENT_SIZE];

};

static const int ACTOR_MAX_COUNTER_SIZE = 16; //���16������
class ActorQuest
{
public:
	int InitByRes(QuestRes *res_quest)
	{
		//init counter
		quest_id = res_quest->quest_id;
		inst_cond_cnt = res_quest->const_cond_cnt;
		memset(inst_cond_counter, 0, sizeof(inst_cond_counter));
	}
private:
	int quest_id;
	//counter
	int inst_cond_cnt;
	int inst_cond_counter[MAX_COMMIT_COND_SIZE];
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

//ɱ��������
//ɱ����С��(id:10001)20ֻ
//ɱѩɽ����(id:20001)10ֻ

//�ɼ�������
//�ɼ���(entity id:30001)1��
//�ɼ�ҩ��(entity id:30001)5��

//�Ի�������
//��������(npcid:12000)�Ի�
//���ϴ峤(npcid:22000)�Ի�

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

	//add trigger for actor:


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

	//��֤const conditions
	ret = _commit_valid_const_quest_condition(actor, res_quest);
	if (ret != 0)
	{
		return ret;
	}
	//��֤inst conditions
	ret = _commit_valid_inst_quest_condition(actor_quest, res_quest);
	if (ret != 0)
	{
		return ret;
	}

	//���轱��
	for (int i = 0; i < res_quest->commit_event_cnt; ++i)
	{
		res_quest->commit_event.ExecuteEvent(actor);
	}
	return 0;
}

//ActorDead, MonsterDead, OnEntityCollected:
int actor_quest_inc_counter(Actor *actor);




//WCCTODO:Ч�ʺܵ�,�ĳ�trigger
int actor_quest_on_kill_monster(Actor *actor, u64 monster_mid)

int actor_quest_on_collect_entity(Actor *actor, int entity_id)
{
	ActorQuestArray *quest_array = &actor->quest_array;
	for (int i = 0; i < quest_array.quest_count; ++i)
	{
		ActorQuest *actor_quest = &quest_array.quests[i];
		if (actor_quest->)
		{

		}
	}
}

#endif