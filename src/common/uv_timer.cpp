#include "uv_timer.h"
#include "../../common/massert.h"
#include "../../common/errcode.h"

static UvTimer *gs_uv_timer = NULL;


//tv1 ʱ�䷶Χ 256��slot   ÿ��slotʱ������1ms      slot0[0~0]          slot1[1~1]               ... slot255[255~255]
//tv2 ʱ�䷶Χ 64��slot    ÿ��slotʱ������256ms    slot0[256~256+256]  slot1[256+256~256+2*256] ... slot63[256+63*256 ~ 256+63*256]
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
	//tv1ת��1Ȧ,��tv2���д���
	//��һ��slot��Ӧ�� time list, �������չ���ʱ��ŵ�tv1�ĸ���slot��
	//����ÿ��slot��Ӧ��time list�Ƶ�ǰһ��slot
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