#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "Hierarchy.h"
#include "AniControll.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CDynamicMesh :
	public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh();

public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Render_Meshes();
	void Set_AnimationSet(const _uint& iIndex);
	void Play_AnimationSet(const _float& fTimeDelta);

private:
	// 모든 뼈들을 순회하면서 뼈들이 가지고 있는 행렬(Combined Transform Matrix)을 갱신(적용)해주는 함수
	void Update_FrameMatrix(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);


private:
	D3DXFRAME* m_pRootFrame;
	CHierarchy*	m_pLoader;
	CAniControll* m_pAniCon;
	list<D3DXMESHCONTAINER_DERIVED*> m_MeshContainerList;

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CResourceses* Clone();
	virtual void	Free();

};

NS_END

#endif // DynamicMesh_h__
