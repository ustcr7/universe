#include <string>
#include <stdio.h>
#include <string.h>
#include "massert.h"
#include "universe_cs_proto.h"

int main(){
	static char data[1024];
	memset(data, 0, sizeof(data));
	size_t data_len = sizeof(data)/sizeof(data[0]);
	
	{
		//pack msg
    	UniverseLoginMsg loginMsg;
		loginMsg.setMsgId(10000);
		loginMsg.setActorId(100000001UL);
		loginMsg.setPasswd("123456");
		loginMsg.serialize(data, data_len);
	}

	
   	{
		//unpack msg
		UniverseLoginMsg msg;
		msg.deserialize(data, data_len);
		printf("msg id:%d, actor_id:%llu, passwd:%s\n"
			, msg.getMsgId(), msg.getActorId(), msg.getPasswd());
	}

	return 0;
}
