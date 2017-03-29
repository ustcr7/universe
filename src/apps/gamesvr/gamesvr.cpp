#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_server.h"
#include <unistd.h>
#include <vector>
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

bool workOn = true;
u64 idleCnt = 0;      //��������ת����,����һ��ֵ���÷�����sleepһ��

void * NetIoThreadfunc(void *arg)
{
    while(workOn)
    {
	    TcpServer *tcpServer = TcpServer::GetInstance();
		tcpServer->StartWork();

		sleep(1);
    }
}

static pthread_mutex_t lockMutex;

int main()
{
    int ret = 0;
    printf("gamesvr start...\n");
    idleCnt = 0;
	pthread_mutex_init(&lockMutex,NULL);

    TcpServer *tcpServer = TcpServer::GetInstance();
	ret = tcpServer->Init("127.0.0.1", 6789, &lockMutex);
	massert_retval(ret == 0, ret);
	printf("tcp server init success\n");

    //����IO�߳�
    printf("create thread begin\n");
	pthread_t thread;
	if(pthread_create(&thread, NULL, NetIoThreadfunc, NULL)!=0)
    {
        printf("create thread failed!\n");
        return -1;
    }
	printf("create thread finish\n");

    static char buff[1024*1024];
	size_t buff_len;
	std::vector<MsgBuffer> msgs;
	while(workOn)
	{
	   if(!tcpServer->HasClientMsg())
	   	{
    	   	idleCnt++;
			if(idleCnt > 100)
			{
			    //printf("idle too long sleep\n");
			    usleep(100000);
				//printf("awake, begin work\n");
				idleCnt = 0;
			}
	   	    continue;
	   	}

	   printf("gamesvr tcp server curent msg size :%d\n", tcpServer->ClientMsgCount());
	   
	   int ret = 0;
       //GetMsgFrom TcpServer ע����Ҫ��֤��д�̵߳Ļ���
	   ret = tcpServer->PopClientMsg(msgs);
	   for(int i=0; i<msgs.size(); ++i)
	   {
	       printf("gamesvr recv client msg:[%s]\n", msgs.at(i).data); //Ǳ����:client������buff���Ǵ�'\0'��

		   MsgBuffer sendBuffer;
		   sendBuffer.fd = msgs.at(i).fd;
  		   strcpy(sendBuffer.data, "hello client");
		   sendBuffer.dataLen = 13;
		   tcpServer->PushClientMsg(&sendBuffer);

	       //WCC_TODO send msg
	   }
	   massert_continue(ret == 0);
	   //pop msgs and process it
	   msgs.clear();  
	
	}

	//WCC_TODO ��Ҫpthread_join ���߳�??

	return 0;
}


