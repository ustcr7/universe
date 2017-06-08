#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_client.h"
#include "universe_cs.pb.h"
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

		int my_actor_rid = 12345678;
		char my_name[100] = "wcc";
		
		if(cmdStr.compare("send_reg_msg") == 0)
		{
			UniverseMsg msg;
			msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_REGISTE_REQ);
			msg.mutable_msgbody()->mutable_registereq()->set_id(my_actor_rid);
			msg.mutable_msgbody()->mutable_registereq()->set_name(my_name);
		    tcpClient->sendMsg(&msg);
	    }
		if (cmdStr.compare("send_login_msg") == 0)
		{
			UniverseMsg msg;
			msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_LOGIN_REQ);
			msg.mutable_msgbody()->mutable_loginreq()->set_id(my_actor_rid);
			tcpClient->sendMsg(&msg);
		}
		if (cmdStr.compare("send_logout_msg") == 0)
		{
			UniverseMsg msg;
			msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_LOGOUT_REQ);
			msg.mutable_msgbody()->mutable_logoutreq()->set_id(my_actor_rid);
			tcpClient->sendMsg(&msg);
		}
		if (cmdStr.compare("send_getdata_msg") == 0)
		{
			UniverseMsg msg;
			msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_GET_FULL_DATA_REQ);
			msg.mutable_msgbody()->mutable_getfulldatareq()->set_id(my_actor_rid);
			tcpClient->sendMsg(&msg);
		}
		if (cmdStr.compare("send_move_msg") == 0)
		{
			UniverseMsg msg;
			msg.mutable_msghead()->set_actorid(my_actor_rid);
			msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_MOVE_REQ);
			ActorMoveReq *move_req = msg.mutable_msgbody()->mutable_movereq();
			PathNode *node = move_req->add_paths();

		    //斜线行走 有问题
			node->set_pos_x(100);
			node->set_pos_y(100);
			node = move_req->add_paths();
			node->set_pos_x(200);
			node->set_pos_y(600);
			node = move_req->add_paths();
			//直线行走 OK
			/*
			PathNode *node = move_req->add_paths();
			node->set_pos_x(300);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(340);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(500);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(530);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(1000);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(1000);
			node->set_pos_y(30);
			node = move_req->add_paths();
			node->set_pos_x(1000);
			node->set_pos_y(100);
			node = move_req->add_paths();
			node->set_pos_x(1000);
			node->set_pos_y(300);
			*/
			tcpClient->sendMsg(&msg);
		}
		if(cmdStr.compare("recv_msg") == 0)
		{
		    UniverseMsg msg;
		    ret = tcpClient->recvMsg(&msg);
			if(ret != 0)
			{
			    //printf("recv failed for %d\n", ret);
				continue;
			}
			switch (msg.msghead().msgid())
			{
			case UNIVERSE_MSG_ID_ACTOR_REGISTE_RSP:
			{
				printf("recv registe rsp ret:%d\n", msg.msgbody().registersp().result());
				break;
			}
			case UNIVERSE_MSG_ID_ACTOR_LOGIN_RSP:
			{
				printf("recv login rsp ret:%d\n", msg.msgbody().loginrsp().result());
				break;
			}
			case UNIVERSE_MSG_ID_ACTOR_LOGOUT_RSP:
			{
				printf("recv log out rsp\n");
				break;
			}
			case UNIVERSE_MSG_ID_ACTOR_GET_FULL_DATA_RSP:
			{
				printf("recv get full data rsp, id:%llu, pos_x:%d, pos_y:%d\n"
					, (u64)msg.msgbody().getfulldatarsp().id()
					, msg.msgbody().getfulldatarsp().pos().x()
					, msg.msgbody().getfulldatarsp().pos().y());
				break;
			}
			case UNIVERSE_MSG_ID_ACTOR_MOVE_RSP:
			{
				printf("recv move resp\n");
			}
			default :
			{
				printf("invalid recv msgid:%d\n", msg.msghead().msgid());
				break;
			}
			}
			printf("recv server msg id:%d\n", msg.msghead().msgid());
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

