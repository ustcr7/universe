PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=unitest
MODULE_OBJ=$(MODULE_NAME)d
COMPILE_CPP = test_db_util.cpp
#COMPILE_CPP = test_proto.cpp
#COMPILE_CPP = test_ringbuffer.cpp
#SUB_MODULE += 

CFLAGS += -I .
CFLAGS += -I ../common/
CFLAGS += -L $(PROJ_LIB_ROOT)/
CFLAGS += -I $(MYSQL_INCLUDE_PATH)
DEP_LIB = $(MYSQL_LIB_PATH)/libmysqlclient.a

all:$(COMPILE_CPP)
	$(CPP) $(CFLAGS) $^  $(LDFLAGS) -o $(PROJ_BIN_ROOT)/$(MODULE_NAME)/$(MODULE_OBJ) -lcommon -lproto -lpthread $(DEP_LIB)
arcv:

	@echo "nothing"

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	@rm -rf $(PROJ_BIN_ROOT)/$(MODULE_NAME)/$(MODULE_OBJ)
	@find . -name "*.o" | xargs rm -f
	