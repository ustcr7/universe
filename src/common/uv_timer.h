#ifndef  __UNIVERSE_TIMER_H__
#define  __UNIVERSE_TIMER_H__

#include <list>
#include "../../common/base_type.h"

//�����»ص������᲻���timer��������
typedef int(*TimerCallbackFunc)(const char *callback_data, size_t callback_data_len);

class TimerData
{
public:
	int GetTimeoutTime();

	u64  mid;
	int  timeout_ms;
	TimerCallbackFunc *callback_func;
	char callback_data[128];
	size_t callback_data_len;
};

static const int TV1_SLOT_NUM = 255;
static const int TV2_SLOT_NUM = 63;
static const int TV3_SLOT_NUM = 63;
static const int TV4_SLOT_NUM = 63;
static const int TV5_SLOT_NUM = 63;

//����timer������ʽ��,��������Ҫ����ʱ������
//����һ��������sleep poll select  boost:asio��ʵ�ֵ�����Ҫ�������ʱ������

class UvTimer
{
public:
	static UvTimer* GetSingleInstance();
	//WCC_TODO:callback_func�ĳ�һ��type,Ȼ��type��func��̬ӳ��, ����ʹ�ص����������� �� ������resume(����Ҫ���溯��ָ��) 
	int AddTimer(int timerout_ms, TimerCallbackFunc *callback_func, const char *callback_data, size_t callback_data_len);
	int DelTimer(u64 mid);

	int UpdateTimer(u32 cur_ms_sec); //ϵͳ��ǰʱ��(����)  //��ʱ�����ȵ�������1����, ������������UpdateTimer��RunTimer�ļ��
	int RunTimer();                  //ָ�����ڵĶ�ʱ��

private:
	std::list<u64> tv1_timer_lists[TV1_SLOT_NUM];
	std::list<u64> tv2_timer_lists[TV2_SLOT_NUM];
	std::list<u64> tv3_timer_lists[TV3_SLOT_NUM];
	std::list<u64> tv4_timer_lists[TV4_SLOT_NUM];
	std::list<u64> tv5_timer_lists[TV5_SLOT_NUM];


	int ModifyTimerList(std::list<u64> *tv_timer_lists, int scale);

	int FreeTimerList(std::list<u64> *timer_list);

	int ModifyTimerDataSlot(TimerData *timer_data); //����timer��Ӧ��slot

	TimerData* GetTimerDataByMid(u64_mid);
	u64 AllocTimerData();
	void FreeTimerData(u64 mid);
	std::map<u64, TimerData*> timer_data_map;
};



#endif // ! __UNIVERSE_TIMER_H__
