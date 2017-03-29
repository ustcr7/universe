#ifndef _UNIVERSE_NET_TCP_CLIENT_H_
#define _UNIVERSE_NET_TCP_CLIENT_H_

#include <stddef.h>

class TcpClient{
public:
	static TcpClient* GetInstance();
	int connect(const char *svr_ip, int port);
	int close();

	int sendMsg(const char *buff, size_t len);	
	int recvMsg(char *buff, size_t &len);  //non block
private:
	int clientFd;
};

#endif

