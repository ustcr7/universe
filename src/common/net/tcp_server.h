#ifndef __UNIVERSE_NET_TCP_SERVER_H_
#define __UNIVERSE_NET_TCP_SERVER_H_

#include <vector>
#include <pthread.h> 
#include <sys/epoll.h>


//DEMO版本先不用ringbuffer了,假设一次读取能读完所有数据,然后放到一个vector里

//限定一个消息打包后最大100k
struct MsgBuffer
{
    int fd;
    char data[100*1024];
	size_t dataLen;
};

class TcpServer{
public:
	int Init(const char *ip, int port, pthread_mutex_t *mutex);
	bool HasClientMsg();
	int ClientMsgCount();
	int PopClientMsg(std::vector<MsgBuffer> &clientMsgArray);
	int PushClientMsg(const MsgBuffer *buffer);
	static TcpServer* GetInstance();
	int StartWork();
	int StopWork();

private:
	int GetMsgLock();
	int FreeMsgLock();
	int AcceptNewClient();
	int ReadDataFromClient(int clientFd);
	
private:
    //消息队列	
	std::vector<MsgBuffer> recvMsgArray;  //接收到的client消息

	//epoll相关数据
	int ip;
	int port;
	int epFd;      //epoll fd
	int listenFd;  //监听新连接的fd
	struct epoll_event epEvents[128];

	//控制相关数据
	bool workOn;                  //停止开关
	pthread_mutex_t *msgMutex;    //IO线程锁,避免读写消息产生互斥
};


#endif

