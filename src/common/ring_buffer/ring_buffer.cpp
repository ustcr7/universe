#include "common/errcode.h"
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


int RingBuffer::write(const char *data, size_t len){
	if(len <= 0){
		return ERR_INVALID_PARAM; 
	}
	if(len > _capacity - _size){
		printf("len %zu, remain size %zu\n", len, _capacity - _size);
	}

	if(_head_idx + len < _capacity){
		memcpy(_data + _head_idx, data, len);
		_head_idx = _head_idx + len;
		if(_head_idx == (int)(_capacity - 1)){
			_head_idx = 0;
		}
		_size += len;
	}else{
		size_t len1 = _capacity - _head_idx;
		write(data, len1);
		write(data+len1, len - len1);
	}

	return 0;
}


int RingBuffer::read(char *buff, size_t &len){
	if(len ==0 || _size == 0){
		len = 0;
		return 0;
	}
	if(len > _size){
		len = _size;
	}

	if(_tail_idx + len < _capacity){
		memcpy(buff, _data+_tail_idx, len);
		_tail_idx += len;
		if(_tail_idx == (int)(_capacity - 1)){
			_tail_idx = 0;
		}
		_size -= len;
	}else{
		size_t len1 = _capacity - _tail_idx;
		read(buff,  len1);
		size_t len2 = len - len1;
		read(buff+len1, len2);
	}
	return 0;
}


int RingBuffer::peek_int(int &value){
	if(_size < sizeof(value)){
		printf("cur size %d\n", (int)_size);
		return ERR_NO_DATA;
	}

	char *data =(char*)&value;
	size_t len = sizeof(value);

	if(_tail_idx + len < _capacity){
		memcpy(data, _data+_tail_idx, len);
	}else{
		size_t len1 = _capacity - _tail_idx;
		memcpy(data, _data+_head_idx, len1);
		size_t len2 = len - len1;
		memcpy(data, _data, len2);
	}

	return 0;
}

int RingBuffer::pop_int(int &value){
	size_t len =  sizeof(value);
	return read((char*)&value, len);
}


int RingBuffer::size(){
	return _size;
}