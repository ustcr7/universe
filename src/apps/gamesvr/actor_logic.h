#ifndef __H_ACTOR_LOGIC__
#define __H_ACTOR_LOGIC__
#include "../../common/base_type.h"
#include "universe_cs.pb.h"
//WCC_TODO: put all logic here, when logic is too complex, devide logic to different files

struct Pos;

class ActorReqHandle
{
public:
	ActorReqHandle();
	static ActorReqHandle* GetSingleInstance();
	int ActorRegisteReq(u64 conn_id, u64 id, const char *name);
	int ActorLoginReq(u64 conn_id, u64 id);
	int ActorLogoutReq(u64 conn_id, u64 id);
	int ActorGetFullData(u64 conn_id, u64 id);
	int ActorMoveReq(u64 conn_id, u64 id, Pos *pos, int pos_cnt);
	int ActorChatReq(u64 connId, u64 srcActorid, u64 dstActorId, ChatType chatType, const char *content);
	int ActorLearnSpellReq(u64 connId, u64 actor_rid, int spellid);
	int ActorCastSpellReq(u64 connId, u64 actor_rid, u64 target_rid, int spellid);
	//int ActorAcceptQuestReq(u64 connId, u64 actor_rid, int quest_id);
	//int ActorCommitQuestReq(u64 connId, u64 actor_rid, int quest_id);
private:
};


#endif // !__H_ACTOR_LOGIC__
