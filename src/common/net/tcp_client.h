#ifndef _UNIVERSE_NET_TCP_CLIENT_H_
#define _UNIVERSE_NET_TCP_CLIENT_H_

#include <stddef.h>
#include "../../proto/universe_cs.pb.h"
#include "../ring_buffer/ring_buffer.h"


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

