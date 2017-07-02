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
#include <sys/epoll.h>
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

static int g_epollfd = 0;
static struct epoll_event events[EPOLLEVENTS];

int my_actor_rid = atoi(argv[2]);
char my_name[100];
TcpClient *tcpClient = NULL;

static char g_tmp_Str[1024];


static char *line_read = (char *)NULL;

void add_event(int epollfd, int fd, int state)
{
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}


void cb_linehandler(char *line)
{
	massert_retval(line != NULL, ERR_INVALID_PARAM);

	//char *line_no_white = strip_white(line); //WCC_TODO去空格

	static std::string last_cmd;
	std::string cur_cmd = line;

	do
	{
		if (cur_cmd.length() <= 0)
		{
			continue;
		}
		int ret = 0;
		//命令字符串解析
		ResultStr result[10];
		int result_cnt = sizeof(result) / sizeof(result[0]);
		int ret = SplitStr(line, " ", result, &result_cnt);
		massert_retval(ret == 0, ret);

		ClientReqHandle(result, result_cnt);

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

	const CommandInfo *cmd = CommandMgr::GetInstance()->GetFirstMatchCommand(text, len);
	if (cmd != NULL)
	{
		return cmd->GetCommandStr();
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

	const int fd = fileno(stdin);
	g_epollfd = epoll_create(10);
	add_event(g_epollfd, fd, EPOLLIN);

    int ret = 0;

    tcpClient = TcpClient::GetInstance();
	ret = tcpClient->connect("10.154.142.48", 6789);
	massert_retval(ret == 0, ret);


	for (;;) {
		usleep(100);

		int ret = 0;

		int event_num = epoll_wait(g_epollfd, events, EPOLLEVENTS, 0); //控制台输入 epoll监听
		if (event_num > 0)
		{
			rl_callback_read_char();
		}

		RecvServerMsg();       //网络数据不断地recv  //WCC_TODO:连续recv N次无数据后应该sleep一小段时间

	}// end for


	return 0;
}

