#ifndef __UNIVERSE_CHAT_H__
#define __UNIVERSE_CHAT_H__

#include "../../common/base_type.h"

static const int MAX_CHAT_CONTENT_SIZE = 512;
static const int MAX_CHAT_DST_COUNT = 32;
class ChatContent
{
	const char content[MAX_CHAT_CONTENT_SIZE];
	int contentSize;
};

enum CHAT_SRC_TYPE
{
	CHAT_SRC_SYSTEM   = 1, //ϵͳ���͵�������Ϣ
	CHAT_SRC_PERSONAL = 2, //���˷��͵�������Ϣ
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

// a --> b˽��
// a --> b,c,d ���˹㲥
// a --> ������ ȫ���㲥
// ϵͳ
class Actor;
class UvChatMgr
{
public:
	int SendA2AChatMsg(Actor src_actor, Actor *dst_actor, int dst_actor_cnt, const char *content, int content_len); //actor to actor
	int SendS2AChatMsg(Actor *dst_actor, int dst_actor_cnt, const char *content, int content_len);                  //sytem to actor
	int BroadcastChatMsg(const char *content, int content_len);
};

#endif // !__UNIVERSE_CHAT_H__
