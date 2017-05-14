PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=common
MODULE_OBJ=$(MODULE_NAME).o
MODULE_STATIC_LIB=lib$(MODULE_NAME).a

LIB_PATH=$(PROJ_LIB_ROOT)

#ALL_CPP = *.cpp
ALL_CPP += ring_buffer/*.cpp
ALL_CPP += net/*.cpp
ALL_CPP += db/*.cpp

CFLAGS += -I .
CFLAGS += -I $(MYSQL_INCLUDE_PATH)
#WCC_TODO 为提高项目独立性,可以把mysql的头文件拷贝到src/deps/mysql目录中
CFLAGS += -I $(MYSQL_INCLUDE_PATH)

$(MODULE_OBJ):$(ALL_CPP)
	$(CPP) -c $(CFLAGS) $^  $(LDFLAGS)

arcv:
	ar -r $(LIB_PATH)/$(MODULE_STATIC_LIB)  *.o

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	rm -rf $(LIB_PATH)/$(MODULE_STATIC_LIB)
	find . -name "*.o" | xargs rm -f