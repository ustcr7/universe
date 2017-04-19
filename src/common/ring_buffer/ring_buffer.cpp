#include "errcode.h"
#include "massert.h"
#include "ring_buffer.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace universe;

RingBuffer::RingBuffer(size_t len){
	if(len <= 0){
		len = DEFAULT_RING_BUFF_SIZE;
	}

	_size = 0;
	_capacity = len;

	_data= new char[len];
	_head_idx = 0;
	_tail_idx = 0;
}

RingBuffer::~RingBuffer(){
	_size = 0;
	_capacity = 0;
	_head_idx = 0;
	_tail_idx = 0;
	if(_data != NULL){
		delete [] _data;
	}
}


//_head_idx第一个可写位置(初始为0)
//_tail_idx第一个可读位置(初始为0)
int RingBuffer::write(const char *data, size_t len){
	if(len <= 0){
		return ERR_INVALID_PARAM; 
	}
	if(len > _capacity - _size){
		printf("len %zu, remain size %zu\n", len, _capacity - _size);
	}

	if(_head_idx < _tail_idx
		|| _capacity - _head_idx >= len)
	{
	    memcpy(_data+_head_idx, data, len);
		_head_idx = _head_idx + len;
		_size += len;
	}
	else
	{
		size_t first_seg_len = _capacity - _head_idx;
		size_t second_seg_len = (len - (_capacity - _head_idx));
		memcpy(_data+_head_idx, data, first_seg_len);
		_head_idx = 0;
	    memcpy(_data, data+first_seg_len, second_seg_len);
		_head_idx += second_seg_len;
		_size += len;
	}

	printf("wite len:%d, bufer cur size:%d, <head_idx:%d, tail_idx:%d>\n"
		, (int)len, (int)_size, _head_idx, _tail_idx);

	return 0;
}

//size为0时,_tail_idx = head_idx
int RingBuffer::read(char *buff, size_t &len){
    massert_retval(buff != NULL, -1);
	if(len == 0 || _size == 0)
    {
		len = 0;
		return -1;
	}
	if(len > _size)
	{
		len = _size;
	}

	if(_tail_idx < _head_idx
		|| _capacity - _tail_idx >= len) //不需要回绕,直接读取
	{
	    memcpy(buff, _data + _tail_idx, len);
		_tail_idx += len;
		if(_tail_idx >= (int)_capacity)
		{
		    _tail_idx = 0;
		}

		_size -= len;
	}
	else
	{
	    int first_seg_len = _capacity - _tail_idx;
		int second_seg_len = len - (first_seg_len);
		memcpy(buff, _data + _tail_idx, first_seg_len);
		_tail_idx = 0;
		memcpy(buff+first_seg_len, _data, second_seg_len);
		_tail_idx += second_seg_len;
		_size -= len;
	}

	printf("read len:%d, bufer cur size:%d, <head_idx:%d, tail_idx:%d>\n"
		, (int)len, (int)_size,  _head_idx, _tail_idx);

	return 0;
}

int RingBuffer::getBufferLen()
{
    return _size;
}

int RingBuffer::peekInt(int &value){
	int data_len = sizeof(value);
	if(_size < sizeof(data_len)){
		printf("cur size %d\n", (int)_size);
		return ERR_NO_DATA;
	}
    
	if(_tail_idx + data_len < (int)_capacity)
	{
	    value = *(int*)(_data+_tail_idx);
	}
	else
	{
	    char tmp_data[4] = {0};
		int first_seg_len = _capacity - _tail_idx;
        memcpy(tmp_data, _data+_tail_idx, first_seg_len);
		memcpy(tmp_data + first_seg_len, _data, data_len - first_seg_len);
		value = *(int*)(tmp_data);
	}

	printf("peek int result :%d(start_idx:%d)\n", value, _tail_idx);

	return 0;
}

