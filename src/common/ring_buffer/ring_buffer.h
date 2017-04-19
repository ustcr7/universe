#ifndef _UNIVERSE_RING_BUFFER_H_
#define _UNIVERSE_RING_BUFFER_H_

#include <stddef.h>

const static int DEFAULT_RING_BUFF_SIZE = 1024*1024; // WCC_TODO�����ڴ��㹻ʱ�ĳ�:100M

//TODO: implement with list  ref:http://blog.codingnow.com/2012/02/ring_buffer.html

//ring buffer���ص�buff��Ҫ����ʱ,����һ���ڴ濽��,�����ϲ��õ��Ĳ���������buff����
//���õĽ������ʱ,��ѹ������ʱ,���β���ռ䲻���Է���������Ϣʱ,��β���Ŀռ��������,Ȼ���ͷ��ʼ



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