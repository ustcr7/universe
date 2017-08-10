#ifndef __UNIVERSE_QUEST_H__
#define __UNIVERSE_QUEST_H__

//玩家任务定义
static const int ACTOR_MAX_COUNTER_SIZE = 16; //最多16个条件
static u64 quest_mid = 0;
class ActorQuest
{
public:
	int GetQuestId();
	int InitByRes(QuestRes *res_quest);
private:
	int quest_id;
	int mid; //内存ID
			 //counter
	int inst_cond_cnt;
	int inst_cond_counter[MAX_COMMIT_COND_SIZE];
};

//任务管理器
class ActorQuestMgr
{
public:
	static ActorQuestMgr* GetInstance();
	AccepQuest(Actor *actor, int quest_id);
	CommitQuest(Actor *actor, int quest_id);

private:
	//int AddActorQuest(ActorQuest *actor_quest);
	ActorQuest* AllocActorQuest();
	void        FreeActorQuest(u64 quest_mid);
	const ActorQuest* GetActorQuest(u64 quest_mid) const;
private:
	std::map<u64, ActorQuest*> actor_quest_map;
};


//玩家身上的任务数组
static const int ACTTOR_MAX_QUEST_ARRAY_SIZE = 1024;
class ActorQuestArray
{
public:
	ActorQuest* GetActorQuest(int quest_id);
	int AddQuest(ActorQuest* quest);
private:
	int quest_count;
	ActorQuest* quests[ACTTOR_MAX_QUEST_ARRAY_SIZE];
};


#endif