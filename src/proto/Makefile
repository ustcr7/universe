PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=proto
MODULE_OBJ=$(MODULE_NAME).o
MODULE_STATIC_LIB=lib$(MODULE_NAME).a

LIB_PATH=$(PROJ_LIB_ROOT)

ALL_CPP = *.cpp

CFLAGS += -I .
CFLAGS += -I $(PROTOBUF_INCLUDE_PATH)

#LDFLAGS += -L$(PROTOBUF_LIB_PATH)/

#DEP_LIB += $(PROTOBUF_LIB_PATH)/libprotobuf.a

PROTOBUF_PROTO = $(wildcard *.proto)
PROTOBUF_CPP = $(patsubst %.proto, %.pb.cc, $(PROTOBUF_PROTO))
PROTOBUF_HEADER_FILE = $(patsubst %.proto, %.pb.h, $(PROTOBUF_PROTO))

$(MODULE_OBJ):$(PROTOBUF_CPP)
	$(CPP) -c $(CFLAGS) $^  $(LDFLAGS)

$(PROTOBUF_CPP) : $(PROTOBUF_PROTO)
	$(PROTOBUF_BIN_PATH)/$(PROTOC)  -I=. --cpp_out=. $(PROTOBUF_PROTO)

arcv:
	ar -r $(LIB_PATH)/$(MODULE_STATIC_LIB)  *.o

#根据.cpp找到.o然后删除 比find 删除高级点吧
clean:
	@rm -rf $(LIB_PATH)/$(MODULE_STATIC_LIB)
	@find . -name "*.o" | xargs rm -f
	@rm -f $(PROTOBUF_CPP) $(PROTOBUF_HEADER_FILE)