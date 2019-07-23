#include "DynamicMesh.h"

USING(ENGINE)

CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev),
	m_pLoader(nullptr),
	m_pAniCon(nullptr)
{
}

CDynamicMesh::CDynamicMesh(const CDynamicMesh & rhs)
	: CMesh(rhs),
	m_pLoader(rhs.m_pLoader),
	m_pRootFrame(rhs.m_pRootFrame),
	m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAniCon = CAniControll::Create(*rhs.m_pAniCon);
}

CDynamicMesh::~CDynamicMesh()
{
}

HRESULT CDynamicMesh::Ready_Meshes(const _tchar * pFilePath, const _tchar * pFileName)
{
	_tchar szFullPath[MAX_STR] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	//Hierarchy / Animation Controller
	m_pLoader = CHierarchy::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER pAniCon = nullptr;

	FAILED_CHECK_RETURN(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader,
		NULL,
		&m_pRootFrame,
		&pAniCon), E_FAIL);

	m_pAniCon = CAniControll::Create(pAniCon);
	NULL_CHECK_RETURN(m_pAniCon, E_FAIL);

	Safe_Release(pAniCon);

	_matrix matTemp;
	Update_FrameMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void CDynamicMesh::Render_Meshes()
{
	auto iter = m_MeshContainerList.begin();
	auto iter_End = m_MeshContainerList.end();

	for (; iter != iter_End; ++iter)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshCon = (*iter);

		for (_ulong i = 0; i < pMeshCon->dwNumBones; ++i)
			pMeshCon->pRenderingMatrix[i] = pMeshCon->pFrameOffSetMatrix[i] * (*pMeshCon->ppFrameCombinedMatrix[i]);

		void* pSrcVtx = nullptr;
		void* pDestVtx = nullptr;

		pMeshCon->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshCon->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshCon->pSkinInfo->UpdateSkinnedMesh(pMeshCon->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);

		//Render

		for (_ulong i = 0; i < pMeshCon->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshCon->ppTexture[i]);
			pMeshCon->MeshData.pMesh->DrawSubset(i);
		}


		pMeshCon->pOriMesh->UnlockVertexBuffer();
		pMeshCon->MeshData.pMesh->UnlockVertexBuffer();


	}

}

void CDynamicMesh::Set_AnimationSet(const _uint & iIndex)
{
	m_pAniCon->Set_AnimationSet(iIndex);
}

void CDynamicMesh::Set_QuickSet(const _uint & iIndex)
{
	m_pAniCon->Set_QuickSet(iIndex);
}

void CDynamicMesh::Play_AnimationSet(const _double& TimeDelta)
{
	m_pAniCon->Play_AnimationSet(TimeDelta);

	//_matrix matTemp;
	Update_FrameMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&_matrix()));
}

const	D3DXFRAME_DERIVED* CDynamicMesh::Get_FrameByName(const char * pFrameName)
{
	return	(D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool CDynamicMesh::Is_AnimationSetEnd(void)
{
	return m_pAniCon->Is_AnimationSetEnd();
}

D3DXMESHCONTAINER_DERIVED* CDynamicMesh::Get_MeshContainer()
{
	return m_MeshContainerList.front();
}

CAniControll * CDynamicMesh::Get_AniController()
{
	return m_pAniCon;
}

void CDynamicMesh::Update_FrameMatrix(D3DXFRAME_DERIVED * pFrame, const _matrix * pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->combinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->combinedTransformMatrix);
}

void CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshCon = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshCon->dwNumBones; ++i)
		{
			//뼈의 이름을 얻어오는 함수
			const char* pBonName = pMeshCon->pSkinInfo->GetBoneName(i);
			// 개중요한 함수. 이름을 토대로 뼈를 찾아다줌
			D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED *)D3DXFrameFind(m_pRootFrame, pBonName);

			pMeshCon->ppFrameCombinedMatrix[i] = &pFrame->combinedTransformMatrix;
		}
		m_MeshContainerList.emplace_back(pMeshCon);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

CDynamicMesh * CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFilePath, const _tchar * pFileName)
{
	CDynamicMesh* pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CResourceses * CDynamicMesh::Clone()
{
	return new CDynamicMesh(*this);
}

void CDynamicMesh::Free()
{
	CMesh::Free();

	Safe_Release(m_pAniCon);

	if (!m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}
	m_MeshContainerList.clear();
}
