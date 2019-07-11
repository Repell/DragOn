#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CStaticMesh :
	public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh();

public: //Get
	_ulong		Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong		Get_Stride(void) const { return m_dwStride; }
	_vec3*	Get_VtxMeshPos(void) { return m_pVtxPos; }
	LPD3DXMESH Get_Mesh() { return m_pMesh; }

public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Render_Meshes();

private:
	LPD3DXMESH						m_pOriginMesh;
	LPD3DXMESH						m_pMesh;
	LPD3DXBUFFER					m_pAdjacency;		//인접 정보 저장 버퍼
	LPD3DXBUFFER					m_pSubSetBuffer;
	D3DXMATERIAL*				m_pSubSet;
	_ulong									m_dwSubSetCnt;

	LPDIRECT3DTEXTURE9*		m_ppTextures;

	_ulong		 m_dwNumVtx;
	_ulong		 m_dwStride;
	_vec3*	m_pVtxPos;

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pFilePath, 
		const _tchar* pFileName);
	virtual CResourceses* Clone();
	virtual void	Free();


};

NS_END	

#endif // StaticMesh_h__
