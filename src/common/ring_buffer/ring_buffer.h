#ifndef _UNIVERSE_RING_BUFFER_H_
#define _UNIVERSE_RING_BUFFER_H_

#include <stddef.h>

const static int DEFAULT_RING_BUFF_SIZE = 1024*1024; // WCC_TODO机器内存足够时改成:100M

//TODO: implement with list  ref:http://blog.codingnow.com/2012/02/ring_buffer.html

//ring buffer返回的buff需要回绕时,进行一次内存拷贝,否则上层拿到的不是完整的buff数组
//更好的解决方案时,当压入数据时,如果尾部空间不足以放下整个消息时,则尾部的空间做个标记,然后从头开始



class RingBuffer{
public:
	RingBuffer(size_t len = DEFAULT_RING_BUFF_SIZE);
	~RingBuffer();

	int write(const char *data, size_t len);
	
	int read(char *buff, size_t &len);

	int getBufferLen();

	int peekInt(int &value);

	int size();
private:
	size_t _size;
	size_t _capacity;

	char *_data;

	int _head_idx;
	int _tail_idx;
};


#endif