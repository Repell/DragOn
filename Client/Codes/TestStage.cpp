#include "stdafx.h"
#include "TestStage.h"

CTestStage::CTestStage(LPDIRECT3DDEVICE9 pDevice)
	: ENGINE::CScene(pDevice), m_pLoading(nullptr)
{
}

CTestStage::~CTestStage()
{
}

HRESULT CTestStage::Ready_Scene()
{
	RenderPipeLine_SetUp();
	Create_Light();

	FAILED_CHECK_RETURN(ENGINE::CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);

	return S_OK;
}

_int CTestStage::Update_Scene(_double TimeDelta)
{
	ENGINE::CScene::Update_Scene(TimeDelta);

	return NO_EVENT;
}

void CTestStage::Late_Update_Scene()
{
	ENGINE::CScene::Late_Update_Scene();
}

void CTestStage::Render_Scene()
{
}

HRESULT CTestStage::Add_Environment_Layer()
{
	ENGINE::CLayer* pObject_Layer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pObject_Layer, E_FAIL);

	ENGINE::CGameObject* pObject = nullptr;
	////////////INSERT GAME OBJECT////////////

	pObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Terrain", pObject);

	pObject = CTerrainSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Terrain_SkyBox", pObject);

	LoadForStaticDat(pObject_Layer);

	//////////////INSERT LAYER//////////////
	m_MapLayer.emplace(ENGINE::CLayer::ENVIRONMENT, pObject_Layer);

	return S_OK;
}

HRESULT CTestStage::Add_GameObject_Layer()
{
	ENGINE::CLayer* pObject_Layer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pObject_Layer, E_FAIL);

	ENGINE::CGameObject* pObject = nullptr;
	////////////INSERT GAME OBJECT////////////

	//Normal

	pObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Player", pObject);

	pObject = CSword::Create(m_pGraphicDev, 0);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Sword", pObject);

	//Rider

	//pObject = CMichael::Create(m_pGraphicDev, _vec3(40.f, 0.1f, 15.f));
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"Player_Michael", pObject);

	//pObject = CPlayer_Rider::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"Player_Rider", pObject);

	//pObject = CSpear::Create(m_pGraphicDev, 0);
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"Spear", pObject);

	//ETC

	pObject = CEffect_Tex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Effect", pObject);

	pObject = CShade::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Shade", pObject);

	pObject = CSnowman::Create(m_pGraphicDev, _vec3(40.f, 0.1f, 6.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Snowman", pObject);

	//pObject = CSnowman::Create(m_pGraphicDev, _vec3(45.f, 0.1f, 3.f));
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"Snowman", pObject);


	//////////////INSERT LAYER//////////////
	m_MapLayer.emplace(ENGINE::CLayer::OBJECT, pObject_Layer);
	return S_OK;
}

HRESULT CTestStage::Add_UI_Layer()
{
	ENGINE::CLayer* pObject_Layer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pObject_Layer, E_FAIL);

	ENGINE::CGameObject* pObject = nullptr;
	////////////INSERT GAME OBJECT////////////

	pObject = CDynamicCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"DynamicCamera", pObject);

	//////////////INSERT LAYER//////////////
	m_MapLayer.emplace(ENGINE::CLayer::UI, pObject_Layer);
	return S_OK;
}

void CTestStage::RenderPipeLine_SetUp()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 샘플링 
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void CTestStage::Create_Light()
{
	D3DLIGHT9 DirLight;
	ZeroMemory(&DirLight, sizeof(D3DLIGHT9));

	//방향성 조명
	DirLight.Type = D3DLIGHT_DIRECTIONAL;
	//rgba
	DirLight.Diffuse.a = 0.f;
	DirLight.Diffuse.r = 1.f;
	DirLight.Diffuse.g = 1.f;
	DirLight.Diffuse.b = 1.f;
	// 위치
	DirLight.Position = { 0.f, 25.f, 0.f };
	// 방향
	_vec3 vLightDir = { TERRAIN_VTX_X >> 2, -25.f, TERRAIN_VTX_Z >> 2 };
	D3DXVec3Normalize((_vec3*)&DirLight.Direction, &vLightDir);

	m_pGraphicDev->SetLight(0, &DirLight);
	m_pGraphicDev->LightEnable(0, TRUE);

}

void CTestStage::LoadForStaticDat(ENGINE::CLayer* pLayer)
{
	ENGINE::CGameObject* pObject = nullptr;

	HANDLE hFile = CreateFile(L"../../Data/Static.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_BOX("Load Static Dat");
		return;
	}

	DWORD dwByte = 0;
	DWORD dwBuff = 0;
	TCHAR* szKey = nullptr;


	while (true)	//static
	{
		ENGINE::MESH_INFO pMesh;

		ReadFile(hFile, &pMesh.iUnitType, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, &pMesh.wMeshType, sizeof(_ushort), &dwByte, nullptr);

		//Mesh_Tag
		ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
		szKey = new TCHAR[dwBuff];
		ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
		pMesh.pMeshTag = szKey;
		ENGINE::Safe_Delete_Array(szKey);

		//FileName
		ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
		szKey = new TCHAR[dwBuff];
		ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
		pMesh.pFileName = szKey;
		ENGINE::Safe_Delete_Array(szKey);

		//Pos, Rot, Scale
		_vec3 vTransform[3];
		ReadFile(hFile, &vTransform[0], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vTransform[1], sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vTransform[2], sizeof(_vec3), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		pObject = CStaticObj::Create_MeshObject(m_pGraphicDev, pMesh.pMeshTag, vTransform);
		NULL_CHECK_RETURN(pObject);
		pLayer->Add_GameObject(pMesh.pMeshTag, pObject);
	}

}

CTestStage * CTestStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTestStage* pInstance = new CTestStage(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CTestStage::Free()
{
	if(m_pLoading != nullptr)
		ENGINE::Safe_Release(m_pLoading);

	ENGINE::CScene::Free();
}
