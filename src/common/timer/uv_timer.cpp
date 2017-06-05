#include "uv_timer.h"
#include "massert.h"
#include "errcode.h"
#include <sys/time.h>
#include <cstring>
#include <stdio.h>

static UvTimer *gs_uv_timer = NULL;

int TimerData::SetTimeoutMs(u64 ms)
{
	timeout_ms = ms;
	printf("timer mid:%llu set new timeout ms:%llu\n", mid, ms);
	return 0;
}
u64 TimerData::GetTimeoutMs()
{
	return timeout_ms;
}

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

int UvTimer::AddTimer(int ms_interval, TimerCallbackFunc callback_func, const char *callback_data, u32 callback_data_len)
{
	printf("add timer timeout at %llu\n", ms_interval + GetCurRuntimeMs());

	u64 mid = AllocTimerData();

	TimerData *timer_data = GetTimerDataByMid(mid);
	timer_data->SetTimeoutMs(ms_interval + GetCurRuntimeMs());
	timer_data->callback_func = callback_func;
	if(callback_data != NULL)
	{
	    memcpy(timer_data->callback_data, callback_data, callback_data_len);
	    timer_data->callback_data_len = callback_data_len;
	}

	int idx = timer_data->GetTimeoutMs()% TV_SLOT_NUM;

	tv_timer_lists[idx].push_back(mid);

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

	for (u64 i = last_runtime_ms + 1; i <= cur_runtime_ms; ++i)
	{
		int idx = i%TV_SLOT_NUM;
		std::list<u64> *timer_list = &tv_timer_lists[idx];
		for (std::list<u64>::iterator it = timer_list->begin(); it != timer_list->end(); )
		{
			bool del = false;
			u64 mid = *it;
			TimerData *timer_data = GetTimerDataByMid(mid);
			massert_continue(timer_data != NULL);
			if (timer_data->GetTimeoutMs() == i)
			{
				timer_data->callback_func(timer_data->callback_data, timer_data->callback_data_len);

				it = timer_list->erase(it);
				del = true;
			}
			if (!del)
			{
				it++;
			}
		}
	}

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

