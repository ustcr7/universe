#include "universe_cs_proto.h"
#include <cstring>


int UniverseMsg::getMsgId() const 
{
	return head.msgId;
}

void UniverseMsg::setMsgId(int msgId)
{
    head.msgId = msgId;
}

u64 UniverseMsg::getActorId() const
{
	return head.actorId;
}

void UniverseMsg::setActorId(u64 actorId)
{
    head.actorId = actorId;
}

int UniverseLoginMsg::deserialize(char *data, int data_len)
{
    massert_retval(data_len >= (int)sizeof(int), -1);
	
    int *lenPtr = (int*)data;
	if((int)data_len != *lenPtr)
	{
	    printf("data_len:%d, msg_len:%d\n", data_len, *lenPtr);
	    massert_retval(0, -1);
	}
	massert_retval( *lenPtr == (int)data_len, -1);
	int *msgIdPtr = (int*)(data + sizeof(int));
	setMsgId(*msgIdPtr);
	u64 *actorIdPtr = (u64*) (data + sizeof(int) + sizeof(int));
	setActorId(*actorIdPtr);

	char *passwdPtr = data + sizeof(int) + sizeof(int) + sizeof(u64);

    memset(passwd, 0 , sizeof(passwd));
	strncpy(passwd, passwdPtr, sizeof(passwd));

	//printf("deserialize success, passwd:%s(%s), total_data_len:%d\n", passwd, passwdPtr, int(data_len));
	
    return 0;
}

int UniverseLoginMsg::serialize(char *data, size_t &data_len) const
{
    massert_retval( data != NULL && data_len > 0, -1);

    char *msg_len_ptr = data;	
    size_t max_data_len = data_len;
	data_len = 0;

    char *ptr = data;
	data_len += sizeof(int);
    massert_retval(data_len <= max_data_len, -1);
	*(int*)ptr = 0;
	ptr += sizeof(int);

	data_len += sizeof(int);
	massert_retval(data_len <= max_data_len, -1);
	*(int*)ptr = getMsgId();
	ptr += sizeof(int);

	data_len += sizeof(u64);
	massert_retval(data_len <= max_data_len, -1);
	*(u64*)ptr = getActorId();
	ptr += sizeof(u64);

    data_len += (1+strlen(passwd));
	massert_retval(data_len <= max_data_len, -1);
	strncpy(ptr, passwd, sizeof(passwd));
	*(ptr+strlen(passwd)) = 0;

	*(int*)msg_len_ptr = data_len;
	//printf("serialize success data len:%d, passwd:<%s,%s>\n", (int)data_len, passwd, ptr);
	//4+4+8+(6+1) = 23
	
    return 0;
}

int UniverseLoginMsg::setPasswd(const char *str)
{
    memset(passwd, 0, sizeof(passwd));
    strncpy(passwd, str, sizeof(passwd));
	//printf("set passwd :%s success\n", passwd);
	return 0;
}

char *UniverseLoginMsg::getPasswd()
{
    return passwd;
}

