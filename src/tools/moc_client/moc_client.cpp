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
#include "command_mgr.h"
#include <readline/readline.h>
#include <readline/history.h>


int my_actor_rid = atoi(argv[2]);
char my_name[100];

static char g_tmp_Str[1024];


static char *line_read = (char *)NULL;

void cb_linehandler(char *line)
{

	massert_retval(line != NULL, ERR_INVALID_PARAM);

	//char *line_no_white = strip_white(line); //WCC_TODOÈ¥¿Õ¸ñ

	static std::string last_cmd;
	std::string cur_cmd = line;

	do
	{
		if (cur_cmd.length() <= 0)
		{
			continue;
		}
		int ret = 0;
		//ÃüÁî×Ö·û´®½âÎö
		ResultStr result[10];
		int result_cnt = sizeof(result) / sizeof(result[0]);
		int ret = SplitStr(line, " ", result, &result_cnt);
		massert_retval(ret == 0, ret);

		ClientReqHandle(result);

	} while (0);
	if (cur_cmd.compare(last_cmd) != 0)
	{
		add_history(line_no_white);
		last_cmd = cur_cmd;
	}

	if (signal(SIGINT, client_stop) == SIG_ERR) {
		printf("signal SIGINT error!\n");
		return;
	}
}

const char * command_generator(const char *text, int state)
{
	static int list_index, len;

	if (!state)
	{
		list_index = 0;
		len = strlen(text);
	}

	/* Return the next name which partially matches from the command list. */
	int cmd_num = g_commands.size();
	while (list_index < cmd_num)
	{
		const CommandInfo *cmd = CommandMgr::GetInstance()->GetFirstMatchCommand(text, len);
		if (cmd != NULL)
		{
			return cmd->GetCommandStr();
		}
		list_index++;
	}
	return ((char *)NULL);
}

char ** moc_client_completion(const char *text, int start, int end)
{
	char **matches;

	matches = (char **)NULL;

	if (start == 0)
		matches = rl_completion_matches(text, command_generator);

	return (matches);
}


void initialize_readline()
{
	stifle_history(100);
	read_history("simu_history");
	using_history( );
	rl_callback_handler_install("cmd>>", linehandler);

	rl_attempted_completion_function = moc_client_completion;

	return;
}

int main(int argc, char **argv)
{	if (argc < 3)
	{
		printf("usage ./moc_client name rid\n");
		return - 1;
	}

	strncpy(my_name, argv[1], sizeof(my_name));
	printf("moc client start name:%s rid:%d\n", my_name, my_actor_rid);

    int ret = 0;

    TcpClient *tcpClient = TcpClient::GetInstance();
	ret = tcpClient->connect("10.154.142.48", 6789);
	massert_retval(ret == 0, ret);

    while(true)
   	{
	    //client req
      	printf("please input command\n");
   	    char cmd[1024];		
		std::cin.getline(cmd, 1024);

		

		//server msg rsp
		if (strcmp(result[0].str, "recv_msg") == 0)
		{
			UniverseMsg msg;
			ret = tcpClient->recvMsg(&msg);
			if (ret != 0)
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
		}
		if (strcmp(result[0].str, "stop") == 0)
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

