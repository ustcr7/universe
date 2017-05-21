#include <string>
#include <stdio.h>
#include <string.h>
#include "massert.h"
#include "universe_cs.pb.h"

int main(){
	
	UniverseMsg msg;
	msg.set_msgid(12345678);
	msg.set_name("wcc");

	char data[1024];
	int data_len = msg.ByteSize();
	if (!msg.SerializeToArray(data, data_len))
	{
		printf("serialize failed\n");
		return -1;
	}
	printf("serialize success datalen:%d\n", data_len);

	//deserizlize
	UniverseMsg new_msg;
	if (!new_msg.ParseFromArray(data, data_len))
	{
		printf("parse failed\n");
		return -1;
	}
	printf("deserialize success id:%d name:%s\n", new_msg.msgid(), new_msg.name().c_str());

	return 0;
}