#ifndef __UNIVERSE_NET_TCP_SERVER_H_
#define __UNIVERSE_NET_TCP_SERVER_H_

#include <vector>
#include <pthread.h> 
#include <sys/epoll.h>


//DEMO�汾�Ȳ���ringbuffer��,����һ�ζ�ȡ�ܶ�����������,Ȼ��ŵ�һ��vector��

//�޶�һ����Ϣ��������100k
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
    //��Ϣ����	
	std::vector<MsgBuffer> recvMsgArray;  //���յ���client��Ϣ

	//epoll�������
	int ip;
	int port;
	int epFd;      //epoll fd
	int listenFd;  //���������ӵ�fd
	struct epoll_event epEvents[128];

	//�����������
	bool workOn;                  //ֹͣ����
	pthread_mutex_t *msgMutex;    //IO�߳���,�����д��Ϣ��������
};


#endif

