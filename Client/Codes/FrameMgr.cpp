#include "stdafx.h"
#include "FrameMgr.h"

#include "Export_System.h"

CFrameMgr::CFrameMgr()
	: m_fDeletaTime(0.f), m_fSecForFrm(0.f), m_fpsTime(0.f),  m_iFPS(0), szFPS(L"")
{
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuClock, sizeof(LARGE_INTEGER));
}


CFrameMgr::~CFrameMgr()
{
}

void CFrameMgr::InitFrm(float Frm)
{
	m_fSecForFrm = 1 / Frm;		//초당 FRAME 재생 수 설정 (1 / 60 ~ 144);

	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CpuClock);	// 초당 진동수 1 / 1000000 

}

bool CFrameMgr::LockFrm()
{
	QueryPerformanceCounter(&m_CurTime);				//현재 시간 측정, OldTime 기록 이후 부터 현재 시간 간격으로 체크
	QueryPerformanceFrequency(&m_CpuClock);		//진동 수 체크

	///진동 수를 시간단위로 변환 (Delta Time : Second단위)
	m_fDeletaTime += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuClock.QuadPart;
	//진동 수를 시간단위로 변환 (Delta Time : Milli Second단위)
	//m_fDeletaTime += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / (m_CpuClock.QuadPart / 1000);

	m_OldTime = m_CurTime;

	if (m_fSecForFrm <= m_fDeletaTime)
	{
		++m_iFPS;
		m_fDeletaTime = 0.f;
		return true;
	}
	

	return false;
}

void CFrameMgr::RenderFrm()
{
	m_fpsTime += ENGINE::Get_DeltaTime();

	if (1.f < m_fpsTime)
	{
		swprintf_s(szFPS, L"FPS: %d", m_iFPS);
		m_iFPS = 0;
		m_fpsTime = 0.f;
	}

	SetWindowText(g_hWnd, szFPS);
}
