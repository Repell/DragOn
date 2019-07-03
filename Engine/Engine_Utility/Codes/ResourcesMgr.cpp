#include "ResourcesMgr.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CResourcesMgr)

CResourcesMgr::CResourcesMgr(void)
	: m_pMapResources(nullptr)
{
}

CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT CResourcesMgr::Reserve_ContainerSize(const _ushort & wSize)
{
	if (nullptr != m_pMapResources)
		return E_FAIL;

	m_pMapResources = new MAP_RESOURCES[wSize];

	m_iContainerSize = wSize;

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pBufferTag, BUFFERID eID, const WORD vtxX, const WORD vtxY, const WORD Intv, const WORD wDetail)
{
	if (nullptr == m_pMapResources)
	{
		ERR_BOX("Resources Container not Reserved!");
		return E_FAIL;
	}

	CResourceses* pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;
	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;
	case BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;
	case BUFFER_TRITEX:
		pResources = CTriTex::Create(pGraphicDev);
		break;
	case BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, vtxX, vtxY, Intv, wDetail);
		break;
	case BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;
	case BUFFER_SKYBOX:
		pResources = CSkyBox::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pTextureTag, TEXTURETYPE eType, const _tchar * pPath, const _uint & iCnt)
{

	if (nullptr == m_pMapResources)
	{
		ERR_BOX("Resoruces Container Reserver Err!");
		return E_FAIL;
	}

	CResourceses* pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pPath, eType, iCnt);
	if (nullptr == pResources)
		return E_FAIL;

	m_pMapResources[wContainerIdx].emplace(pTextureTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName)
{
	if (nullptr == m_pMapResources)
	{
		ERR_BOX("Resources Container not Reserved!");
		return E_FAIL;
	}

	CResourceses* pResource = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResource)
		return E_FAIL;

	switch (eType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	case MESH_NAVI:
		break;
	}
	
	if (nullptr == pResource)
		return E_FAIL;

	m_pMapResources[wContainerIdx].emplace(pMeshTag, pResource);

	return S_OK;
}

void CResourcesMgr::Render_Buffers(const _ushort & wContainerIdx, const _tchar* pBufferTag)
{
	CResourceses* pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr == pResources)
		return;

	dynamic_cast<CViBuffer*>(pResources)->Render_Buffer();
}

void CResourcesMgr::Render_Texture(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint iCnt)
{
	CResourceses* pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr == pResources)
		return;

	dynamic_cast<CTexture*>(pResources)->Render_Texture(iCnt);
}

CResourceses * CResourcesMgr::Find_Resources(const _ushort wContainerIdx, const _tchar * pResourcesTag)
{
	auto iter = find_if(m_pMapResources[wContainerIdx].begin(), m_pMapResources[wContainerIdx].end(), [=](auto& pair)->_bool
	{
		if (0 == lstrcmpW(pair.first, pResourcesTag))
			return true;

		return false;
	});

	if (iter == m_pMapResources[wContainerIdx].end())
		return nullptr;

	return iter->second;
}

CResourceses * CResourcesMgr::Clone_Resources(const _ushort & wContainIdx,
	const _tchar * pResourceTag)
{
	if (nullptr == m_pMapResources)
		return nullptr;

	CResourceses* pInstance = Find_Resources(wContainIdx, pResourceTag);

	return pInstance->Clone();
}

void CResourcesMgr::Release_Terrain(const _ushort wContainerIdx, const _tchar * pResourceTag)
{
	//auto& iter = Find_Resources(wContainerIdx, pResourceTag);
	CResourceses* pInstance = Find_Resources(wContainerIdx, pResourceTag);

	if (pInstance == nullptr)
		return;
	
	Safe_Release(pInstance);
	m_pMapResources[wContainerIdx].erase(pResourceTag);

}

void CResourcesMgr::Free(void)
{
	for (size_t i = 0; i < m_iContainerSize; ++i)
	{
		for_each(m_pMapResources[i].begin(), m_pMapResources[i].end(), CDeleteMap());
		m_pMapResources[i].clear();
	}
	Safe_Delete_Array(m_pMapResources);
}
