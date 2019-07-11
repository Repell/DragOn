#include "StaticMesh.h"

USING(ENGINE)

CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{
}

CStaticMesh::CStaticMesh(const CStaticMesh & rhs)
	: CMesh(rhs),
	m_pOriginMesh(rhs.m_pOriginMesh),
	m_pMesh(rhs.m_pMesh),
	m_pAdjacency(rhs.m_pAdjacency),
	m_pSubSetBuffer(rhs.m_pSubSetBuffer),
	m_pSubSet(rhs.m_pSubSet),
	m_dwSubSetCnt(rhs.m_dwSubSetCnt),
	m_dwStride(rhs.m_dwStride),
	m_dwNumVtx(rhs.m_dwNumVtx),
	m_pVtxPos(rhs.m_pVtxPos)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubSetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubSetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}
	m_pAdjacency->AddRef();
	m_pSubSetBuffer->AddRef();
	m_pMesh->AddRef();
	m_pOriginMesh->AddRef();
}

CStaticMesh::~CStaticMesh()
{

}

HRESULT CStaticMesh::Ready_Meshes(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar	szFullPath[MID_STR] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, &m_pAdjacency,
		&m_pSubSetBuffer, NULL, &m_dwSubSetCnt, &m_pOriginMesh)))
		return E_FAIL;
	
	//////////////////////////////

	_ulong dwFVF = m_pOriginMesh->GetFVF();

	if (!(dwFVF & D3DFVF_NORMAL))	//Normal 이 없을때
	{
		m_pOriginMesh->CloneMeshFVF(m_pOriginMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		//정점의 노말 값을 계산하여 삽입해주는 함수
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
		m_pOriginMesh->CloneMeshFVF(m_pOriginMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);

	void*	pVertex = nullptr;

	m_dwNumVtx = m_pMesh->GetNumVertices();

	m_pVtxPos = new _vec3[m_dwNumVtx];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	//메쉬 정점 중 위치정보를 파악함

	_ubyte byOffSet = 0;
	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = (_ubyte)Decl[i].Offset;
			break;
		}
	}
	

	m_pMesh->UnlockVertexBuffer();
	//////////////////////


	m_pSubSet = (D3DXMATERIAL*)m_pSubSetBuffer->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];

	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		_tchar szFileName[MID_STR] = L"";

		lstrcpy(szFullPath, pFilePath);
		MultiByteToWideChar(CP_ACP, 0, m_pSubSet[i].pTextureFilename, 
			strlen(m_pSubSet[i].pTextureFilename), szFileName, MID_STR);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
			return E_FAIL;

	}


	return S_OK;
}

void CStaticMesh::Render_Meshes()
{
	for (_ulong i = 0; i < m_dwSubSetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}

CStaticMesh * CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar * pFileName)
{
	CStaticMesh* pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		ERR_BOX("StaticMesh Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CResourceses * CStaticMesh::Clone()
{
	return new CStaticMesh(*this);
}

void CStaticMesh::Free()
{
	CMesh::Free();

	for (_ulong i = 0 ; i < m_dwSubSetCnt; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubSetBuffer);
	Safe_Release(m_pMesh);
	Safe_Release(m_pOriginMesh);

	if (!m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Delete_Array(m_ppTextures);
}
