#include "massert.h"
#include "tcp_server.h"
#include <vector>
#include <assert.h>
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

static int MAX_LISTEN_FD_NUM = 128;

static TcpServer *gsTcpServer = NULL; //single instance

void setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	massert_retnone(opts >= 0);
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		massert_retnone(0);
	}
}

TcpServer* TcpServer::GetInstance()
{
    if(NULL == gsTcpServer)
    {
        gsTcpServer = new TcpServer();
		massert_retval(gsTcpServer != NULL, NULL)
    }
	return gsTcpServer;
}

//初始化
int TcpServer::Init(const char *ip, int port, pthread_mutex_t *mutex)
{
    int ret = 0;
	massert_retval(ip != NULL && mutex != NULL, -1);

	//create listen epoll evt
    struct epoll_event ev;
	epFd = epoll_create(MAX_LISTEN_FD_NUM); //WCC_TODO check error
	massert_retval(epFd >= 0, -1);

	//set listen socket	
    struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));	
    serveraddr.sin_family = AF_INET;
    if( 0 == inet_aton(ip, &(serveraddr.sin_addr)) )
    {
       massert_retval(0, -1);
    }
    serveraddr.sin_port = htons(port);
	printf("bind ip:%s, port:%d\n", ip, port);
	fflush(NULL);

	listenFd = socket(AF_INET, SOCK_STREAM, 0);
	massert_retval( listenFd >= 0, -1);
	setnonblocking(listenFd);
	
    ret = bind(listenFd, (sockaddr *)&serveraddr, sizeof(serveraddr));
	if(ret != 0)
	{
	    printf("bind failed, errno:%d(%s)", errno, strerror(errno));
		massert_retval(0, -1);
	}
	
	ret = listen(listenFd, 10);
	massert_retval(ret == 0, ret);

	ev.data.fd = listenFd;
    ev.events = EPOLLIN | EPOLLET;
	ret = epoll_ctl(epFd, EPOLL_CTL_ADD, listenFd, &ev);
	massert_retval( ret >= 0, ret);

	workOn = true;
	msgMutex = mutex;

	printf("init tcp server success\n");

	return 0;
}

//判断是否有客户端消息等待读取
bool TcpServer::HasClientMsg()
{
    return recvMsgArray.size() > 0; //此处其实没必要加锁,如果没读取到,下次再读取即可
}

int TcpServer::ClientMsgCount()
{
	return recvMsgArray.size();
}

//读取客户端消息 //vector swap了,效率极低
int TcpServer::PopClientMsg(std::vector<MsgBuffer> &clientMsgArray)
{
    if( !HasClientMsg())
    {
        return -1;
	}
	GetMsgLock();
	clientMsgArray.swap(recvMsgArray);
	recvMsgArray.clear();
	printf("tcp server pop client msg success, size:%d\n", (int)clientMsgArray.size());
	FreeMsgLock();
	
	return 0;
}

int TcpServer::PushClientMsg(const MsgBuffer *buffer)
{
    massert_retval(buffer != NULL, -1);
    //WCC_TODO 先假定一次能写完吧
	int write_len = write(buffer->fd, buffer->data, buffer->dataLen);
	if(write_len != (int)buffer->dataLen)
	{
		printf("write failed buff_len:%d, write_len:%d, error:%s\n", (int)buffer->dataLen, write_len, strerror(errno));
		massert_retval(0, -1);
	}
	printf("write success len %d\n", write_len);

    return 0;
}

int TcpServer::StartWork()
{
    while(workOn)
    {
		int fdNum = epoll_wait(epFd, epEvents, 100, -1);
		if(fdNum > 0)
		{
		    printf("epoll wait get fd num:%d\n", fdNum);
		}
		for(int i = 0; i < fdNum; ++i)
		{
			if(epEvents[i].data.fd == listenFd) 			 // 新的连接
			{
				AcceptNewClient();
			}
			else if(epEvents[i].events & EPOLLIN) 	   // 有数据可读
			{
				ReadDataFromClient(epEvents[i].data.fd);
			}
			else if(epEvents[i].events & EPOLLOUT)
			{
			    printf("epoll out event , ignore\n");
       			//massert_noeffect(0);	//目前不考虑socket满了没法写入的情况,暂定写入总是OK的
			}
		}

	}
	
    return 0;	
}

int TcpServer::StopWork()
{
    workOn = true;
	return 0;
}

int TcpServer::GetMsgLock()
{
	pthread_mutex_lock(msgMutex); 
    return 0;
}

int TcpServer::FreeMsgLock()
{
	pthread_mutex_unlock(msgMutex);
	return 0;
}

int TcpServer::AcceptNewClient()
{
    printf("accept new client\n");
	struct sockaddr_in clientaddr;
	socklen_t clilen;

	struct epoll_event ev;
	int clientFd = accept(listenFd, (sockaddr *)&clientaddr, &clilen);
	if(clientFd < 0)
	{
		printf("invalid accept\n");
		exit(1);
	}
	setnonblocking(clientFd);
	
	char *str = inet_ntoa(clientaddr.sin_addr);
	ev.data.fd = clientFd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epFd, EPOLL_CTL_ADD, clientFd, &ev);

	return 0;
}

int TcpServer::ReadDataFromClient(int clientFd)
{
	printf("read data from client\n");

	MsgBuffer buffer;
	buffer.fd = clientFd;

	if ( clientFd < 0) return -1;
	if ( (buffer.dataLen = read(clientFd, buffer.data, 1024)) < 0) //WCC_TODO需要while true读取
	{
		if (errno == ECONNRESET)
		{
			close(clientFd);
		}
		else
		{
			massert_retval(0, -1);
		}
	}
	else if (buffer.dataLen == 0)
	{
		close(clientFd);
	}

    GetMsgLock();
	recvMsgArray.push_back(buffer);
	printf("current msg size:%d\n", (int)recvMsgArray.size());
	FreeMsgLock();

	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epFd, EPOLL_CTL_MOD, clientFd, &ev);

    return 0;
}


