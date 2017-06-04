#include "uv_timer.h"
#include "massert.h"
#include "errcode.h"
#include <sys/time.h>
#include <cstring>
#include <stdio.h>

static UvTimer *gs_uv_timer = NULL;

UvTimer* UvTimer::GetSingleInstance()
{
	if (gs_uv_timer == NULL)
	{
		gs_uv_timer = new UvTimer();
		massert_retval(gs_uv_timer != NULL, NULL);
	}

	return gs_uv_timer;
}

int UvTimer::InitTimer()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	u64 cur_tm_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	start_ms = cur_tm_ms;

	last_runtime_ms = 0;

	printf("uv timer init cur ms:%llu\n", cur_tm_ms);

	return 0;
}

//tv1槽位: 1,2,3... 255
//当n时刻,加了一个1ms延迟的timer,注意并不是加到槽位1后面 而是加到 n%256 + 1 位置
int UvTimer::AddTimer(int ms_interval, TimerCallbackFunc callback_func, const char *callback_data, u32 callback_data_len)
{
	printf("add timer timeout at %llu\n", ms_interval + GetCurRuntimeMs());

	u64 mid = AllocTimerData();

	TimerData *timer_data = GetTimerDataByMid(mid);
	timer_data->timeout_ms = ms_interval + GetCurRuntimeMs();
	timer_data->callback_func = callback_func;
	if(callback_data != NULL)
	{
	    memcpy(timer_data->callback_data, callback_data, callback_data_len);
	    timer_data->callback_data_len = callback_data_len;
	}

	ModifyTimerDataSlot(timer_data);

	return 0;
}

int UvTimer::DelTimer(u64 mid)
{
	//WCC_TODO:找到对应的list,遍历后删除之
	massert_noeffect(0); //not implement
	return 0;
}

int UvTimer::RunTimer()
{
	u64 cur_runtime_ms = GetCurRuntimeMs();

	massert_retval(cur_runtime_ms >= last_runtime_ms, ERR_INVALID_PARAM);
	if (cur_runtime_ms == last_runtime_ms) //定时器的最大精度就是毫秒
	{
		return 0;
	}

	//尝试转动高阶时间轮
	for (u64 i = last_runtime_ms + 1; i <= cur_runtime_ms; ++i)
	{
		UpdateTimerWheal(i);
	}

	//执行第1级时间轮对应槽位上的回调事件 
	int slot_idx = cur_runtime_ms % 256;
	std::list<u64> *timer_list = &tv1_timer_lists[slot_idx];

	{
		//debug code
		if (timer_list->size() > 0)
		{
			printf("^^^^^^^tv1 slot:%d, timer num:%d\n", slot_idx, (int)timer_list->size());
		}
	}

	//遍历list,执行回调函数,然后释放list
	for (std::list<u64>::iterator it = timer_list->begin(); it != timer_list->end(); ++it)
	{
		u64 mid = *it;
		TimerData *timer_data = GetTimerDataByMid(mid);
		massert_continue(timer_data != NULL);

		timer_data->callback_func(timer_data->callback_data, timer_data->callback_data_len);
		//printf("timeout callback sucess timeout runtime ms:%d, cur ms:%llu\n", timer_data->timeout_ms, GetCurRuntimeMs());
	}

	FreeTimerList(timer_list);

	last_runtime_ms = cur_runtime_ms;

	return 0;
}

//获取从系统开始运行,到当前时刻走过的毫秒数
u64  UvTimer::GetCurRuntimeMs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	u64 cur_tm_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	return cur_tm_ms - start_ms;
}

