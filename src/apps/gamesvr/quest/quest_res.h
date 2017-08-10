#ifndef __UNIVERSE_QUEST_H__
#define __UNIVERSE_QUEST_H__
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
	QUEST_FINISH_COND_NONE = 0,
	QUEST_FINISH_COND_KILL_MONSTER = 1,
	QUEST_FINISH_COND_COLLECT_ENTITY = 2,
};
class QuestCommitKillMonsterCond : public QuestCommitInstCond
{
	public GetCondType();
	public int GetTargetValue();
	public TriggerEventData *BuildTriggerEventData();
	public TRIGGER_EVENT_TYPE GetTriggerEventType();
	public void SetMonsterId(int id);
	public void SetMonsterCnt(int cnt);
	int monster_id;
	int monster_cnt;
};
class QuestCommitCollectEntityCond : public QuestCommitInstCond
{
	public int GetCondType();
	public int GetTargetValue();
	public TriggerEventData *BuildTriggerEventData();
	public TRIGGER_EVENT_TYPE GetTriggerEventType();
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
public:
	virtual int ValidCondSatify(Actor *actor) = 0; //条件是否满足
};
class QuestCommitReachLevelCond : public QuestCommitConstCond
{
	int ValidCondSatify(Actor *actor);
private:
	int target_level;
};
class QuestCommitReachLevelCond : public QuestCommitConstCond
{
	int ValidCondSatify(Actor *actor);
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
	int ExecuteEvent(Actor *actor);
private:
	int item_id;
	int item_num;
};
public class QuestCommitAddMoney : public QuestCommitEvent
{
public:
	int ExecuteEvent(Actor *actor);
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
public:
	static QuestResMgr* GetInstance();
	int InitByTest();                         //测试用在内存中随便构造几个qust
	int InitByQuestFile();                    //WCCTODO:从文件中读取

	int AddQuestRes(QuestRes *res_quest);
	QuestRes* GetQuestResById(int quest_id);
private:
	std::map<int, QuestRes*> res_quest_map;
};


#endif