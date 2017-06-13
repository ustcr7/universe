#include "chat.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../actor.h"
#include "../actor_mgr.h"
#include "../gamesvr_msg_processer.h"
#include "./instance_mgr.h"


int UvChatMgr::ForwardChatMsg(ChatType chatType, Actor *src_actor, Actor *dst_actor, const char *content, int content_len)
{
	massert_retval(src_actor != NULL && dst_actor != NULL && content != NULL, ERR_INVALID_PARAM);

	GamesvrMsgProcesser *msgProc = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProc != NULL, ERR_BAD_ALLOC);
	int ret = msgProc->SendForwardChatInfo(dst_actor->GetConnId()
		, chatType
		, src_actor->GetId()
		, dst_actor->GetId()
		, content
		, content_len);
	massert_retval(ret == 0, ret);

	return 0;
}


int UvChatMgr::PersonalChatMsg(Actor *src_actor, Actor *dst_actor, const char *content, int content_len)
{
	ForwardChatMsg(CHAT_TYPE_PRIVATE, src_actor, dst_actor, content, content_len);

	return 0;
}

int UvChatMgr::AroundChatMsg(Actor *src_actor, const char *content, int content_len)
{
	InstanceMgr *instanceMgr = InstanceMgr::GetSingleInstance();

	Instance *instance = instanceMgr->GetInstanceById(src_actor->GetInstanceId());
	massert_retval(instance != NULL, ERR_INVALID_PARAM);

	Around *around = instance->GetAroundByPos(src_actor->GetPos());
	massert_retval(around != NULL, ERR_INVALID_PARAM);

	Actor* actorArr[1024];
	int actorCnt = sizeof(actorArr) / sizeof(actorArr[0]);
	int ret = around->DumpAllActors(actorArr, &actorCnt);
	massert_retval(ret == 0, ret);

	for (int i = 0; i < actorCnt; ++i)
	{
		ForwardChatMsg(CHAT_TYPE_AROUND, src_actor, actorArr[i], content, content_len);
	}
}

//WCC_TODO:所有玩家在一个函数中发送,待优化
int UvChatMgr::BroadcastChatMsg(Actor *src_actor, const char *content, int content_len)
{
	Actor* actorArr[1024];
	int actorCnt = sizeof(actorArr) / sizeof(actorArr[0]);

	ActorMgr *actorMgr = ActorMgr::GetSingleInstance();
	int ret = actorMgr->DumpAllActors(actorArr, &actorCnt);
	massert_retval(ret == 0, ret);

	for (int i = 0; i < actorCnt; ++i)
	{
		ForwardChatMsg(CHAT_TYPE_BROADCAST, src_actor, actorArr[i], content, content_len);
	}

	return 0;
}