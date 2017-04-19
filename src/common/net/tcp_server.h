#ifndef __UNIVERSE_NET_TCP_SERVER_H_
#define __UNIVERSE_NET_TCP_SERVER_H_
#include <map>
#include <vector>
#include <set>
#include <list>
#include <pthread.h> 
#include <sys/epoll.h>
#include "../../proto/universe_cs_proto.h"
#include "../ring_buffer/ring_buffer.h"

typedef int (*TcpServerMsgHandle)(int fd, const UniverseMsg *msg);

struct ConnMsg
{
    u64 connId;
	UniverseLoginMsg msg;
};

struct ConnInfo
{
    int fd;
    RingBuffer *ringBuffer;
};

class TcpServer{
public:
	static TcpServer* GetInstance();
	
	int Init(const char *ip, int port, pthread_mutex_t *mutex);
	void Fini();
	
	int PopClientMsg(ConnMsg *msgs, int &popCount);
	int PushClientMsg(ConnMsg *connMsg);
	int BroadCastClientMsg(const UniverseMsg *msg);

	//int SetFdIdPair(int fd, u64 id);
	
	int StartWork();
	int StopWork();

private:
	//用RAII代替吧,否则函数返回太容易产生死锁锁了
	int GetMsgLock();
	int FreeMsgLock();
	int AcceptNewClient();
	int CloseClient(int clientFd);
	int ReadDataFromClient(int clientFd);

	int GetFdById(u64 actorId);
	u64 GetIdByfd(int fd);
	ConnInfo* GetConnInfoByFd(int fd);
	u64 AllocConnInfoByFd(int fd);	
	int FreeConnInfoByFd(int fd);
		

private:
	//epoll相关数据
	int ip;
	int port;
	int epFd;      //epoll fd
	int listenFd;  //监听新连接的fd
	struct epoll_event epEvents[128];

	//控制相关数据
	bool workOn;                  //停止开关
	pthread_mutex_t *msgMutex;    //IO线程锁,避免读写消息产生互斥

	std::set<int> clientFdSet;      //客户端

	std::map<int, u64> fdIdMap;    //fd     --> connid
	std::map<u64, int> idFdmap;    //connid --> fd
	
	std::map<int, ConnInfo> connMap;

    //每次一个socket写入数据(且数据长度 >= 一个完整包的长度时,把fd存入list中,后续遍历list即可按序处理)
    //注意,从list中获取fd对应的ringbuffer要尝试一次性读完
    std::list<int> readFdList;

	TcpServerMsgHandle msgHandle;
};


#endif

