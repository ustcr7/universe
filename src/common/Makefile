PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=common
MODULE_OBJ=$(MODULE_NAME).o
MODULE_STATIC_LIB=lib$(MODULE_NAME).a

LIB_PATH=$(PROJ_LIB_ROOT)

#ALL_CPP = *.cpp
ALL_CPP += ring_buffer/*.cpp
ALL_CPP += net/*.cpp

CFLAGS += -I .

$(MODULE_OBJ):$(ALL_CPP)
	$(CPP) -c $(CFLAGS) $^  $(LDFLAGS)

arcv:
	ar -r $(LIB_PATH)/$(MODULE_STATIC_LIB)  *.o

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	rm -rf $(LIB_PATH)/$(MODULE_STATIC_LIB)
	find . -name "*.o" | xargs rm -f