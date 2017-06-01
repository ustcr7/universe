#include "uv_timer.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"

static UvTimer *gs_uv_timer = NULL;


//tv1 时间范围 256个slot   每个slot时间区间1ms      slot0[0~0]          slot1[1~1]               ... slot255[255~255]
//tv2 时间范围 64个slot    每个slot时间区间256ms    slot0[256~256+256]  slot1[256+256~256+2*256] ... slot63[256+63*256 ~ 256+63*256]
//


UvTimer* UvTimer::GetSingleInstance()
{
	if (gs_uv_timer == NULL)
	{
		gs_uv_timer = new UvTimer();
		massert_retval(gs_uv_timer != NULL, NULL);
	}

	return gs_uv_timer;
}

int UvTimer::AddTimer(int timerout_ms, TimerCallbackFunc *callback_func, const char *callback_data, size_t callback_data_len)
{

}

int UvTimer::DelTimer(u64 mid)
{

}

int UvTimer::UpdateTimer(u32 cur_ms_sec)
{
	//tv1转了1圈,对tv2进行处理
	//第一个slot对应的 time list, 遍历按照过期时间放到tv1的各个slot中
	//后续每个slot对应的time list移到前一个slot
	bool roll_tv2 =  (cur_ms_sec % (1<<8) == 0);
	std::list<64> &first_slot_list = tv1_timer_lists[0];
	for (std::list<u64>::iterator iter = first_slot_list.begin(); iter != first_slog_list.end(); ++iter)
	{
		u64 mid = *iter;
		TimerData *timer_data = GetTimerDataByMid(mid);
		massert_continue(timer_data != NULL);


	}
	

	bool roll_tv3 =  (cur_ms_sec % (1<<14) == 0);
}

int UvTimer::RunTimer()
{

}


int UvTimer::ModifyTimerDataSlot(TimerData *timer_data)
{
	int left_time = timer_data->GetTimeoutTime();


}