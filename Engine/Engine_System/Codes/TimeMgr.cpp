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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
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
		Init_Time();		//���� ������ �̻��� �ɸ��� �ʱ�ȭ�Ͽ� �ٽ� �ð� üũ

	//QueryPerformanceFrequency(&m_CpuClock);

	m_DeltaTime = _double(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuClock.QuadPart;

	m_OldTime = m_CurTime;
}
