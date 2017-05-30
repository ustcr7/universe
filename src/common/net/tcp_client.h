#ifndef _UNIVERSE_NET_TCP_CLIENT_H_
#define _UNIVERSE_NET_TCP_CLIENT_H_

#include <stddef.h>
#include "../../proto/universe_cs.pb.h"
#include "../ring_buffer/ring_buffer.h"

/*
* 注意:protobuf数据包中并没有直接保护字节流的长度
* 所以发送消息前必须在字节流前面加上一个int表示字节流长度 eg:http://blog.csdn.net/Solstice/article/details/6172391
*/

class TcpClient{
public:
	static TcpClient* GetInstance();
	int connect(const char *svr_ip, int port);
	int close();

	int sendMsg(UniverseMsg *msg);
	int recvMsg(UniverseMsg *msg);  //non block
private:
	int clientFd;
};

#endif

