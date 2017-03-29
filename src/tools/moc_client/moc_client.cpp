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

    char buff[1024];
	size_t buff_len = 0;
	
    while(true)
   	{
      	printf("please input command\n");
   	    char cmd[1024];		
		std::cin.getline(cmd, 1024);

		std::string cmdStr(cmd);

		
		if(cmdStr.compare("send_msg") == 0)
		{
           strcpy(buff, "hello_server");
		   tcpClient->sendMsg(buff, strlen(buff)+1);
	    }
		if(cmdStr.compare("recv_msg") == 0)
		{
		    memset(buff, 0, sizeof(buff));
			buff_len = 1024; //sizeof(buff_len)/sizeof(buff_len[0]);
		    tcpClient->recvMsg(buff, buff_len);
			printf("recv msg:[%s]\n", buff);
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

