#ifndef __UNIVERSE_CS_PROTO_H__
#define __UNIVERSE_CS_PROTO_H__

#include "../common/massert.h"
#include "../common/base_type.h"


class UniverseMsgHead
{
public:
	int msgId;
	u64 actorId;
};


class UniverseMsg
{
public:
	//WCC_TODO:把这些放到UniverseMsgHead中
	int getMsgId() const;
	void setMsgId(int msgId);
	u64 getActorId() const;
	void setActorId(u64 actorIdd);



	virtual int deserialize(char *data, int data_len) = 0;
	virtual int serialize(char *data, size_t &data_len) const = 0;

private:
    UniverseMsgHead head;
};


//WCC_TODO 考虑大小端的问题
//WCC_TODO 看下开源实现里的序列化是如何实现的?
class UniverseLoginMsg : public UniverseMsg
{
public:
	int deserialize(char *data, int data_len);
	int serialize(char *data, size_t &data_len) const;

	int setPasswd(const char *passwd);
	char *getPasswd();
private:
	char passwd[128];
};


#endif
