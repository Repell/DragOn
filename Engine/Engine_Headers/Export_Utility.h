#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Define.h"
#include "Management.h"
#include "Renderer.h"
#include "Transform.h"
#include "Calculator.h"
//
#include "ResourcesMgr.h"

NS_BEGIN(ENGINE)

//Management
inline CManagement* Get_Management();
inline HRESULT SetUp_CurrentScene(CScene* pCurrentScene);
inline HRESULT Create_Management(CManagement** ppManagement);

//Renderer
inline CRenderer* Get_Renderer();

// ResourcesMgr
// Get
// Set
// Public
inline HRESULT Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pBufferTag,
	BUFFERID eID, const WORD vtxX = 0, const WORD vtxY = 0, const WORD Intv = 0, const WORD wDetail = 1);
inline HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt = 1);
inline void Render_Buffers(const _ushort& wContainerIdx, const _tchar* pBufferTag);
inline void Render_Texture(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint iCnt = 0);
inline 	HRESULT Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName);

inline CResourceses * Clone_Resources(const _ushort & wContainIdx, const _tchar * pResourceTag);

inline void Release_Terrain(const _ushort wContainerIdx, const _tchar * pResourceTag);



//Release
inline void Release_Utility();

#include "Export_Utility.inl"

NS_END

#endif // Export_Utility_h__
