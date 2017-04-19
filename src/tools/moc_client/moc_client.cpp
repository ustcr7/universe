#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_client.h"
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
#include <iostream>
#include <stddef.h>
#include <cstddef>


int main()
{
    int ret = 0;

    TcpClient *tcpClient = TcpClient::GetInstance();
	ret = tcpClient->connect("127.0.0.1", 6789);
	massert_retval(ret == 0, ret);

    //char buff[1024];
	//size_t buff_len = 0;
	
    while(true)
   	{
      	printf("please input command\n");
   	    char cmd[1024];		
		std::cin.getline(cmd, 1024);

		std::string cmdStr(cmd);

		
		if(cmdStr.compare("send_msg") == 0)
		{
		   UniverseLoginMsg msg;
		   msg.setMsgId(1000);
		   msg.setActorId(100000001UL);
		   msg.setPasswd("123456");

		   tcpClient->sendMsg(&msg);
	    }
		if(cmdStr.compare("recv_msg") == 0)
		{
		    UniverseLoginMsg msg;
		    ret = tcpClient->recvMsg(&msg);
			if(ret != 0)
			{
			    //printf("recv failed for %d\n", ret);
				continue;
			}
			printf("recv server msg id:%d, actor id:%llu, passwd :%s\n"
				, msg.getMsgId(), msg.getActorId(), msg.getPasswd());
		}
		if(cmdStr.compare("stop")==0)
		{
		    tcpClient->close();
			break;
		}
		else
		{
		    printf("ivnalid cmd:%s\n", cmd);
		}
   	}

	return 0;
}

