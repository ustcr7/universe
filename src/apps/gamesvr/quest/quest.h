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
class QuestFinishCond
{
	QUEST_FINISH_COND_TYPE type;
};
class QuestFinishKillMonsterCond : public QuestFinishCond
{
	int monster_id;
	int kill_num;
};
static const int MAX_ACCEPT_COND_SIZE = 10;
static const int MAX_FINISH_COND_SIZE = 10;
class QuestRes
{
	int quest_id;
	QuestAcceptCond accept_conds[MAX_ACCEPT_COND_SIZE];
	QuestFinishCond commit_conds[MAX_FINISH_COND_SIZE];
};

static const int ACTOR_MAX_COUNTER_SIZE = 16; //���16������
class ActorQuest
{
	int quest_id;
	

	//counter
	int counter[ACTOR_MAX_COUNTER_SIZE];
};

//---------------------------quest api---------------------------
int actor_commit_quest(Actor *actor, int quest_id)
{
	massert_retval(actor != NULL, ERR_INVALID_ARG);
	ActorQuest *quest = actor->GetQuest(quest_id);
	if (quest == NULL)
	{
		return ERR_QUEST_NOT_ACCEPT;
	}
	

	//��������������



}

#endif