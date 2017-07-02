#include "../../common/massert.h"
#include "../../common/errcode.h"
#include "../../common/base_type.h"
#include "../../common/net/tcp_client.h"
#include "../../common/util/str_util.h"
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
#include "universe_cs.pb.h"
extern int my_actor_rid;
extern char my_name[100];
extern TcpClient *tcpClient;

int ClientReqHandle(ResultStr *result, int result_cnt)
{
	if (strcmp(result[0].str, "send_reg_msg") == 0)
	{
		UniverseMsg msg;
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_REGISTE_REQ);
		msg.mutable_msgbody()->mutable_registereq()->set_id(my_actor_rid);
		msg.mutable_msgbody()->mutable_registereq()->set_name(my_name);
		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_login_msg") == 0)
	{
		UniverseMsg msg;
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_LOGIN_REQ);
		msg.mutable_msgbody()->mutable_loginreq()->set_id(my_actor_rid);
		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_logout_msg") == 0)
	{
		UniverseMsg msg;
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_LOGOUT_REQ);
		msg.mutable_msgbody()->mutable_logoutreq()->set_id(my_actor_rid);
		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_learnspell_msg") == 0)
	{
		if (result_cnt<2)
		{
			printf("Usage: send_learnspell_msg spellid\n");
			break;
		}
		UniverseMsg msg;
		msg.mutable_msghead()->set_actorid(my_actor_rid);
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_LEARN_SPELL_REQ);
		LearnSpellReq *req = msg.mutable_msgbody()->mutable_learnspellreq();
		req->set_spellid(atoi(result[1].str));

		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_castspell_msg") == 0)
	{
		if (result_cnt<3)
		{
			printf("Usage: send_castspell_msg spellid targetid\n");
			break;
		}
		UniverseMsg msg;
		msg.mutable_msghead()->set_actorid(my_actor_rid);
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_CAST_SPELL_REQ);
		CastSpellReq *req = msg.mutable_msgbody()->mutable_castspellreq();
		req->set_spellid(atoi(result[1].str));
		req->set_targetid(atoll(result[2].str));
		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_getdata_msg") == 0)
	{
		UniverseMsg msg;
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_GET_FULL_DATA_REQ);
		msg.mutable_msgbody()->mutable_getfulldatareq()->set_id(my_actor_rid);
		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_chat_msg") == 0)
	{
		int target_rid = atoi(result[1].str);
		char *content = result[2].str;

		UniverseMsg msg;
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_CHAT_REQ);
		ChatInfo *chat_info = msg.mutable_msgbody()->mutable_chatreq()->mutable_chatinfo();
		chat_info->set_type(CHAT_TYPE_PRIVATE);
		chat_info->set_content(content);
		chat_info->set_dstid(target_rid);
		tcpClient->sendMsg(&msg);
	}
	if (strcmp(result[0].str, "send_move_msg") == 0)
	{
		UniverseMsg msg;
		msg.mutable_msghead()->set_actorid(my_actor_rid);
		msg.mutable_msghead()->set_msgid(UNIVERSE_MSG_ID_ACTOR_MOVE_REQ);
		ActorMoveReq *move_req = msg.mutable_msgbody()->mutable_movereq();

		if (false)   //折线走
		{
			//(50,150) --> (200,100) --> (320,160) --> (0,0)
			PathNode *node = move_req->add_paths();
			node->set_pos_x(50);
			node->set_pos_y(150);
			node = move_req->add_paths();
			node->set_pos_x(200);
			node->set_pos_y(100);
			node = move_req->add_paths();
			node->set_pos_x(320);
			node->set_pos_y(160);
			node = move_req->add_paths();
			node->set_pos_x(0);
			node->set_pos_y(0);
		}
		if (false)   //往回走
		{
			//(300,0) --> (330,0) --> (330,100) --> (150,100)
			PathNode *node = move_req->add_paths();
			node->set_pos_x(300);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(330);
			node->set_pos_y(0);
			node = move_req->add_paths();
			node->set_pos_x(330);
			node->set_pos_y(100);
			node = move_req->add_paths();
			node->set_pos_x(150);
			node->set_pos_y(100);
		}

		//斜线行走
		if (false)
		{
			PathNode *node = move_req->add_paths();
			node->set_pos_x(100);
			node->set_pos_y(100);

			node = move_req->add_paths();
			node->set_pos_x(200);
			node->set_pos_y(600);
		}
		//直线行走 OK
		if (false)
		{
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
		}
		tcpClient->sendMsg(&msg);
	}
	return 0;
}


int RecvServerMsg()
{
	UniverseMsg msg;
	int ret = tcpClient->recvMsg(&msg);
	if (ret != 0)
	{
		//printf("recv failed for %d\n", ret);
		return -1;
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
		break;
	}
	case UNIVERSE_MSG_ID_FORWRARD_CHAT:
	{
		printf("recv forward char msg\n");
		const ChatInfo &chatinfo = msg.msgbody().forwardchatinfo().chatinfo();
		printf("actor:%d chat to you:%s\n", (int)chatinfo.dstid(), chatinfo.content().c_str());
		break;
	}
	default:
	{
		printf("invalid recv msgid:%d\n", msg.msghead().msgid());
		break;
	}
	}
	printf("recv server msg id:%d\n", msg.msghead().msgid());
	return 0;
}