#include "stdafx.h"
#include "BossStage.h"

#include "Export_Function.h"

CBossStage::CBossStage(LPDIRECT3DDEVICE9 pDevice)
	: ENGINE::CScene(pDevice), m_pLoading(nullptr)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene()
{
	RenderPipeLine_SetUp();
	Create_Light();

	FAILED_CHECK_RETURN(ENGINE::CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);

	return S_OK;
}

_int CBossStage::Update_Scene(_double TimeDelta)
{
	ENGINE::CScene::Update_Scene(TimeDelta);

	return NO_EVENT;
}

void CBossStage::Late_Update_Scene()
{
	ENGINE::CScene::Late_Update_Scene();

}

void CBossStage::Render_Scene()
{
}

HRESULT CBossStage::Add_Environment_Layer()
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

HRESULT CBossStage::Add_GameObject_Layer()
{
	ENGINE::CLayer* pObject_Layer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pObject_Layer, E_FAIL);

	ENGINE::CGameObject* pObject = nullptr;
	////////////INSERT GAME OBJECT////////////

	//Sword Player
	pObject = CNewPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Player", pObject);

	//Rider
	pObject = CMichael::Create(m_pGraphicDev, _vec3(128.f, 16.f, 128.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Player_Dragon", pObject);

	pObject = CPlayer_Rider::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Player_Rider", pObject);

	//pObject = CSpear::Create(m_pGraphicDev, 0);
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"Spear", pObject);

	//ETC
	pObject = CEffect_Tex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Effect", pObject);

	//pObject = CShade::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"Shade", pObject);

	//Monster

	pObject = CBlue::Create(m_pGraphicDev, _vec3(32.f, 0.f, 32.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Boss_Blue", pObject);

	pObject = CBlue::Create(m_pGraphicDev, _vec3(224.f, 0.f, 32.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Boss_Blue", pObject);

	pObject = CBlue::Create(m_pGraphicDev, _vec3(32.f, 0.f, 224.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Boss_Blue", pObject);

	pObject = CBlue::Create(m_pGraphicDev, _vec3(224.f, 0.f, 224.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Boss_Blue", pObject);

	pObject = CTroll::Create(m_pGraphicDev, _vec3(127.f, 0.1f, 12.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Troll", pObject);

	pObject = CTroll::Create(m_pGraphicDev, _vec3(133.f, 0.1f, 12.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Troll", pObject);
		
	for (int i = 0; i < 5; ++i)
	{
		pObject = CEnemy_Swordman::Create(m_pGraphicDev, _vec3(128 + i, 0.1f, 16.f));
		NULL_CHECK_RETURN(pObject, E_FAIL);
		pObject_Layer->Add_GameObject(L"Enemy_Swordman", pObject);
	}

	for (int i = 0; i < 5; ++i)
	{
		pObject = CEnemy_Spearman::Create(m_pGraphicDev, _vec3(128 + i, 0.1f, 14.f));
		NULL_CHECK_RETURN(pObject, E_FAIL);
		pObject_Layer->Add_GameObject(L"Enemy_Spearman", pObject);
	}

	for (int i = 0; i < 5; ++i)
	{
		pObject = CEnemy_Shieldman::Create(m_pGraphicDev, _vec3(128 + i, 0.1f, 18.f));
		NULL_CHECK_RETURN(pObject, E_FAIL);
		pObject_Layer->Add_GameObject(L"Enemy_Shieldman", pObject);

	}

	for (int i = 0; i < 3; ++i)
	{
		pObject = CEnemy_Bowman::Create(m_pGraphicDev, _vec3(129 + i, 0.1f, 12.f));
		NULL_CHECK_RETURN(pObject, E_FAIL);
		pObject_Layer->Add_GameObject(L"Enemy_Bowman", pObject);

	}

	pObject = CKeroberos::Create(m_pGraphicDev, _vec3(129, 0.1f, 9.f));
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Boss_Keroberos", pObject);


	//////////////INSERT LAYER//////////////
	m_MapLayer.emplace(ENGINE::CLayer::OBJECT, pObject_Layer);
	return S_OK;
}

HRESULT CBossStage::Add_UI_Layer()
{
	ENGINE::CLayer* pObject_Layer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pObject_Layer, E_FAIL);

	ENGINE::CGameObject* pObject = nullptr;
	////////////INSERT GAME OBJECT////////////

	//pObject = CDynamicCamera::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"DynamicCamera", pObject);


	//ENGINE::CTransform* pTransfom =(ENGINE::CTransform*)Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC);
	//pObject = CStaticCamera::Create(m_pGraphicDev, pTransfom, D3DXToRadian(60.f), _float(WINCX) / _float(WINCY), 0.1f, 1000.f);
	//NULL_CHECK_RETURN(pObject, E_FAIL);
	//pObject_Layer->Add_GameObject(L"StaticCamera", pObject);

	//////////////INSERT LAYER//////////////
	m_MapLayer.emplace(ENGINE::CLayer::UI, pObject_Layer);
	return S_OK;
}

void CBossStage::RenderPipeLine_SetUp()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 샘플링 
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void CBossStage::Create_Light()
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

void CBossStage::LoadForStaticDat(ENGINE::CLayer* pLayer)
{
	ENGINE::CGameObject* pObject = nullptr;

	HANDLE hFile = CreateFile(L"../../Data/Boss_Static.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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

		if (pMesh.pMeshTag == L"Mesh_Structure")
		{
			pObject = CStatic_Tower::Create_MeshObject(m_pGraphicDev, pMesh.pMeshTag, vTransform);
			NULL_CHECK_RETURN(pObject);
			pLayer->Add_GameObject(pMesh.pMeshTag, pObject);
		}
		else if (pMesh.pMeshTag == L"Mesh_Relic")
		{
			pObject = CStatic_Relic::Create_MeshObject(m_pGraphicDev, pMesh.pMeshTag, vTransform);
			NULL_CHECK_RETURN(pObject);
			pLayer->Add_GameObject(pMesh.pMeshTag, pObject);
		}
		else
		{
			pObject = CStaticObj::Create_MeshObject(m_pGraphicDev, pMesh.pMeshTag, vTransform);
			NULL_CHECK_RETURN(pObject);
			pLayer->Add_GameObject(pMesh.pMeshTag, pObject);
		}

	}

}

CBossStage * CBossStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBossStage* pInstance = new CBossStage(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CBossStage::Free()
{
	if (m_pLoading != nullptr)
		ENGINE::Safe_Release(m_pLoading);

	ENGINE::CScene::Free();
}
