PROJ_ROOT=../../

include ../Makefile.in

MODULE_NAME=tools
SUB_MODULE += moc_client


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
