#include "stdafx.h"
#include "ToolRender.h"

CToolRender::CToolRender()
	: m_pGraphicDev(ENGINE::Get_GraphicDev())
{
	m_pGraphicDev->AddRef();
}

CToolRender::~CToolRender()
{
	Free();
}

void CToolRender::Update_Render(const _float& fTimeDelta)
{
	MAP_RENDER::iterator iter_Begin = m_mapRender.begin();
	MAP_RENDER::iterator iter_End = m_mapRender.end();
	list<ENGINE::CGameObject*>::iterator iter = {};

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		iter = iter_Begin->second.begin();
		for (; iter != iter_Begin->second.end();)
		{
			int iEvent = (*iter)->Update_Object(fTimeDelta);

			if (999 == iEvent)
			{
				ENGINE::Safe_Release(*iter);
				iter = iter_Begin->second.erase(iter);
			}
			else
				++iter;
		}
	}

}

void CToolRender::Late_Update_Render()
{
	MAP_RENDER::iterator iter_Begin = m_mapRender.begin();
	MAP_RENDER::iterator iter_End = m_mapRender.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		for (auto& pObj : iter_Begin->second)
			pObj->Late_Update_Object();
	}

}

void CToolRender::Render()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	MAP_RENDER::iterator iter_Begin = m_mapRender.begin();
	MAP_RENDER::iterator iter_End = m_mapRender.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		for (auto& pObj : iter_Begin->second)
			pObj->Render_Object();
	}
}

HRESULT CToolRender::Ready_ToolRender(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pDevice = pGraphicDev;
	m_pDevice->AddRef();

	Ready_Resources(m_pDevice, RESOURCE_TOOL);
	Ready_GameObject();

	return S_OK;
}

void CToolRender::Free()
{
	ENGINE::Safe_Release(m_pDevice);
	ENGINE::Safe_Release(m_pGraphicDev);

	MAP_RENDER::iterator iter_Begin = m_mapRender.begin();
	MAP_RENDER::iterator iter_End = m_mapRender.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		for_each(iter_Begin->second.begin(), iter_Begin->second.end(), ENGINE::CDeleteObj());

		iter_Begin->second.clear();
	}
	m_mapRender.clear();
}

CToolRender * CToolRender::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolRender* pInstance = new CToolRender;
	if (FAILED(pInstance->Ready_ToolRender(pGraphicDev)))
	{
		ENGINE::Safe_Delete(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CToolRender::Ready_Resources(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCED eID)
{
	
	ENGINE::Reserve_ContainerSize(RESOURCE_END);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_TriCol", ENGINE::BUFFER_TRICOL), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_TriTex", ENGINE::BUFFER_TRITEX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_RcCol", ENGINE::BUFFER_RCCOL), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_RcTex", ENGINE::BUFFER_RCTEX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_CubeTex", ENGINE::BUFFER_CUBETEX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_SkyBox", ENGINE::BUFFER_SKYBOX), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(pGraphicDev,
		RESOURCE_TOOL, L"Buffer_Terrain", ENGINE::BUFFER_TERRAINTEX,
		TERRAIN_VTX_X, TERRAIN_VTX_Z, TERRAIN_INTRV, 1), E_FAIL);

	vector<_int> vecTemp;
	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(pGraphicDev, RESOURCE_TOOL, L"Texture_Terrain", ENGINE::TEX_NORMAL,
		L"../../Client/Bin/Resources/Texture/Terrain/Terrain%d.png"), E_FAIL);

	vecTemp.emplace_back(0);
	m_MapTexture.emplace(L"Texture_Terrain", vecTemp);
	vecTemp.clear();

	//FAILED_CHECK_RETURN(ENGINE::Ready_Texture(pGraphicDev, RESOURCE_TOOL, L"Texture_Logo", ENGINE::TEX_NORMAL,
	//	L"../../Client/Bin//Resources/Texture/LogoBack/LogoBack_%d.png", 98), E_FAIL);

	//for (int i = 0; i < 98; ++i)
	//	vecTemp.emplace_back(i);

	//m_MapTexture.emplace(L"Texture_Logo", vecTemp);
	//vecTemp.clear();

	//FAILED_CHECK_RETURN(ENGINE::Ready_Texture(pGraphicDev, RESOURCE_TOOL, L"Texture_Player", ENGINE::TEX_NORMAL,
	//	L"../../Client/Bin/Resources/Texture/Player%d.png", 3), E_FAIL);

	//for (int i = 0; i < 3; ++i)
	//	vecTemp.emplace_back(i);

	//m_MapTexture.emplace(L"Texture_Player", vecTemp);
	//vecTemp.clear();

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(pGraphicDev, RESOURCE_TOOL, L"Texture_SkyBox", ENGINE::TEX_CUBE,
		L"../../Client/Bin/Resources/Texture/SkyBox/SkyBox%d.dds"), E_FAIL);

	FAILED_CHECK_RETURN(ENGINE::Ready_Texture(pGraphicDev, RESOURCE_TOOL, L"Texture_ColorRect", ENGINE::TEX_NORMAL,
		L"../../Client/Bin/Resources/Texture/NaviMesh/ColorRect.png"), E_FAIL);


	////ColorRect Texture
	//FAILED_CHECK_RETURN(Ready_Texture(pGraphicDev,	RESOURCE_TOOL, 	L"Texture_ColorRect", ENGINE::TEX_NORMAL,
	//	L"../../Client/Bin/Resources/Texture/ColorRect%d.png", 3), E_FAIL);

	//for (int i = 0; i < 3; ++i)
	//	vecTemp.emplace_back(i);

	//m_MapTexture.emplace(L"Texture_ColorRect", vecTemp);
	//vecTemp.clear();
	

	return S_OK;
}

