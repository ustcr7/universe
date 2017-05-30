#include "gamesvr_msg_processer.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/net/tcp_server.h"
#include "universe_cs.pb.h"
#include "actor_logic.h"
#include "actor.h"

static GamesvrMsgProcesser * gs_gamesvr_msg_processer = NULL;
GamesvrMsgProcesser* GamesvrMsgProcesser::GetSingleInstance()
{
	if (gs_gamesvr_msg_processer == NULL)
	{
		gs_gamesvr_msg_processer = new GamesvrMsgProcesser();
		massert_retval(gs_gamesvr_msg_processer != NULL, NULL);
	}
	return gs_gamesvr_msg_processer;
};

int GamesvrMsgProcesser::RecvActorReq(u64 connId, const UniverseMsg *msg)
{
	ActorReqHandle *req_handle = ActorReqHandle::GetSingleInstance();
	massert_retval(req_handle != NULL, ERR_INVALID_PARAM);

	printf("[GamesvrMsgProcesser]recv client msg msg id:%d\n", msg->msghead().msgid());

	int ret = 0;
	switch (msg->msghead().msgid())
	{
	case UNIVERSE_MSG_ID_ACTOR_REGISTE_REQ:
	{
		ret = req_handle->ActorRegisteReq(connId
			                           , msg->msgbody().registereq().id()
			                           , msg->msgbody().registereq().name().c_str());
		break;
	}
	case UNIVERSE_MSG_ID_ACTOR_LOGIN_REQ:
	{
		ret = req_handle->ActorLoginReq(connId
			                          , msg->msgbody().loginreq().id());
		break;
	}
	case UNIVERSE_MSG_ID_ACTOR_LOGOUT_REQ:
	{
		ret = req_handle->ActorLogoutReq(connId
			                          , msg->msgbody().logoutreq().id());
		break;
	}
	case UNIVERSE_MSG_ID_ACTOR_GET_FULL_DATA_REQ:
	{
		ret = req_handle->ActorGetFullData(connId
			, msg->msgbody().getfulldatareq().id());
		break;
	}
	case UNIVERSE_MSG_ID_ACTOR_SET_POS_REQ:
	{
		massert_noeffect(0); //not implement
		break;
	}
	default:
	{
		massert_retval(0, ERR_INVALID_PARAM);
		break;
	}
	}

	return ret;
}

int GamesvrMsgProcesser::SendActorRegisteRsp(u64 connId, u64 acotrId, int result)
{
	ConnMsg connMsg;
	connMsg.connId = connId;
	UniverseMsg *msgData = &connMsg.msg;
	msgData->mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_REGISTE_RSP);
	msgData->mutable_msgbody()->mutable_registersp()->set_result(result);

	printf("send registe rsp actor:%llu result:%d\n", acotrId, result);

	SendMsgByTcpServer(&connMsg);

	return 0;
};

int GamesvrMsgProcesser::SendActorLoginRsp(u64 connId, u64 id, int result)
{
	ConnMsg connMsg;
	connMsg.connId = connId;
	UniverseMsg *msgData = &connMsg.msg;
	msgData->mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_LOGIN_RSP);
	msgData->mutable_msgbody()->mutable_loginrsp()->set_result(result);

	SendMsgByTcpServer(&connMsg);
	return 0;
};

int GamesvrMsgProcesser::SendActorLogoutRsp(u64 connId, u64 id, int result)
{
	ConnMsg connMsg;
	connMsg.connId = connId;
	UniverseMsg *msgData = &connMsg.msg;
	msgData->mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_LOGOUT_RSP);
	msgData->mutable_msgbody()->mutable_logoutrsp()->set_reserve(0);

	SendMsgByTcpServer(&connMsg);
	return 0;
};

int GamesvrMsgProcesser::SendActorFullDataRsp(u64 connId, Actor *actor)
{
	ConnMsg connMsg;
	connMsg.connId = connId;
	UniverseMsg *msgData = &connMsg.msg;
	msgData->mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_GET_FULL_DATA_RSP);
	msgData->mutable_msgbody()->mutable_getfulldatarsp()->set_id(actor->GetId());
	msgData->mutable_msgbody()->mutable_getfulldatarsp()->set_name(actor->GetName());

	SendMsgByTcpServer(&connMsg);
	return 0;
};

int GamesvrMsgProcesser::SendActorSetPosRsp(u64 connId, u64 id, Pos *pos)
{
	massert_noeffect(0); //not implement
	return 0;
};

int GamesvrMsgProcesser::SendMsgByTcpServer(ConnMsg *msg)
{
	massert_retval(msg != NULL, ERR_INVALID_PARAM);
	TcpServer *tcpServer = TcpServer::GetInstance();
	massert_retval(tcpServer != NULL, -1);
	int ret = tcpServer->PushClientMsg(msg);
	//printf("eoch msg back result:%d\n", ret);

	return ret;
}
