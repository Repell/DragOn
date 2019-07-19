#include "Collider.h"
#include "SphereColl.h"

USING(ENGINE)

CCollider::CCollider(LPDIRECT3DDEVICE9 pDevice)
	: m_pGraphicDev(pDevice)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_matColWorld, sizeof(_matrix));
}

CCollider::~CCollider()
{
	m_pMesh->Release();
}

_float CCollider::Get_Radius()
{
	return (m_fRadius * m_fScale);
}

void CCollider::Set_Scale(_float fScale)
{
	m_fScale = fScale;
}

HRESULT CCollider::Ready_Collider(const _vec3 * pPos, const _ulong & dwNumVtx, const _ulong & dwStride, _float fRadius)
{
	D3DXComputeBoundingBox(pPos, dwNumVtx, sizeof(_vec3), &m_vMin, &m_vMax);	

#ifdef _DEBUG
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTX_CUBE) * 8,
		0,
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX32) * 12,
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL), E_FAIL);

	VTX_CUBE*		pVtxCube = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTex = pVtxCube[0].vPos;

	pVtxCube[1].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTex = pVtxCube[1].vPos;

	pVtxCube[2].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTex = pVtxCube[2].vPos;

	pVtxCube[3].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTex = pVtxCube[3].vPos;
	
	pVtxCube[4].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTex = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTex = pVtxCube[5].vPos;

	pVtxCube[6].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTex = pVtxCube[6].vPos;

	pVtxCube[7].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTex = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX32*	pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// -x
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;
	
	// +y
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// -y	 
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// +z
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// -z
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

#endif

	Ready_Collider_Sphere(fRadius);

	return S_OK;
}

void CCollider::Render_Collider(COLLTYPE eType, const _matrix * pCollMatrix, _vec3 CollPos)
{
	_matrix matRevolve;	
	D3DXMatrixIdentity(&matRevolve);
	D3DXMatrixTranslation(&matRevolve, CollPos.x, CollPos.y, CollPos.z);	// 로컬 상태의 공전매트릭스(줄어든 스케일만큼 값이 커야함)
	
	m_matColWorld = matRevolve * *pCollMatrix;		//공전, 부모
	
#ifdef _DEBUG
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pCollMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTX_CUBE));
	m_pGraphicDev->SetFVF(FVF_CUBE);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matColWorld);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#endif
}

_bool CCollider::Check_ComponentColl(CSphereColl * pSphere)
{
	_float fDist = 0.f;
	_vec3 vDiff = { 0.f, 0.f, 0.f };
	_vec3 vCollPos = { 0.f, 0.f, 0.f };
	memcpy(vCollPos, m_matColWorld.m[3], sizeof(_vec3));

	_vec3 vTarget = pSphere->Get_CollPos();
	_float fTargetRad = pSphere->Get_Radius();

	vDiff = vTarget - vCollPos;
	fDist = D3DXVec3Length(&vDiff);

	if (fDist <= (fTargetRad + (m_fRadius* m_fScale)))
		return TRUE;

	return FALSE;
}

HRESULT CCollider::Ready_Collider_Sphere(_float fRadius)
{
	//D3DXComputeBoundingSphere(pPos, dwNumVtx, sizeof(_vec3), &m_vCenter, &m_fRadius);

	//_vec3 vDir = *D3DXVec3Normalize(&vDir, &m_vCenter);

	LPD3DXMESH pMesh;
	D3DXCreateSphere(m_pGraphicDev, fRadius, 10, 10, &pMesh, nullptr);
	m_fRadius = fRadius;

	//Clone을 해야만 우리가 직접만든 FVF 포맷을 적용시켜 사용할 수 있음
	pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, ENGINE::VTXFVF_SPHERE, m_pGraphicDev, &m_pMesh);
	pMesh->Release();
		
	//ENGINE::VTX_SPHERE* pCube;
	//if (SUCCEEDED(m_pMesh->LockVertexBuffer(0, (void**)&pCube)))
	//{
	//	int numVtx = m_pMesh->GetNumVertices();

	//	for (int i = 0; i < numVtx; ++i)
	//	{
	//		pCube[i].vPos.x += 5.f;
	//		pCube[i].vPos.y += 5.f;
	//		pCube[i].vPos.z += 5.f;
	//	}

	//	m_pMesh->UnlockVertexBuffer();
	//}

	return S_OK;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pDevice, const _vec3 * pPos, const _ulong & dwNumVtx, const _ulong & dwStride, _float fRadius)
{
	CCollider* pInstance = new CCollider(pDevice);

	if (FAILED(pInstance->Ready_Collider(pPos, dwNumVtx, dwStride, fRadius)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCollider::Free()
{
	Safe_Release(m_pGraphicDev);

#ifdef _DEBUG
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	for (_uint i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);
#endif
}
