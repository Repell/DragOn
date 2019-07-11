#pragma once

#ifndef __TIMEMGR_H__
#define __TIMEMGR_H__

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
 	explicit CTimeMgr();
	~CTimeMgr();

public:
	const float& Get_DeltaTime();

public:
	void Init_Time();
	void Update_Time();

private:
	float m_fDeltaTime; //�ð� / fps   = ����ð�

	LARGE_INTEGER m_OldTime;		//���� ���� �ð�(Time)
	LARGE_INTEGER m_CurTime;		//���� �� ������ �ð�(Time)
	LARGE_INTEGER m_CpuClock;	//CPU ������(Frequency)

};

#endif // !__TIMEMGR_H__
