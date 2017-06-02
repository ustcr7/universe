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

int UvTimer::AddTimer(int timeout_ms, TimerCallbackFunc *callback_func, const char *callback_data, size_t callback_data_len)
{
	u64 mid = AllocTimerData();

	TimerData *timer_data = GetTimerDataByMid(mid);
	timer_data->timeout_ms = timeout_ms;
	timer_data->callback_func = callback_func;
	memcpy(timer_data->callback_data, callback_data, callback_data_len);
	timer_data->callback_data_len = callback_data_len;

	ModifyTimerDataSlot(timer_data);

	return 0;
}

int UvTimer::DelTimer(u64 mid)
{
	//WCC_TODO:�ҵ���Ӧ��list,������ɾ��֮
	return 0;
}


//�����ס��һ��runtimer��ʱ��, �Ӹ�ʱ�䵽��ǰʱ�̼������ʱ����Ӧ��timerdata��Ӧ�ò���һ��
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

int UvTimer::ModifyTimerList(std::list<u64> *tv_timer_lists, int scale)
{
	std::list<u64> *timer_list = &tv_timer_lists[0];
	for (std::list<u64>::iterator it = timer_list->begin(); it != timer_list->end(); ++it)
	{
		u64 mid = *it;
		TimerData *timer_data = GetTimerDataByMid(mid);
		massert_continue(timer_data != NULL);

		timer_data->GetTimeoutTime -= scale;

		ModifyTimerDataSlot(timer_data);
	}
	timer_list->clear();

	for (int i = 0; i <62; ++i)
	{
		tv_timer_lists[i] = tv_timer_lists[i + 1];
	}
}

int UvTimer::RunTimer()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int tm_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	if (tm_ms % (1 << 26) == 0)
	{
		//������5��ʱ����
		ModifyTimerList(tv5_timer_lists, 1 << 26);
	}
	else if (tm_ms % (1 << 20) == 0)
	{
		//������4��ʱ����
		ModifyTimerList(tv4_timer_lists, 1 << 20);
	}
	else if (tm_ms % (1 << 14) == 0)
	{
		//������3��ʱ����
		ModifyTimerList(tv3_timer_lists, 1 << 14);
	}
	else if (tm_ms%(1<<8) == 0)
	{
		//������2��ʱ����
		//slot[0] --> ת�Ƶ�tv1: 256+0, 256+1, ...256+256-1 --> [0] [1] ... [255]
		//slot[1] = slot[0] ... slot[62] = slot[61]
		ModifyTimerList(tv2_timer_lists, 1 << 8);
	}

	//ִ�е�1��ʱ���ֶ�Ӧ��λ�ϵĻص��¼�
	int slot_idx = tm_ms % 256;
	std::list<u64> *timer_list = &tv_timer_lists[slot_idx];

	//����list,ִ�лص�����,Ȼ���ͷ�list
	for (std::list<u64>::iterator it = timer_list->begin(); it != timer_list->end(); ++it)
	{
		u64 mid = *it;
		TimerData *timer_data =  GetTimerDataByMid(mid);
		massert_continue(timer_data != NULL);

		timer_data->callback_func( timer_data->callback_data, timer_data->callback_data_len);
	}

	FreeTimerList(timer_list);

	return 0;
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

	std::list<u64> *tv_timer_lists = NULL;
	int slot_idx = -1;
	if (left_time >= 0 && left_time < (1<<8))
	{
		tv_timer_lists = tv1_timer_lists;
		slot_idx = slot_idx % 256;
	}
	else if (left_time >= 8 && left_time < (1 << 14))
	{
		tv_timer_lists = tv2_timer_lists;
		slot_idx = (left_time) / (1 << 8) - 1;
	}
	else if (left_time >= 14 && left_time < (1 << 20))
	{
		tv_timer_lists = tv3_timer_lists;
		slot_idx = (left_time) / (1 << 14) - 1;
	}
	else if (left_time >= 20 && left_time < (1 << 26))
	{
		tv_timer_lists = tv4_timer_lists;
		slot_idx = (left_time) / (1 << 20) - 1;

	}
	else if (left_time >= 26 && left_time < (1 << 32))
	{
		tv_timer_lists = tv5_timer_lists;
		slot_idx = (left_time) / (1 << 26) - 1;
	}
	else
	{
		massert_retval(0, -1); //invali arg
	}

	massert_retval(tv_timer_lists != NULL && slot_idx != -1, ERR_INVALID_PARAM);

	tv_timer_lists[slot_idx]->push_back(timer_data);

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