#include<stdio.h>  
#include<stddef.h>
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include <cstring>
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <unistd.h>
#include <fcntl.h>

#include "tcp_client.h"
#include "errcode.h"
#include "massert.h"


using namespace universe;


int clientFd;
static const size_t MAX_BUFF_SIZE = 1024*100;
char send_buff[MAX_BUFF_SIZE];
char recv_buff[MAX_BUFF_SIZE];

struct sockaddr_in svr_addr;
TcpClient *gsTcpClient = NULL;

TcpClient* TcpClient::GetInstance()
{
   if( NULL == gsTcpClient )
   {
       gsTcpClient = new TcpClient();
	   massert_retval(gsTcpClient != NULL, NULL);
   }
   return gsTcpClient;
}

int  TcpClient::connect(const char *svr_ip, int port){
	int ret = 0;
	clientFd = socket(AF_INET, SOCK_STREAM, 0);
	massert_retval(clientFd > 0, -1);

	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(port);

	if(inet_pton(AF_INET, svr_ip, &svr_addr.sin_addr) <= 0 )
	{
		printf("pton failed\n");
		return -1;
	}

	if(::connect(clientFd, (struct sockaddr*)&svr_addr, sizeof(svr_addr)) < 0)
	{
		printf("connect failed\n");
		return -1;
	}

	int flags = fcntl(clientFd, F_GETFL, 0); 
    ret = fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);
    if(ret != 0)
	{
    	printf("set socket non block failed\n");
    	return -1;
    }

	printf("tcp client connect to<%s,%d> success\n", svr_ip, port);

	return 0;
}

int TcpClient::close(){
	::close(clientFd);	
	return 0;
}

int TcpClient::sendMsg(UniverseMsg *msg){
	int msg_byte_len = msg->ByteSize();

	int data_len = msg->ByteSize() + sizeof(msg_byte_len);
	char data[1024];	
	
	memcpy(data, &msg_byte_len, sizeof(msg_byte_len));

	msg->SerializeToArray(data+sizeof(msg_byte_len), msg_byte_len);
	
	int write_len = write(clientFd, data, data_len);
	if(write_len != data_len)
	{
		printf("write failed %d, %d\n", (int)write_len, write_len);
	    return -1;
	}
	//printf("write success len %d, msg byte size:%d\n", write_len, msg_byte_len);
	return 0;
}

int TcpClient::recvMsg(UniverseMsg *msg){
	static char data[100*1024];
	size_t data_len = sizeof(data)/sizeof(data[0]);

	int read_len = read(clientFd, data, data_len);
	
	//printf("read_len %d\n", read_len);
	if(read_len < 0)
	{
		//printf("recv msg failed ,err:%s\n", strerror(errno));
		return ERR_NO_DATA;
	}
	if(0 == read_len)
	{
		printf("%d closed\n", clientFd);
		close();
		return 0;
	}

	int msg_byte_len = *(int*)data;

	msg->ParseFromArray(data+sizeof(msg_byte_len), data_len - sizeof(msg_byte_len));   //WCC_TODO 也应该用一个ringbuffer否则可能出现一个包在多次read结果里
	
	return 0;
}

