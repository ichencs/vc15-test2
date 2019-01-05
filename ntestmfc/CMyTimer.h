#pragma once
#include <afxtempl.h>

class CMyTimer;
typedef CMap<UINT, UINT, CMyTimer*, CMyTimer*> CTimerMap;

class CMyTimer
{
public:
	CMyTimer();
	~CMyTimer();

public:
	//设置定时器，nElapse表示时间间隔，sz表示要提示的内容
	void SetMyTimer(UINT nElapse, CString sz);
	//销毁该实例的定时器
	void KillMyTimer();
	//保存该实例的定时器标志值
	UINT m_nTimerID;
	//静态数据成员要提示的内容 
	CString szContent;
	//静态数据成员，映射表类，用于保存所有的定时器信息
	static CTimerMap m_sTimeMap;
	//静态成员函数，用于处理定时器的消息
	static void CALLBACK MyTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

};

