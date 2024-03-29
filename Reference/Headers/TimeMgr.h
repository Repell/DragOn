#pragma once

#ifndef __TIMEMGR_H__
#define __TIMEMGR_H__

#include "Engine_Define.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
 	explicit CTimeMgr();
	virtual ~CTimeMgr();

public:
	const _double& Get_DeltaTime();

public:
	void Init_Time();
	void Update_Time();

private:
	_double m_DeltaTime; //시간 / fps   = 수행시간

	LARGE_INTEGER m_OldTime;		//측정 시작 시간(Time)
	LARGE_INTEGER m_CurTime;		//측정 후 지나간 시간(Time)
	LARGE_INTEGER m_CpuClock;	//CPU 진동수(Frequency)

};

NS_END

#endif // !__TIMEMGR_H__
