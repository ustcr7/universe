#include <stdio.h>
#include "ring_buffer/ring_buffer.h"
#include "errcode.h"
#include "massert.h"

#include <stdio.h>
using namespace universe;

void writeData(RingBuffer *buffer, int len)
{
	char data[1024] = {0};
	for(int i=0; i<len-1; ++i)
	{
	    data[i] = 'a';
	}
	int ret = buffer->write(data, len);
	massert_noeffect(ret == 0);
}

void readData(RingBuffer *buffer, char *data, int len)
{
    size_t readLen = len;
    buffer->read(data, readLen);
	massert_retnone((int)readLen == len);
}

int main(){
	char data[256];
	RingBuffer buffer(60);

	int len = 30;
    writeData(&buffer, len);
	readData(&buffer, data, len);
	printf("data: %s\n", data);

	len = 15;
    writeData(&buffer, len);
	readData(&buffer, data, len);
	printf("data: %s\n", data);

	len = 5;
    writeData(&buffer, len);
	readData(&buffer, data, len);
	printf("data: %s\n", data);

	len = 11;
    writeData(&buffer, len);
	readData(&buffer, data, len);
	printf("data: %s\n", data);

	len = 5;
    writeData(&buffer, len);
	readData(&buffer, data, len);
	printf("data: %s\n", data);

	return 0;
}