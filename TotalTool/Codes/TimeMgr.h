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
	float m_fDeltaTime; //시간 / fps   = 수행시간

	LARGE_INTEGER m_OldTime;		//측정 시작 시간(Time)
	LARGE_INTEGER m_CurTime;		//측정 후 지나간 시간(Time)
	LARGE_INTEGER m_CpuClock;	//CPU 진동수(Frequency)

};

#endif // !__TIMEMGR_H__
