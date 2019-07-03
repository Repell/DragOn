#ifndef MainApp_h__
#define MainApp_h__

#include "Base.h"
#include "Loading.h"

#include "Logo.h"

namespace ENGINE
{
	class CGraphicDev;
	class CManagement;
}

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_MainApp(void);
	_int Update_MainApp();
	void Late_Update_MainApp();
	void Render_MainApp(void);

private:
	HRESULT SetUp_GraphicDevice(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT SetUp_SystemManagers();
	HRESULT Ready_PreLoad(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCED eID);
	HRESULT Ready_NextScene(LPDIRECT3DDEVICE9 pDevice, ENGINE::CManagement** ppManagement);

private:  

	ENGINE::CGraphicDev*	m_pDeviceInstance;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	ENGINE::CManagement*	m_pManagement;
	CLoading* m_pLoading;

public:
	static CMainApp* Create();
	void Free();


};

#endif // MainApp_h__
