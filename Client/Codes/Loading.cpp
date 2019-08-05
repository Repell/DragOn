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
		L"../Bin/Resources/Mesh/DynamicMesh/Player_Sword/",
		L"Player_Sword.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Rider",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Player_Ride/",
		L"Player_Ride.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Michael",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Michael/",
		L"Michael.X"),
		E_FAIL);

	//////////////////////////////////////

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Troll",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Troll/",
		L"Troll.X"),
		E_FAIL);		//Troll

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Swordman",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Soldier_Sword/",
		L"Soldier_Sword_Binary.X"),
		E_FAIL);		//Enemy_Swordman

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Spearman",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Soldier_Spear/",
		L"Soldier_Spear_Binary.X"),
		E_FAIL);		//Enemy_Spearman

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Shieldman",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Soldier_Shield/",
		L"Soldier_Shield_Binary.X"),
		E_FAIL);		//Enemy_Shieldman

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Bowman",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Soldier_Bow/",
		L"Soldier_Bow_Binary.X"),
		E_FAIL);		//Enemy_Bowman

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Boss_Blue",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Blue/",
		L"BLUE_BINARY.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Boss_NudeBlue",
		ENGINE::MESH_DYNAMIC,
		L"../Bin/Resources/Mesh/DynamicMesh/Blue/",
		L"BLUE_NUDEBINARY.X"),
		E_FAIL);

	///////////////// STATIC ////////////////////////

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Sword",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Sword/",
		L"Sword.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Sword",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Enemy_Sword/",
		L"Enemy_Sword.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Spear",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Enemy_Spear/",
		L"Enemy_Spear.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Shield",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Enemy_Shield/",
		L"Enemy_Shield.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Bow",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Enemy_Bow/",
		L"Enemy_Bow.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Enemy_Arrow",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Enemy_Arrow/",
		L"Enemy_Arrow.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Spear",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Spear/",
		L"Spear_Player.X"),
		E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Fireball",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/DragonFireBall/",
		L"FireBall.X"),
		E_FAIL);


	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Structure",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Stage2/",
		L"Structure.X"),
		E_FAIL);	//Tower

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Interior",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Stage2/",
		L"Interior.X"),
		E_FAIL);	//Room

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Relic",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/Relic/",
		L"Relic.X"),
		E_FAIL);	//Relic

	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_3D_BG",
		ENGINE::MESH_STATIC,
		L"../Bin/Resources/Mesh/StaticMesh/BG/",
		L"3D_BG.X"),
		E_FAIL);

	//Navi Mesh
	FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Mesh_Navi",
		ENGINE::MESH_NAVI, L"../../Data/",
		L"Line.dat"),
		E_FAIL);

	//FAILED_CHECK_RETURN(ENGINE::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_LOGO,
	//	L"Pl_Spear",
	//	ENGINE::MESH_DYNAMIC,
	//	L"../Bin/Resources/Mesh/DynamicMesh/Player/",
	//	L"Player_Spear.X"),
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

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Torch", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Torch/Torch_0%d.png", 32), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Shade", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Etc/Shade.png"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Aura", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Etc/Aura0.tga"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_HolyCircle", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/HolyCircle/HolyCircle.tga"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Fireball", ENGINE::TEX_NORMAL,
		L"../Bin/Resources/Texture/Fireball/Fireball_0%d.png", 15), E_FAIL);

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

	LoadForTerrainDat();
	//FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
	//	RESOURCE_PRELOAD, L"Buffer_Terrain", ENGINE::BUFFER_TERRAINTEX, TERRAIN_VTX_X, TERRAIN_VTX_Z, TERRAIN_INTRV, 20), E_FAIL);


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

HRESULT CLoading::LoadForTerrainDat()
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain2.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_BOX("Load Terrain Dat Failed!");
		return E_FAIL;
	}

	DWORD dwByte = 0;
	DWORD dwBuff = 0;
	TCHAR* szKey = nullptr;
	_vec3 vPos, vRot;
	_uint iTex = 0;

	WORD iSizeX, iSizeZ, iInterval, iDetail;

	ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
	szKey = new TCHAR[dwBuff];
	ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);

	ReadFile(hFile, &iSizeX, sizeof(WORD), &dwByte, nullptr);
	ReadFile(hFile, &iSizeZ, sizeof(WORD), &dwByte, nullptr);
	ReadFile(hFile, &iInterval, sizeof(WORD), &dwByte, nullptr);
	ReadFile(hFile, &iDetail, sizeof(WORD), &dwByte, nullptr);
	ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hFile, &vRot, sizeof(_vec3), &dwByte, nullptr);
	ReadFile(hFile, &iTex, sizeof(_uint), &dwByte, nullptr);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pGraphicDev,
		RESOURCE_PRELOAD, L"Buffer_Terrain", ENGINE::BUFFER_TERRAINTEX, iSizeX, iSizeZ, iInterval, iDetail), E_FAIL);

	ENGINE::Safe_Delete_Array(szKey);

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
