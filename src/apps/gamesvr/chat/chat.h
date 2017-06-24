#ifndef __UNIVERSE_CHAT_H__
#define __UNIVERSE_CHAT_H__

#include "../../common/base_type.h"
#include "universe_cs.pb.h"

static const int MAX_CHAT_CONTENT_SIZE = 512;
static const int MAX_CHAT_DST_COUNT = 32;
class ChatContent
{
	char content[MAX_CHAT_CONTENT_SIZE];
	int contentSize;
};

enum CHAT_SRC_TYPE
{
	CHAT_SRC_SYSTEM   = 1, //系统发送的聊天信息
	CHAT_SRC_PERSONAL = 2, //个人发送的聊天信息
};
class ChatSrc
{
	CHAT_SRC_TYPE srcType;
	u64           srcId;
};

enum CHAT_DST_TYPE
{
	CHAT_DST_PERSONAL  = 1,
	CHAT_DST_BROADCAST = 2,
};

class ChatDst
{
	CHAT_DST_TYPE dstType;
	u64 dstIds[MAX_CHAT_DST_COUNT];
	int dstIdCnt;
};

// a --> b私聊
// a --> b,c,d 多人广播
// a --> 所有人 全服广播
// 系统
class Actor;
class UvChatMgr
{
public:
	int PersonalChatMsg(Actor *src_actor, Actor *dst_actor, const char *content, int content_len);  //私聊
	int AroundChatMsg(Actor *src_actor, const char *content, int content_len);   //附近聊天
	int BroadcastChatMsg(Actor *src_actor, const char *content, int content_len);                                     //全服聊天

private:
	int ForwardChatMsg(ChatType chatType, Actor *src_actor, Actor *dst_actor, const char *content, int content_len);
};

#endif // !__UNIVERSE_CHAT_H__
