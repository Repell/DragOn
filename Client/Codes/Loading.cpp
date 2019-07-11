#include "stdafx.h"
#include "Loading.h"

#include "Export_Utility.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev),
	m_bFisihed(FALSE)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * MID_STR);
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{

	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);
	m_eID = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForLogo()
{
	lstrcpy(m_szLoading, L" [ Mesh Loading ]"); //////////////////////////////////////////

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Player",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/PlayerXfile/",
		L"Player.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Sword",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Sword/",
		L"Sword.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Navi",
		ENGINE::MESH_NAVI, NULL, NULL),
		E_FAIL);

	//FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_LOGO,
	//	L"Pl_Sword",
	//	ENGINE::MESH_DYNAMIC,
	//	L"../Bin/Resources/Mesh/DynamicMesh/Player/",
	//	L"Player_Sword.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_LOGO,
	//	L"Pl_Spear",
	//	ENGINE::MESH_DYNAMIC,
	//	L"../Bin/Resources/Mesh/DynamicMesh/Player/",
	//	L"Player_Spear.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_LOGO,
	//	L"Pl_Trans",
	//	ENGINE::MESH_DYNAMIC,
	//	L"../Bin/Resources/Mesh/DynamicMesh/Player/",
	//	L"Player_Transform.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_LOGO,
	//	L"Pl_Ride",
	//	ENGINE::MESH_DYNAMIC,
	//	L"../Bin/Resources/Mesh/DynamicMesh/Player_Ride/",
	//	L"Player_Ride.X"),
	//	E_FAIL);

	lstrcpy(m_szLoading, L" [ Texture Loading ]");/////////////////////////////////////////
	
	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Explosion", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Explosion/Explosion%d.png", 90), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Shade", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Etc/Shade.png"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Aura", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Etc/Aura0.tga"), E_FAIL);

	lstrcpy(m_szLoading, L" === Load Complete! ===");

	m_bFisihed = TRUE;

	return 0;
}

_uint CLoading::Loading_ForStage()
{
	lstrcpy(m_szLoading, L" [ Texture Loading ]");/////////////////////////////////////////



	return 0;
}

_uint CLoading::Load_DefaultResource()
{
	ENGINE::Reserve_ContainerSize(RESOURCE_END);

	lstrcpy(m_szLoading, L" [ Buffer Loading ]");////////////////////////////////

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_TriCol", ENGINE::BUFFER_TRICOL), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_RcCol", ENGINE::BUFFER_RCCOL), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_RcTex", ENGINE::BUFFER_RCTEX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_CubeTex", ENGINE::BUFFER_CUBETEX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_SkyBox", ENGINE::BUFFER_SKYBOX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_Terrain", ENGINE::BUFFER_TERRAINTEX, TERRAIN_VTX_X, TERRAIN_VTX_Z, TERRAIN_INTRV, 20), E_FAIL);

	lstrcpy(m_szLoading, L" [ Texture Loading ]");///////////////////////////////////

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_PRELOAD, L"Texture_SkyBox", ENGINE::TEX_CUBE,
		L"../Bin/Resources/Texture/SkyBox/SkyBox%d.dds"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_PRELOAD, L"Texture_Terrain", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Terrain/Terrain%d.png"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_PRELOAD, L"Texture_Title", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Title/Title%d.png", 7), E_FAIL);

	lstrcpy(m_szLoading, L" [ Load Complete! ]");////////////////////////////////////////

	m_bFisihed = TRUE;

	return 0;
}

_uint CALLBACK CLoading::Thread_Main(void * pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_PRELOAD:
		iFlag = pLoading->Load_DefaultResource();
		break;

	case LOADING_LOGO:
		iFlag = pLoading->Loading_ForLogo();
		break;

	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}
	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		ERR_BOX("Loading Create Failed");
		ENGINE::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	ENGINE::Safe_Release(m_pGraphicDev);
}
