#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_server.h"
#include "../../common/timer/uv_timer.h"
#include "../../proto/universe_cs.pb.h"
#include "gamesvr_msg_processer.h"
#include "instance_mgr.h"
#include <unistd.h>
#include <ctime>
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

//����socket���ݱ������һ�������߳��������������߼��߳�
//pop��Ϣʱע���������
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

int test_timer_call_back(const char *callback_data, u32 callback_data_len)
{
	int add_timer_time = *(int*)callback_data;
	int cur_time = (int)std::time(0);
	printf("timer out at:%d, add timer time:%d, interval:%d\n", cur_time, add_timer_time, cur_time-add_timer_time);
	return 0;
}

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

	//����IO�߳�
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

	//��ʼ����ͼ��Ϣ
	MapMgr *mapMgr = MapMgr::GetSingleInstance();
	massert_retval(mapMgr != NULL, ERR_BAD_ALLOC);
	mapMgr->InitAllMapInfo();

	//ʱ�������
	UvTimer *timer = UvTimer::GetSingleInstance();
	massert_retval(timer != NULL, ERR_BAD_ALLOC);
	timer->InitTimer();

	if(false)
	{
		int cur_time = (int)std::time(0);
		//test code  //1s  2s  5s 10s 20s ����һ����ʱ��
		timer->AddTimer(1*1000,  test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		timer->AddTimer(2 * 1000, test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		timer->AddTimer(5 * 1000, test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		timer->AddTimer(10 * 1000, test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		timer->AddTimer(20 * 1000, test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		timer->AddTimer(55 * 1000, test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		timer->AddTimer(123 * 1000, test_timer_call_back, (const char*)&cur_time, sizeof(cur_time));
		printf("add test timer success cur time:%d\n", cur_time);
	}

	static ConnMsg msgs[16];
	int msgCount = 0;
	while (workOn)
	{
		//server internal logics

		//client logics
		msgCount = (int)sizeof(msgs) / sizeof(msgs[0]);
		ret = tcpServer->PopClientMsg(msgs, msgCount); //һ����ദ��20����Ϣ
		if (msgCount <= 0) //û��������
		{
			idleCnt++;
			if (idleCnt > 100)
			{
				usleep(500); //˯�� ��λ:΢��
				idleCnt = 0;
			}
		}

		for (int i = 0; i < msgCount; ++i)
		{
			msgProcesser->RecvActorReq(msgs[i].connId, &msgs[i].msg);
		}

		timer->RunTimer();
	}

	return 0;
}


