#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "Texture.h"
#include "TriCol.h"
#include "TriTex.h"
#include "RcCol.h"
#include "RcTex.h"
#include "CubeTex.h"
#include "SkyBox.h"
#include "TerrainTex.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CResourcesMgr :
	public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT Reserve_ContainerSize(const _ushort& wSize);

	HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ushort& wContainerIdx,
		const _tchar* pBufferTag,
		BUFFERID eID, const WORD vtxX = 0, const WORD vtxY = 0, const WORD Intv = 0, const WORD wDetail = 1);

	HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ushort& wContainerIdx,
		const _tchar* pTextureTag,
		TEXTURETYPE eType,
		const _tchar* pPath,
		const _uint& iCnt = 1);

	HRESULT Ready_Meshes(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ushort& wContainerIdx,
		const _tchar* pMeshTag,
		MESHTYPE eType,
		const _tchar* pFilePath,
		const _tchar* pFileName);

	void Render_Buffers(const _ushort& wContainerIdx,
		const _tchar* pBufferTag);

	void Render_Texture(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint iCnt = 0);

private:
	CResourceses* Find_Resources(const _ushort wContainerIdx,
		const _tchar* pResourcesTag);

public:
	CResourceses* Clone_Resources(const _ushort& wContainIdx, const _tchar* pResourceTag);
	void Release_Terrain(const _ushort wContainerIdx, const _tchar * pResourceTag);

private:
	map<const _tchar*, CResourceses*>*		m_pMapResources;
	typedef map<const _tchar*, CResourceses*> MAP_RESOURCES;

	_ulong m_iContainerSize;

public:
	virtual void Free(void);

};

NS_END

#endif // ResourcesMgr_h__
