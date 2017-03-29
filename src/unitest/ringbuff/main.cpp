#include "base/ring_buffer.h"
#include "common/errcode.h"

#include <stdio.h>
using namespace universe;


int main(){
	int ret = 0;
	RingBuffer buff(1024);

	char data[] = {"abaabbeeddeeea;"};

	ret = buff.write(data, sizeof(data));
	if(ret != 0){
		printf("write failed %d\n", ret);
		return ret;
	}

	char rdata[1024];
	size_t len =  sizeof(rdata);
	ret = buff.read(rdata, len);
	if(ret != 0){
		printf("read failed %d\n", ret);
		return ret;
	}	
	rdata[len] = '\0';

	printf("data: %s\n %s\n", data, rdata);

}