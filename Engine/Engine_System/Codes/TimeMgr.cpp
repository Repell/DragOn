#include "TimeMgr.h"

USING(ENGINE)

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
	:m_DeltaTime(0.0)
{
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuClock, sizeof(LARGE_INTEGER));
}


CTimeMgr::~CTimeMgr()
{
}

const _double & CTimeMgr::Get_DeltaTime()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_DeltaTime;
}

void CTimeMgr::Init_Time()
{
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CpuClock);
}

void CTimeMgr::Update_Time()
{
	QueryPerformanceCounter(&m_CurTime);
	
	if (m_CurTime.QuadPart - m_OldTime.QuadPart > m_CpuClock.QuadPart)
		Init_Time();		//일정 딜레이 이상이 걸리면 초기화하여 다시 시간 체크

	//QueryPerformanceFrequency(&m_CpuClock);

	m_DeltaTime = _double(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuClock.QuadPart;

	m_OldTime = m_CurTime;
}
