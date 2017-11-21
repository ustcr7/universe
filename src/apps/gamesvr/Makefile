PROJ_ROOT=../../../

include ../../Makefile.in

MODULE_NAME=gamesvr
MODULE_OBJ=$(MODULE_NAME)d

ALL_CPP = *.cpp
ALL_CPP += move/*.cpp
ALL_CPP += chat/*.cpp
ALL_CPP += battle/*.cpp
ALL_CPP += db/*.cpp
ALL_CPP += rank/*.cpp
#ALL_CPP += quest/*.cpp

CFLAGS += -I .
CFLAGS += -I ../../proto/
CFLAGS += -I $(MYSQL_INCLUDE_PATH)
CFLAGS += -L $(PROJ_LIB_ROOT)/
CFLAGS += -L $(PROTOBUF_LIB_PATH)
CFLAGS += -L $(MYSQL_LIB_PATH)
CFLAGS += -I $(HIREDIS_INCLUDE_PATH)
CFLAGS += -L $(HIREDIS_LIB_PATH)


#DEP_LIB = $(MYSQL_LIB_PATH)/libmysqlclient.a
#DEP_LIB += $(PROTOBUF_LIB_PATH)/libprotobuf.a 

$(MODULE_OBJ):$(ALL_CPP)
	$(CPP) $(CFLAGS) $^  $(LDFLAGS) -o $(PROJ_BIN_ROOT)/$(MODULE_NAME)/$(MODULE_OBJ) -lcommon -lproto -lpthread -lprotobuf  -lmysqlclient -lhiredis

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	rm -rf $(PROJ_BIN_ROOT)/$(MODULE_NAME)/$(MODULE_OBJ)
	find . -name "*.o" | xargs rm -f
	