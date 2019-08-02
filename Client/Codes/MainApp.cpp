#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	//GraphicDevice
	FAILED_CHECK_RETURN(SetUp_GraphicDevice(&m_pGraphicDev), E_FAIL);
	//Etc Managers
	FAILED_CHECK_RETURN(SetUp_SystemManagers(), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_PreLoad(m_pGraphicDev, RESOURCE_PRELOAD),E_FAIL);
	ENGINE::Safe_Release(m_pLoading);

	FAILED_CHECK_RETURN(Ready_NextScene(m_pGraphicDev, &m_pManagement), E_FAIL);

	return S_OK;
}

_int CMainApp::Update_MainApp()
{
	ENGINE::Update_Time();

	if (nullptr == m_pDeviceInstance)
		return END_EVENT;

	//KeyMgr
	ENGINE::KeyInput();
	ENGINE::Set_InputDev();
	 
	//Management
	m_pManagement->Update_Scene(ENGINE::Get_DeltaTime());

	return NO_EVENT;
}

void CMainApp::Late_Update_MainApp()
{
	if (nullptr == m_pDeviceInstance)
		return;

	m_pManagement->Late_Update_Scene();

}

void CMainApp::Render_MainApp(void)
{
	if (nullptr == m_pDeviceInstance)
		return;

	m_pDeviceInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pManagement->Render_Scene(m_pGraphicDev);
	
	m_pDeviceInstance->Render_End();
}

HRESULT CMainApp::SetUp_GraphicDevice(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(ENGINE::Ready_GraphicDev(g_hWnd, ENGINE::CGraphicDev::MODE_WIN, WINCX, WINCY, &m_pDeviceInstance), E_FAIL);
	m_pDeviceInstance->AddRef();

	*ppGraphicDev = m_pDeviceInstance->Get_Device();
	(*ppGraphicDev)->AddRef();
	
	return S_OK;
}

HRESULT CMainApp::SetUp_SystemManagers()
{
	//Time & Frame
	ENGINE::Init_Time();

	//KeyManagers
	ENGINE::Ready_InputDev(g_hInst, g_hWnd);	//Direct Input(Mouse)
	ENGINE::KeyInput();	//KeyManager

	//Default Font
	ENGINE::Ready_Font(m_pGraphicDev, L"Sp", L"½ºÇÃ·¡2K", 22, 24, FW_NORMAL);

	return S_OK;
}

HRESULT CMainApp::Ready_PreLoad(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCED eID)
{
	m_pLoading = CLoading::Create(pGraphicDev, LOADING_PRELOAD);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_NextScene(LPDIRECT3DDEVICE9 pDevice, ENGINE::CManagement ** ppManagement)
{
	ENGINE::CScene* pScene = nullptr;
	
	FAILED_CHECK_RETURN(ENGINE::Create_Management(pDevice, ppManagement), E_FAIL);

	pScene = CLogo::Create(pDevice);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN(ENGINE::SetUp_CurrentScene(pScene), E_FAIL);

	return S_OK;
}


CMainApp * CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free()
{
	ENGINE::Safe_Release(m_pGraphicDev);
	ENGINE::Safe_Release(m_pManagement);
	ENGINE::Safe_Release(m_pDeviceInstance);

	ENGINE::Release_Utility();
	ENGINE::Release_System();
}
