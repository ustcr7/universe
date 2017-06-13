#ifndef __H_GAMESVR_MSG_HANDLE_H__
#define __H_GAMESVR_MSG_HANDLE_H__
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "universe_cs.pb.h"

class Pos;
class Actor;
class UniverseMsg;
class ConnMsg;

class GamesvrMsgProcesser
{
public:
	static GamesvrMsgProcesser* GetSingleInstance();

	//recv func
	int RecvActorReq(u64 connId, const UniverseMsg *msg);

	//send func
	int SendActorRegisteRsp(u64 connId, u64 acotrId, int result);
	int SendActorLoginRsp(u64 connId, u64 id, int result);
	int SendActorLogoutRsp(u64 connId, u64 id, int result);
	int SendActorFullDataRsp(u64 connId, Actor *actor);
	int SendActorSetPosRsp(u64 connId, u64 id, Pos *pos);
	int SendForwardChatInfo(u64 connId
		, ChatType chatType
		, u64 srcActorid
		, u64 dstActorId
		, const char *content
		, int content_len);

private:
	int SendMsgByTcpServer(ConnMsg *msg);
};


#endif // !__H_GAMESVR_MSG_HANDLE_H__
