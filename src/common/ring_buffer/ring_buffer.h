#ifndef _UNIVERSE_RING_BUFFER_H_
#define _UNIVERSE_RING_BUFFER_H_

#include <stddef.h>

namespace universe{

const static int DEFAULT_RING_BUFF_SIZE = 1<<17;

//TODO: implement with list  ref:http://blog.codingnow.com/2012/02/ring_buffer.html

struct RingBuffer{
public:
	RingBuffer(size_t len = DEFAULT_RING_BUFF_SIZE);
	~RingBuffer();

	int write(const char *data, size_t len);
	
	int read(char *buff, size_t &len);

	int peek_int(int &value);
	int pop_int(int &valud);

	int size();
private:
	size_t _size;
	size_t _capacity;

	char *_data;

	int _head_idx;
	int _tail_idx;
};


}

#endif