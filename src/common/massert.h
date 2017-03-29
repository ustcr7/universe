#ifndef _UNIVERSE_MASSERT_H_
#define _UNIVERSE_MASSERT_H_

#include <assert.h>
#include <stdio.h>

namespace universe{

#define massert_log(cond, process) \
{\
	if(!(cond)){ \
		printf("FATAL: assert failed %s , file %s, line %d", (#cond),  __FILE__, __LINE__); \
		assert((cond) && (#cond)); \
		process;\
	}\
}

#define massert_retval(cond, ret) massert_log(cond, return ret)
#define massert_retnone(cond) massert_log(cond, return)
#define massert_break(cond) massert_log(cond, break)
#define massert_continue(cond) massert_log(cond, continue)
#define massert_noeffect(cond) massert_log(cond, )


}

#endif
