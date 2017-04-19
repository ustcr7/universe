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
	//��RAII�����,����������̫���ײ�����������
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
	//epoll�������
	int ip;
	int port;
	int epFd;      //epoll fd
	int listenFd;  //���������ӵ�fd
	struct epoll_event epEvents[128];

	//�����������
	bool workOn;                  //ֹͣ����
	pthread_mutex_t *msgMutex;    //IO�߳���,�����д��Ϣ��������

	std::set<int> clientFdSet;      //�ͻ���

	std::map<int, u64> fdIdMap;    //fd     --> connid
	std::map<u64, int> idFdmap;    //connid --> fd
	
	std::map<int, ConnInfo> connMap;

    //ÿ��һ��socketд������(�����ݳ��� >= һ���������ĳ���ʱ,��fd����list��,��������list���ɰ�����)
    //ע��,��list�л�ȡfd��Ӧ��ringbufferҪ����һ���Զ���
    std::list<int> readFdList;

	TcpServerMsgHandle msgHandle;
};


#endif