int UvTimer::ModifyTimerList(std::list<u64> *tv_timer_lists, int scale)
{
	//slot 0降低到低阶时间轮
	std::list<u64> *timer_list = &tv_timer_lists[0];
	for (std::list<u64>::iterator it = timer_list->begin(); it != timer_list->end(); ++it)
	{
		u64 mid = *it;
		TimerData *timer_data = GetTimerDataByMid(mid);
		massert_continue(timer_data != NULL);

		timer_data->timeout_ms -= scale; //改成get set函数吧,看看到底哪里该出问题了

		ModifyTimerDataSlot(timer_data);

		printf("@@@@high tv down to low tv, old timeout ms:%d, new:%d, cur runtime ms:%llu, mid:%llu\n"
			, timer_data->timeout_ms+scale, timer_data->timeout_ms, GetCurRuntimeMs(), timer_data->mid);
	}
	timer_list->clear();

	//slot 1 ~ 62修正槽位的timeout时间,并修正list所属槽位
	for (int i = 0; i <63; ++i)
	{
		tv_timer_lists[i] = tv_timer_lists[i + 1];
		std::list<u64> *tmp_timer_list = &tv_timer_lists[i];
		for (std::list<u64>::iterator it = tmp_timer_list->begin(); it != tmp_timer_list->end(); ++it)
		{
			u64 mid = *it;
			TimerData *timer_data = GetTimerDataByMid(mid);
			massert_continue(timer_data != NULL);
			timer_data->timeout_ms -= scale;
		}
	}
	tv_timer_lists[62].clear();

	//printf("ModifyTimerList success\n");
	return 0;
}

int UvTimer::UpdateTimerWheal(u64 tm_ms)
{
	//printf("UpdateTimerWheal tmms:%llu\n", tm_ms);
	if (tm_ms % (1 << 26) == 0)
	{
		//调整第5级时间轮
		ModifyTimerList(tv5_timer_lists, 1 << 26);
		printf("------------->modify tv 5\n");
	}
	if (tm_ms % (1 << 20) == 0)
	{
		//调整第4级时间轮
		ModifyTimerList(tv4_timer_lists, 1 << 20);
		printf("------------->modify tv 4\n");
	}
	if (tm_ms % (1 << 14) == 0)
	{
		//调整第3级时间轮
		ModifyTimerList(tv3_timer_lists, 1 << 14);
		printf("------------->modify tv 3\n");
	}
	if (tm_ms % (1 << 8) == 0)
	{
		//调整第2级时间轮
		//slot[0] --> 转移到tv1: 256+0, 256+1, ...256+256-1 --> [0] [1] ... [255]
		//slot[1] = slot[0] ... slot[62] = slot[61]
		ModifyTimerList(tv2_timer_lists, 1 << 8);
		//printf("modify tv 2\n"); //too frequency
	}
	return 0;
}


//----------------第一级时间轮----------------
// 刻度1             槽位数256   最小值0   最大值 255
//slot0         时间范围: 0                   ~     0
//slot1         时间范围: 1                   ~     1
//...
//slot255       时间范围: 255                 ~     255
//----------------第二级时间轮----------------
// 刻度 1<<8          槽位数64  最小值 1<<8  最大值 (1<<14) -1
//slot0         时间范围: 256                 ~    256+刻度*1-1
//slot1         时间范围: 256+刻度*1          ~    256+刻度*2-1
//...
//slot63        时间范围: 256+刻度*63         ~    刻度*64-1
//----------------第三级时间轮----------------
// 刻度 1<<14       槽位数64  最小值 1<<14  最大值 (1<<20) -1
//slot0         时间范围: (1<<14)              ~   (1<<14)+(1<<14)    - 1
//slot1         时间范围: (1<<14)+(1<<14)      ~   (1<<14)+(1<<14)*2  - 1
//...
//slot63        时间范围: (1<<14)+(1<<14)*62   ~   (1<<14)+(1<<14)*64  - 1
//----------------第四级时间轮----------------
// 刻度 1<<20       槽位数64  最小值 1<<20  最大值 (1<<26) -1
//slot0         时间范围: (1<<20)              ~   (1<<20)+(1<<20)    - 1
//slot1         时间范围: (1<<20)+(1<<20)      ~   (1<<20)+(1<<20)*2  - 1
//...
//slot63        时间范围: (1<<20)+(1<<20)*63   ~   (1<<20)+(1<<20)*64  - 1
//----------------第五级时间轮----------------
// 刻度 1<<26       槽位数64  最小值 1<<26  最大值 (1<<32) -1
//slot0         时间范围: (1<<26)              ~   (1<<26)+(1<<26)    - 1
//slot1         时间范围: (1<<26)+(1<<26)      ~   (1<<26)+(1<<26)*2  - 1
//...
//slot63        时间范围: (1<<26)+(1<<26)*63   ~   (1<<26)+(1<<26)*64  - 1

