#ifndef  __UNIVERSE_TIMER_H__
#define  __UNIVERSE_TIMER_H__

#include <list>
#include <map>
#include "../base_type.h"

//�����»ص������᲻���timer��������
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

//1.��ʱ������ (https://www.ibm.com/developerworks/cn/linux/l-cn-timerm/)
//����timer������ʽ��,��������Ҫ����ʱ������(������ϵͳ������ԱȽϸ�)
//����һ��������sleep poll select  boost:asio��ʵ�ֵ�����Ҫ�������ʱ������
//2.callback����
//callback_func�ĳ�һ��type,Ȼ��type��func��̬ӳ��, ����ʹ�ص����������� �� ������resume(����Ҫ���溯��ָ��) 


//��ʵ�ָ��򵥰�Ķ�ʱ����,��ʱ����,Ч�ʵͺ����Ż��ɶ༶ʱ����
// ���е�time_outʱ��,����ָ��ϵͳ��ʼ ���߹�time_out �����ʱ��
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

	int RunTimer();                  //ָ�����ڵĶ�ʱ��

private:
	std::list<u64> tv_timer_lists[TV_SLOT_NUM];

	u64 last_runtime_ms;  //�ϴ��ܵ��ĺ�����
	u64 start_ms; //ϵͳ��ʼʱ�̵ĺ�����

	u64 GetCurRuntimeMs();


	int SetLastRuntimeMs(u64 ms);
	u64 GetLastRuntimeMs();

	//timer data����
	TimerData* GetTimerDataByMid(u64 mid);
	u64 AllocTimerData();
	void FreeTimerData(u64 mid);
	std::map<u64, TimerData*> timer_data_map;
};



#endif // ! __UNIVERSE_TIMER_H__
