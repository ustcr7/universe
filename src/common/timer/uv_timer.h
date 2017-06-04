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
	int  timeout_ms;
	TimerCallbackFunc callback_func;
	char callback_data[128];
	u32 callback_data_len;
};

static const int TV1_SLOT_NUM = 256;
static const int TV2_SLOT_NUM = 63;
static const int TV3_SLOT_NUM = 63;
static const int TV4_SLOT_NUM = 63;
static const int TV5_SLOT_NUM = 63;

//1.定时器类型 (https://www.ibm.com/developerworks/cn/linux/l-cn-timerm/)
//这种timer算侵入式的,程序本身需要驱动时间流逝(与其他系统的耦合性比较高)
//还有一类用类似sleep poll select  boost:asio来实现的则不需要自身管理时间流逝
//2.callback函数
//callback_func改成一个type,然后type和func静态映射, 可以使回调函数更集中 且 更容易resume(不需要保存函数指针) 



// 所有的time_out时刻,都是指从系统开始 再走过time_out 毫秒的时刻
class UvTimer
{
public:
	UvTimer()
	{
		last_runtime_ms = 0;
		start_ms = 0;
		timer_data_map.clear();
		for (int i = 0; i < TV1_SLOT_NUM; ++i)
		{
			tv1_timer_lists[i].clear();
		}
		for (int i = 0; i < TV2_SLOT_NUM; ++i)
		{
			tv2_timer_lists[i].clear();
		}
		for (int i = 0; i < TV3_SLOT_NUM; ++i)
		{
			tv3_timer_lists[i].clear();
		}
		for (int i = 0; i < TV4_SLOT_NUM; ++i)
		{
			tv4_timer_lists[i].clear();
		}
		for (int i = 0; i < TV5_SLOT_NUM; ++i)
		{
			tv5_timer_lists[i].clear();
		}
	}
	static UvTimer* GetSingleInstance();
	int InitTimer();
	
	int AddTimer(int ms_interval, TimerCallbackFunc callback_func, const char *callback_data, u32 callback_data_len);
	int DelTimer(u64 mid);

	int RunTimer();                  //指定到期的定时器

private:
	std::list<u64> tv1_timer_lists[TV1_SLOT_NUM];
	std::list<u64> tv2_timer_lists[TV2_SLOT_NUM];
	std::list<u64> tv3_timer_lists[TV3_SLOT_NUM];
	std::list<u64> tv4_timer_lists[TV4_SLOT_NUM];
	std::list<u64> tv5_timer_lists[TV5_SLOT_NUM];
	u64 last_runtime_ms;  //上次跑到的毫秒数
	u64 start_ms; //系统开始时刻的毫秒数


	u64 GetCurRuntimeMs();

	int UpdateTimerWheal(u64 tm_ms);                                   //转动时间轮
	int ModifyTimerList(std::list<u64> *tv_timer_lists, int scale);	   //调整时间轮各槽位

	int FreeTimerList(std::list<u64> *timer_list);                     //释放list

	int ModifyTimerDataSlot(TimerData *timer_data);                   //调整timer_data对应的slot

	//timer data管理
	TimerData* GetTimerDataByMid(u64 mid);
	u64 AllocTimerData();
	void FreeTimerData(u64 mid);
	std::map<u64, TimerData*> timer_data_map;
};



#endif // ! __UNIVERSE_TIMER_H__
