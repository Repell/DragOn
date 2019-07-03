#include "Export_Utility.h"

inline CManagement * Get_Management()
{
	return CManagement::Get_Instance();
}

inline HRESULT SetUp_CurrentScene(CScene * pCurrentScene)
{
	return CManagement::Get_Instance()->SetUp_CurrentScene(pCurrentScene);
}

inline HRESULT Create_Management(CManagement ** ppManagement)
{
	CManagement* pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;
}

inline CRenderer * Get_Renderer()
{
	return CRenderer::Get_Instance();
}

inline HRESULT Reserve_ContainerSize(const _ushort & wSize)
{
	return CResourcesMgr::Get_Instance()->Reserve_ContainerSize(wSize);
}

inline HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pBufferTag, BUFFERID eID, const WORD vtxX, const WORD vtxY, const WORD Intv, const WORD wDetail)
{
	return CResourcesMgr::Get_Instance()->Ready_Buffers(pGraphicDev, wContainerIdx, pBufferTag, eID, vtxX, vtxY, Intv, wDetail);
}

inline HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pTextureTag, TEXTURETYPE eType, const _tchar * pPath, const _uint & iCnt)
{
	return CResourcesMgr::Get_Instance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}

inline void Render_Buffers(const _ushort & wContainerIdx, const _tchar * pBufferTag)
{
	return CResourcesMgr::Get_Instance()->Render_Buffers(wContainerIdx, pBufferTag);
}

inline void Render_Texture(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint iCnt)
{
	return CResourcesMgr::Get_Instance()->Render_Texture(wContainerIdx, pTextureTag, iCnt);
}

inline HRESULT Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName)
{
	return CResourcesMgr::Get_Instance()->Ready_Meshes(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}

inline CResourceses * Clone_Resources(const _ushort & wContainIdx, const _tchar * pResourceTag)
{
	return CResourcesMgr::Get_Instance()->Clone_Resources(wContainIdx, pResourceTag);
}

inline void Release_Terrain(const _ushort wContainerIdx, const _tchar * pResourceTag)
{
	return CResourcesMgr::Get_Instance()->Release_Terrain(wContainerIdx, pResourceTag);
}

inline void Release_Utility()
{
	CRenderer::Get_Instance()->Destroy_Instance();
	CResourcesMgr::Get_Instance()->Destroy_Instance();
	CManagement::Get_Instance()->Destroy_Instance();
}



