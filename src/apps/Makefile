PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=apps
SUB_MODULE += gamesvr
#SUB_MODULE += mailsvr

define make_sub
	pwd && cd $(strip $(1))  && pwd && make && cd - && pwd
endef

define clean_sub
	pwd && cd $(strip $(1))  && pwd && make clean && cd - && pwd
endef

all:
	$(foreach module, $(SUB_MODULE),  $(call make_sub, $(module)) )
arcv:
	@echo "nothing"

clean:
	$(foreach module, $(SUB_MODULE),  $(call clean_sub, $(module)) )
