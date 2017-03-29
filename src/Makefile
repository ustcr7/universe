.PHONY : all arcv clean

PROJ_ROOT=..
include Makefile.in
include Makefile.release

MODULE_NAME=universe

SUB_MODULE += common
SUB_MODULE += net

define make_sub
	pwd && cd $(strip $(1))  && pwd && make && make arcv && cd - && pwd
endef

define clean_sub
	pwd && cd $(strip $(1))  && pwd && make clean && cd - && pwd
endef

all:
	$(foreach module, $(SUB_MODULE),  $(call make_sub, $(module)) )

release:
	$(foreach module, $(SUB_MODULE),  $(call make_sub, $(module)) )
	cp -r -f $(PROJ_LIB_ROOT)/* $(RELEASE_LIB_DIR)
	cp -r -f $(PROJ_BIN_ROOT)/* $(RELEASE_BIN_DIR)
	find $(PROJ_LIB_SRC) -name "*.h" | xargs -i cp -r -f --parents  {} $(RELEASE_INC_DIR)


clean:
	$(foreach module, $(SUB_MODULE),  $(call clean_sub, $(module)) )



#------------------------------------
#pwd && cd $(strip $(1))  && pwd && make && make arcv && cd - && pwd
# what a fuck , this do not work?????
#cd $(strip $(1))  && make && make arcv && cd - 



#pwd && cd $(strip $(1))  && pwd && make clean && cd - && pwd
# what a fuck , this do not work?????
#cd $(strip $(1))  && make clean 
