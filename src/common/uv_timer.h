#ifndef  __UNIVERSE_TIMER_H__
#define  __UNIVERSE_TIMER_H__

#include <list>
#include "../../common/base_type.h"

typedef int(*TimerCallbackFunc)(const char *callback_data, size_t callback_data_len);

class TimerData
{
public:
	int GetTimeoutTime();
private:
	u64  mid;
	int  timeout_ms;
	TimerCallbackFunc *callback_func;
	char callback_data[128];
	size_t callback_data_len;
};

static const int TV1_SLOT_NUM = 255;
static const int TV2_SLOT_NUM = 64;
static const int TV3_SLOT_NUM = 64;
static const int TV4_SLOT_NUM = 64;
static const int TV5_SLOT_NUM = 64;

class UvTimer
{
public:
	static UvTimer* GetSingleInstance();
	//WCC_TODO:callback_func改成一个type,然后type和func静态映射, 可以使回调函数更集中 且 更容易resume(不需要保存函数指针) 
	int AddTimer(int timerout_ms, TimerCallbackFunc *callback_func, const char *callback_data, size_t callback_data_len);
	int DelTimer(u64 mid);

	int UpdateTimer(u32 cur_ms_sec); //系统当前时间(毫秒)  //定时器精度的上限是1毫秒, 下限是外界调用UpdateTimer和RunTimer的间隔
	int RunTimer();                  //指定到期的定时器

private:
	std::list<u64> tv1_timer_lists[TV1_SLOT_NUM];
	std::list<u64> tv2_timer_lists[TV2_SLOT_NUM];
	std::list<u64> tv3_timer_lists[TV3_SLOT_NUM];
	std::list<u64> tv4_timer_lists[TV4_SLOT_NUM];
	std::list<u64> tv5_timer_lists[TV5_SLOT_NUM];



	int ModifyTimerDataSlot(TimerData *timer_data); //调整timer对应的slot

	TimerData* GetTimerDataByMid(u64_mid);
	std::map<u64, TimerData*> timer_data_map;
};



#endif // ! __UNIVERSE_TIMER_H__
