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

//----------------��һ��ʱ����----------------
// �̶�1             ��λ��256   ��Сֵ0   ���ֵ 255
//slot0         ʱ�䷶Χ: 0                   ~     0
//slot1         ʱ�䷶Χ: 1                   ~     1
//...
//slot255       ʱ�䷶Χ: 255                 ~     255
//----------------�ڶ���ʱ����----------------
// �̶� 1<<8          ��λ��64  ��Сֵ 1<<8  ���ֵ (1<<14) -1
//slot0         ʱ�䷶Χ: 256                 ~    256+�̶�*1-1
//slot1         ʱ�䷶Χ: 256+�̶�*1          ~    256+�̶�*2-1
//...
//slot63        ʱ�䷶Χ: 256+�̶�*63         ~    �̶�*64-1
//----------------������ʱ����----------------
// �̶� 1<<14       ��λ��64  ��Сֵ 1<<14  ���ֵ (1<<20) -1
//slot0         ʱ�䷶Χ: (1<<14)              ~   (1<<14)+(1<<14)    - 1
//slot1         ʱ�䷶Χ: (1<<14)+(1<<14)      ~   (1<<14)+(1<<14)*2  - 1
//...
//slot63        ʱ�䷶Χ: (1<<14)+(1<<14)*62   ~   (1<<14)+(1<<14)*64  - 1
//----------------���ļ�ʱ����----------------
// �̶� 1<<20       ��λ��64  ��Сֵ 1<<20  ���ֵ (1<<26) -1
//slot0         ʱ�䷶Χ: (1<<20)              ~   (1<<20)+(1<<20)    - 1
//slot1         ʱ�䷶Χ: (1<<20)+(1<<20)      ~   (1<<20)+(1<<20)*2  - 1
//...
//slot63        ʱ�䷶Χ: (1<<20)+(1<<20)*63   ~   (1<<20)+(1<<20)*64  - 1
//----------------���弶ʱ����----------------
// �̶� 1<<26       ��λ��64  ��Сֵ 1<<26  ���ֵ (1<<32) -1
//slot0         ʱ�䷶Χ: (1<<26)              ~   (1<<26)+(1<<26)    - 1
//slot1         ʱ�䷶Χ: (1<<26)+(1<<26)      ~   (1<<26)+(1<<26)*2  - 1
//...
//slot63        ʱ�䷶Χ: (1<<26)+(1<<26)*63   ~   (1<<26)+(1<<26)*64  - 1

int UvTimer::ModifyTimerDataSlot(TimerData *timer_data)
{
	int left_time = timer_data->GetTimeoutTime();

	//����tv����,����left_time��Ӧ��tv

	//����tv�ж�Ӧ��slot
	//tv1:
	//slot_idx = left_time%256
	//tv2:
	//slot_idx = left_time/256 -1




	return 0;
}