PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=tools
MODULE_OBJ=$(MODULE_NAME).o
MODULE_STATIC_LIB=lib$(MODULE_NAME).a

LIB_PATH=$(PROJ_LIB_ROOT)

ALL_CPP=*.cpp
CFLAGS+= -I ../
CFLAGS+= -I ../../dep/include/protobuf
CFLAGS+= -L ../../dep/lib
LDFLAGS+=-lprotobuf


$(MODULE_OBJ):$(ALL_CPP)
	$(CPP) -c $(CFLAGS) $^  $(LDFLAGS) -o $@

arcv:
	ar -r $(LIB_PATH)/$(MODULE_STATIC_LIB)  *.o


clean:
	@rm -rf $(MODULE_OBJ)
	@rm -rf $(LIB_PATH)/$(MODULE_STATIC_LIB)
