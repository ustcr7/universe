.PHONY : clean

INC_PATH += -I ..

CFLAGS += $(INC_PATH)

massert:test-massert.cpp
	g++ $^ $(CFLAGS) -g -o $@


clean:
	@rm -rf massert
