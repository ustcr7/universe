.PHONY : all arcv clean

PROJ_ROOT=..
include Makefile.in

MODULE_NAME=universe

SUB_MODULE += proto
SUB_MODULE += common
SUB_MODULE += apps
SUB_MODULE += tools
SUB_MODULE += unitest

define make_sub
	pwd && cd $(strip $(1))  && pwd && make && make arcv && cd - && pwd
endef

define clean_sub
	pwd && cd $(strip $(1))  && pwd && make clean && cd - && pwd
endef

all:
	$(foreach module, $(SUB_MODULE),  $(call make_sub, $(module)) )

clean:
	$(foreach module, $(SUB_MODULE),  $(call clean_sub, $(module)) )


#------------------------------------
#pwd && cd $(strip $(1))  && pwd && make && make arcv && cd - && pwd
# what a fuck , this do not work?????
#cd $(strip $(1))  && make && make arcv && cd - 



#pwd && cd $(strip $(1))  && pwd && make clean && cd - && pwd
# what a fuck , this do not work?????
#cd $(strip $(1))  && make clean 