HRESULT CToolRender::Ready_GameObject()
{
	list<ENGINE::CGameObject*> pTempList;

	ENGINE::CGameObject* pObject = nullptr;
	///////////////////////////////////////

	//DynamicCamera
	pObject = CDynamicCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pObject, E_FAIL);

	pTempList.emplace_back(pObject);
	m_mapRender.emplace(L"DynamicCam", pTempList);
	pTempList.clear();

	//Terrain
	pObject = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(pObject, E_FAIL);

	pTempList.emplace_back(pObject);
	m_mapRender.emplace(L"Terrain", pTempList);
	pTempList.clear();

	//MouseCtn
	//pObject = CMouseCtn::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObject, E_FAIL);

	//pTempList.emplace_back(pObject);
	//m_mapRender.emplace(L"MouseCtn", pTempList);
	//pTempList.clear();

	//NaviMaker
	pObject = CNaviMaker::Create(m_pDevice);
	NULL_CHECK_RETURN(pObject, E_FAIL);

	pTempList.emplace_back(pObject);
	m_mapRender.emplace(L"NaviMaker", pTempList);
	pTempList.clear();

	////Mesh Object
	//pObject = CTestObj::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObject, E_FAIL);

	//pTempList.emplace_back(pObject);
	m_mapRender.emplace(L"Static_MeshObject", pTempList);
	m_mapRender.emplace(L"Dynamic_MeshObject", pTempList);
	pTempList.clear();

	return S_OK;
}

HRESULT CToolRender::Add_GameObject(wstring strMesh, wstring typeKey, _vec3 vTransform[])
{
	
	ENGINE::CGameObject* pObject = nullptr;
	///////////////////////////////////////

	//Mesh
	pObject = CTestObj::Create_MeshObject(m_pDevice, strMesh, vTransform, CValueMgr::bMeshState);
	NULL_CHECK_RETURN(pObject, E_FAIL);

	

	m_mapRender[typeKey].emplace_back(pObject);

	return S_OK;
}

HRESULT CToolRender::Delete_GameObject(wstring typeKey)
{

	for_each(m_mapRender[typeKey].begin(), m_mapRender[typeKey].end(), [](auto& MyPair)
	{
		ENGINE::Safe_Release(MyPair);
	});

	m_mapRender[typeKey].clear();

	return S_OK;
}

HRESULT CToolRender::Create_NewTerrain(const WORD vtx_X, const WORD vtx_Z, const WORD Intv, const WORD Detail)
{
	//Resource
	FAILED_CHECK_RETURN(ENGINE::Ready_Buffers(m_pDevice,
		RESOURCE_TOOL, L"Buffer_Terrain", ENGINE::BUFFER_TERRAINTEX,
		vtx_X, vtx_Z, Intv, Detail), E_FAIL);

	return S_OK;
}

void CToolRender::Set_TextureString()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pForm = dynamic_cast<CMyForm*>(pMain->Get_MainWnd().GetPane(0, 1));

	for (auto iter : m_MapTexture)
	{
		for (auto iter2 : iter.second)
		{
			pForm->m_pTapTerrain.Add_TextureString(iter.first, iter2);
		}
	}

}

list<ENGINE::CGameObject*>& CToolRender::Get_RenderList(wstring wstrKey)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_mapRender[wstrKey];
}

