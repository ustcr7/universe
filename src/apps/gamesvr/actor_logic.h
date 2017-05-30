#ifndef __H_ACTOR_LOGIC__
#define __H_ACTOR_LOGIC__
#include "../../common/base_type.h"
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
	int ActorMoveReq(u64 conn_id, u64 id, Pos *pos);
private:
};


#endif // !__H_ACTOR_LOGIC__
