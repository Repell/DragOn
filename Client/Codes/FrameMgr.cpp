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
	m_fSecForFrm = 1 / Frm;		//�ʴ� FRAME ��� �� ���� (1 / 60 ~ 144);

	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CpuClock);	// �ʴ� ������ 1 / 1000000 

}

bool CFrameMgr::LockFrm()
{
	QueryPerformanceCounter(&m_CurTime);				//���� �ð� ����, OldTime ��� ���� ���� ���� �ð� �������� üũ
	QueryPerformanceFrequency(&m_CpuClock);		//���� �� üũ

	///���� ���� �ð������� ��ȯ (Delta Time : Second����)
	m_fDeletaTime += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CpuClock.QuadPart;
	//���� ���� �ð������� ��ȯ (Delta Time : Milli Second����)
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
