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

static const int ACTOR_MAX_COUNTER_SIZE = 16; //最多16个条件
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
	

	//检查任务完成条件



}

#endif