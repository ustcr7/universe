PROJ_ROOT=../../../

include ../../Makefile.in

FARTHER_MODULE=tools
MODULE_NAME=moc_client
MODULE_OBJ=$(MODULE_NAME)d

ALL_CPP = *.cpp

CFLAGS += -I .
CFLAGS += -L $(PROJ_LIB_ROOT)/

$(MODULE_OBJ):$(ALL_CPP)
	$(CPP) $(CFLAGS) $^  $(LDFLAGS) -o $(PROJ_BIN_ROOT)/$(FARTHER_MODULE)/$(MODULE_NAME)/$(MODULE_OBJ) -lcommon -lproto -lpthread -lprotobuf

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	@rm -rf $(PROJ_BIN_ROOT)/$(FARTHER_MODULE)/$(MODULE_NAME)/$(MODULE_OBJ)
	@find . -name "*.o" | xargs rm -f
	