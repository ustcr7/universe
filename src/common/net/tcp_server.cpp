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

static u64 globalConnId = 0;

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

	printf("init tcp server success, listenFd:%d\n", listenFd);

	return 0;
}

void TcpServer::Fini()
{
	std::map<int, ConnInfo>::iterator iter;
    for(iter = connMap.begin(); iter != connMap.end(); ++iter)
    {
        RingBuffer *ptr = iter->second.ringBuffer; //需要对iter->second解引用吗
        if(ptr != NULL)
        {
            delete ptr;
        }		
    }
	connMap.clear();
    

}

int TcpServer::PopClientMsg(ConnMsg *msgs, int &popCount)
{
	GetMsgLock();

	int maxPopCount = popCount;
	popCount = 0;

	for(std::list<int>::iterator iter = readFdList.begin();
	   iter != readFdList.end() && popCount < maxPopCount;)
	{
	    int clientFd = *iter;
		ConnInfo *connInfo = GetConnInfoByFd(clientFd);
		if(connInfo== NULL)
		{
		    FreeMsgLock();
			massert_retval(0, -1);
		}
		
	    RingBuffer *rb = connInfo->ringBuffer;
		if(rb == NULL)
		{
		    FreeMsgLock();
			massert_retval(0, -1);
		}

        //对单个rb是否也应该要数量限制能,否则有可能导致其他的fd等待时间很长
		while(popCount < maxPopCount)
		{
		    int msgLen = 0;			
			int ret = rb->peekInt(msgLen);
			if(ret != 0 || rb->getBufferLen() < msgLen)
			{
			    break;
			}
			printf("aaaa\n");
			static char data[10*1024];
			size_t data_len = msgLen + sizeof(msgLen); //第一个字节是整个消息体的长度N, 后面N个字节才是pb结构体打包后的字节
			ret = rb->read(data, data_len);
			if(ret < 0)
			{
		        FreeMsgLock();
			    massert_retval(0, -1);
		    }

			msgs[popCount].msg.ParseFromArray(data+ sizeof(msgLen), msgLen);
			msgs[popCount].connId = GetIdByfd(clientFd);
           
			++popCount;
						
			ret = rb->peekInt(msgLen);
			printf("bbbbb\n");
			data_len = msgLen + sizeof(msgLen);
			if(ret == 0 && rb->getBufferLen() >= (int)data_len)
			{
			    readFdList.push_back(clientFd);
			    break;
			}
		}

		iter = readFdList.erase(iter);
	}
	
	FreeMsgLock();
	
	return (popCount>0)?0:-1;
}

int TcpServer::PushClientMsg(ConnMsg *connMsg)
{
	u64 connId = connMsg->connId;
	const UniverseMsg *msg = &connMsg->msg;

    int fd = GetFdById(connId);;
	massert_retval(fd >= 0, -1);
	int msg_byte_len = msg->ByteSize();
	int data_len = msg->ByteSize() + msg_byte_len;
	char data[1024];
	memcpy(data, &msg_byte_len, sizeof(msg_byte_len));
	msg->SerializeToArray(data+sizeof(msg_byte_len), msg_byte_len);

    //WCC_TODO 先假定一次能写完吧
	int write_len = write(fd, data, data_len);
	if(write_len != (int)data_len)
	{
		printf("write failed buff_len:%d, write_len:%d, error:%s\n"
			, (int)data_len, write_len, strerror(errno));
		massert_retval(0, -1);
	}
	printf("write success len %d\n", write_len);

    return 0;
}

int TcpServer::BroadCastClientMsg(const UniverseMsg *msg)
{
   /* for(std::set<int>::iterator iter = clientFdSet.begin(); iter != clientFdSet.end(); ++iter)
    {
        PushClientMsg(msg);
    }*/
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
	socklen_t clilen = sizeof(clientaddr);

	struct epoll_event ev;
	int clientFd = accept(listenFd, (sockaddr *)&clientaddr, &clilen);
	if(clientFd < 0)
	{
		printf("invalid accept, client fd:%d, listen fd:%d, error:%s\n"
			, clientFd, listenFd, strerror(errno));
		exit(1);
	}
	setnonblocking(clientFd);
	
	//char *str = inet_ntoa(clientaddr.sin_addr);
	ev.data.fd = clientFd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epFd, EPOLL_CTL_ADD, clientFd, &ev);

	clientFdSet.insert(clientFd);

	u64 id = AllocConnInfoByFd(clientFd);
	massert_retval(id != 0, -1);

	return 0;
}

