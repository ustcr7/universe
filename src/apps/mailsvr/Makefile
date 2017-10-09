PROJ_ROOT=../../../

include ../../Makefile.in

MODULE_NAME=mailsvr
MODULE_OBJ=$(MODULE_NAME)d

ALL_CPP = *.cpp

CFLAGS += -I .
CFLAGS += -I ../../proto/
CFLAGS += -I $(MYSQL_INCLUDE_PATH)
CFLAGS += -I $(BRPC_INCLUDE_PATH)
CFLAGS += -L $(PROJ_LIB_ROOT)/
CFLAGS += -L $(PROTOBUF_LIB_PATH)
CFLAGS += -L $(MYSQL_LIB_PATH)
CFLAGS += -L $(BRPC_LIB_PATH)

$(MODULE_OBJ):$(ALL_CPP)
	echo $(ALL_CPP)
	$(CPP) $(CFLAGS)  -DGFLAGS_NS=gflags $^  $(LDFLAGS) -o $(PROJ_BIN_ROOT)/$(MODULE_NAME)/$(MODULE_OBJ) -lcommon -lproto -lpthread -lprotobuf  -lmysqlclient -lgflags -Bstatic  -lbrpc

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	rm -rf $(PROJ_BIN_ROOT)/$(MODULE_NAME)/$(MODULE_OBJ)
	find . -name "*.o" | xargs rm -f
	