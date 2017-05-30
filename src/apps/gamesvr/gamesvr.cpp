#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_server.h"
#include "../../proto/universe_cs.pb.h"
#include "gamesvr_msg_processer.h"
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
u64 idleCnt = 0;      //服务器空转次数,超过一定值会让服务器sleep一会

//接受socket数据必须放在一个独立线程里否则会阻塞主逻辑线程
//pop消息时注意加锁即可
void* NetIoThreadfunc(void *arg)
{
	while (workOn)
	{
		TcpServer *tcpServer = TcpServer::GetInstance();
		tcpServer->StartWork();

		sleep(1);
	}
	return NULL;
}
static pthread_mutex_t lockMutex;

int main()
{
	int ret = 0;
	printf("gamesvr start...\n");
	idleCnt = 0;
	pthread_mutex_init(&lockMutex, NULL);

	TcpServer *tcpServer = TcpServer::GetInstance();
	ret = tcpServer->Init("127.0.0.1", 6789, &lockMutex);
	massert_retval(ret == 0, ret);
	printf("tcp server init success\n");

	//创建IO线程
	printf("create thread begin\n");
	pthread_t thread;
	if (pthread_create(&thread, NULL, NetIoThreadfunc, NULL) != 0)
	{
		printf("create thread failed!\n");
		return -1;
	}
	printf("create thread finish\n");

	GamesvrMsgProcesser *msgProcesser = GamesvrMsgProcesser::GetSingleInstance();
	massert_retval(msgProcesser != NULL, ERR_UNKNOWN);

	static ConnMsg msgs[16];
	int msgCount = 0;
	while (workOn)
	{
		//server internal logics

		//client logics
		msgCount = (int)sizeof(msgs) / sizeof(msgs[0]);
		ret = tcpServer->PopClientMsg(msgs, msgCount); //一次最多处理20个消息
		if (ret < 0) //没读到数据
		{
			idleCnt++;
			if (idleCnt > 100)
			{
				usleep(10000);
				idleCnt = 0;
			}
			continue;
		}

		for (int i = 0; i < msgCount; ++i)
		{
			msgProcesser->RecvActorReq(msgs[i].connId, &msgs[i].msg);
		}

	}

	return 0;
}