int TcpServer::CloseClient(int clientFd)
{
	clientFdSet.erase(clientFd);

	FreeConnInfoByFd(clientFd);

    return 0;
}

int TcpServer::ReadDataFromClient(int clientFd)
{
	printf("read data from client\n");

	static char buffer[100 * 1024];
	int buffer_len;

	if ( clientFd < 0){
		printf("invalid fd:%d\n", clientFd);
		return -1;
	}

	if ( (buffer_len = read(clientFd, buffer, 1024)) < 0) //WCC_TODO需要while true读取
	{
		if (errno == ECONNRESET)
		{
		    printf("conn reset, close it\n");
			CloseClient(clientFd);
		    return 0;
		}
		else
		{
			massert_retval(0, -1);
		}
	}
	else if (buffer_len == 0)
	{
	    printf("data len is 0, close it\n");
		CloseClient(clientFd);
		return 0;
	}

    printf("read buffer len:%d\n", (int)buffer_len);
    GetMsgLock();
	//push to ring buffer

	ConnInfo* connInfo = GetConnInfoByFd(clientFd);
	massert_retval(connInfo != NULL, -1);

	RingBuffer *ringBuffer = connInfo->ringBuffer;
	massert_retval(ringBuffer != NULL, -1);
	
	ringBuffer->write(buffer, buffer_len);
	
	struct epoll_event ev;
	ev.data.fd = clientFd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epFd, EPOLL_CTL_MOD, clientFd, &ev);

	int msgLen = 0;
	int ret = ringBuffer->peekInt(msgLen);
	printf("cccc\n");
	int total_len = msgLen + sizeof(msgLen);  //int(表示消息体字节数组) + 消息体字节数组
	massert_retval(ret == 0, -1);
	if((int)ringBuffer->getBufferLen() >= total_len)
	{
	    readFdList.push_back(clientFd);
	}

	FreeMsgLock();

    return 0;
}

int TcpServer::GetFdById(u64 actorId)
{
    std::map<u64 ,int>::iterator iter = idFdmap.find(actorId);
	if(iter == idFdmap.end())
	{
	    return -1;
	}
	else
	{
	    return iter->second;
	}
	return 0;
}

u64 TcpServer::GetIdByfd(int fd)
{
	std::map<int, u64>::iterator iter = fdIdMap.find(fd);
	if(iter == fdIdMap.end())
	{
		return -1;
	}
	else
	{
		return iter->second;
	}
	return 0;
}


ConnInfo* TcpServer::GetConnInfoByFd(int fd)
{
	std::map<int, u64>::iterator iditer = fdIdMap.find(fd);
	massert_retval(iditer != fdIdMap.end(), NULL);
	u64 connId = iditer->second;
		

    std::map<int, ConnInfo>::iterator iter = connMap.find(connId);
	if(iter == connMap.end())
	{
		return NULL;
	}

	return &iter->second;
}

u64 TcpServer::AllocConnInfoByFd(int fd)
{
    globalConnId++;
	ConnInfo connInfo;
	connInfo.fd = fd;
	
    std::map<int, ConnInfo>::iterator iter = connMap.find(fd);
	massert_retval(iter == connMap.end(), -1); //repeta alloc

	RingBuffer *rb = new RingBuffer(512);
	massert_retval(rb != NULL, -1);
	connInfo.ringBuffer = rb;

    connMap.insert(std::make_pair(globalConnId, connInfo));

	fdIdMap.insert(std::make_pair(fd, globalConnId));
	idFdmap.insert(std::make_pair(globalConnId, fd));

	return globalConnId;
}

int TcpServer::FreeConnInfoByFd(int fd)
{
	std::map<int, u64>::iterator iditer = fdIdMap.find(fd);
	massert_retval(iditer != fdIdMap.end(), -1);
    u64 connId = iditer->second;
	
    std::map<int, ConnInfo>::iterator iter = connMap.find(connId);
	massert_retval(iter != connMap.end(), -1);

	RingBuffer* rb = iter->second.ringBuffer;

	if(rb != NULL)
	{
	   delete rb;
	}

	fdIdMap.erase(fd);
	idFdmap.erase(connId);

	return 0;
}


