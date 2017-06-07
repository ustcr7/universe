#ifndef  __UNIVERSE_TIMER_H__
#define  __UNIVERSE_TIMER_H__

#include <list>
#include <map>
#include "../base_type.h"

//考虑下回调函数会不会对timer产生冲入
typedef int(*TimerCallbackFunc)(const char *callback_data, u32 callback_data_len);

class TimerData
{
public:
	u64  mid;
	TimerCallbackFunc callback_func;
	char callback_data[128];
	u32 callback_data_len;

	int SetTimeoutMs(u64 ms);
	u64 GetTimeoutMs();
private:
	u64  timeout_ms;
};

static const int TV_SLOT_NUM = 256;

//1.定时器类型 (https://www.ibm.com/developerworks/cn/linux/l-cn-timerm/)
//这种timer算侵入式的,程序本身需要驱动时间流逝(与其他系统的耦合性比较高)
//还有一类用类似sleep poll select  boost:asio来实现的则不需要自身管理时间流逝
//2.callback函数
//callback_func改成一个type,然后type和func静态映射, 可以使回调函数更集中 且 更容易resume(不需要保存函数指针) 


//先实现各简单版的定时器吧,单时间轮,效率低后续优化成多级时间轮
// 所有的time_out时刻,都是指从系统开始 再走过time_out 毫秒的时刻
class UvTimer
{
public:
	UvTimer()
	{
		last_runtime_ms = 0;
		start_ms = 0;
		timer_data_map.clear();
		for (int i = 0; i < TV_SLOT_NUM; ++i)
		{
			tv_timer_lists[i].clear();
		}
	}
	static UvTimer* GetSingleInstance();
	int InitTimer();
	
	int AddTimer(int ms_interval, TimerCallbackFunc callback_func, const char *callback_data, u32 callback_data_len);
	int DelTimer(u64 mid);

	int RunTimer();                  //指定到期的定时器

private:
	std::list<u64> tv_timer_lists[TV_SLOT_NUM];

	u64 last_runtime_ms;  //上次跑到的毫秒数
	u64 start_ms; //系统开始时刻的毫秒数

	u64 GetCurRuntimeMs();


	int SetLastRuntimeMs(u64 ms);
	u64 GetLastRuntimeMs();

	//timer data管理
	TimerData* GetTimerDataByMid(u64 mid);
	u64 AllocTimerData();
	void FreeTimerData(u64 mid);
	std::map<u64, TimerData*> timer_data_map;
};



#endif // ! __UNIVERSE_TIMER_H__
