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
	int left_time = timer_data->GetTimeoutTime();

	//根据tv区间,计算left_time对应的tv

	//计算tv中对应的slot
	//tv1:
	//slot_idx = left_time%256
	//tv2:
	//slot_idx = left_time/256 -1




	return 0;
}