int UvTimer::ModifyTimerDataSlot(TimerData *timer_data)
{
	u64 timeout_ms = timer_data->timeout_ms;
	int left_time = timeout_ms - GetCurRuntimeMs();
	if (left_time < 0)
	{
		left_time = 0;
	}

	printf("ModifyTimerDataSlot left time:%d, timeout at:%llu \n", left_time, timeout_ms);

	std::list<u64> *tv_timer_lists = NULL;
	int slot_idx = -1;
	if (left_time >= 0 && left_time < (1<<8))
	{
		tv_timer_lists = tv1_timer_lists;
		slot_idx = timeout_ms %256;
		printf("add timer to tv1\n");
	}
	else if (left_time >= (1<<8) && left_time < (1 << 14))
	{
		tv_timer_lists = tv2_timer_lists;
		slot_idx = (timeout_ms / (1 << 8)) - 1;
		printf("add timer to tv2\n");
	}
	else if (left_time >= (1<<14) && left_time < (1 << 20))
	{
		tv_timer_lists = tv3_timer_lists;
		slot_idx = (timeout_ms / (1 << 14)) - 1;
		printf("add timer to tv3\n");
	}
	else if (left_time >= (1<<20) && left_time < (1 << 26))
	{
		tv_timer_lists = tv4_timer_lists;
		slot_idx = (timeout_ms / (1 << 20)) - 1;
		printf("add timer to tv4\n");

	}
	else if (left_time >= 26 && left_time < (1 << 31)) //WCC_TODO这里写成  1<<32会报错的
	{
		tv_timer_lists = tv5_timer_lists;
		slot_idx = (timeout_ms / (1 << 26)) - 1;
		printf("add timer to tv5\n");
	}
	else
	{
		massert_retval(0, -1); //invali arg
	}

	massert_retval(tv_timer_lists != NULL && slot_idx != -1, ERR_INVALID_PARAM);

	tv_timer_lists[slot_idx].push_back(timer_data->mid);

	printf("slot pushback slot_idx :[%d], mid:%llu\n", slot_idx, timer_data->mid);

	return 0;
}

int UvTimer::FreeTimerList(std::list<u64> *timer_list)
{
	massert_retval(timer_list != NULL, ERR_INVALID_PARAM);
	for (std::list<u64>::iterator it = timer_list->begin(); it != timer_list->end(); ++it)
	{
		FreeTimerData(*it);
	}
	timer_list->clear();

	return 0;
}

TimerData* UvTimer::GetTimerDataByMid(u64 mid)
{
	std::map<u64, TimerData*>::iterator iter = timer_data_map.find(mid);
	massert_retval(iter != timer_data_map.end(), NULL);

	return iter->second;
}

u64 UvTimer::AllocTimerData()
{
	static u64 timer_mid_seq = 0;
	++timer_mid_seq;

	TimerData* timer_data = new TimerData();
	timer_data->mid = timer_mid_seq;

	timer_data_map.insert(std::make_pair(timer_data->mid, timer_data));

	return timer_data->mid;
}

void UvTimer::FreeTimerData(u64 mid)
{
	TimerData *ptr = GetTimerDataByMid(mid);
	massert_retnone(ptr != NULL);

	delete ptr;
	ptr = NULL;
	timer_data_map.erase(mid);
}

