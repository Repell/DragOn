#include "SphereColl.h"

USING(ENGINE)

CSphereColl::CSphereColl(LPDIRECT3DDEVICE9 pDevice)
	: m_pGraphicDev(pDevice), m_fScale(0.01f),
	m_bInvisible(FALSE), m_bHit(FALSE), m_bKnockBack(FALSE)
{
	m_pGraphicDev->AddRef();
}

CSphereColl::~CSphereColl()
{
}

_vec3& CSphereColl::Get_CollPos()
{
	return m_vCollPos;
}

_vec3 & CSphereColl::Get_WeakPos()
{
	return m_vWeakPos;
}

_float CSphereColl::Get_Radius()
{
	return (m_fRadius * m_fScale);
}

void CSphereColl::Set_Scale(_float fScale)
{
	m_fScale = fScale;
}

void CSphereColl::Set_WeakPos(const _matrix * vBone)
{
	memcpy(m_vWeakPos, vBone->m[3], sizeof(_vec3));
	m_vWeakPos;
}

_int CSphereColl::Get_iHp(_int iDamage)
{
	if (iDamage > 0 && !m_bInvisible)
		m_iHp -= iDamage;

	return m_iHp;
}

_uint CSphereColl::Get_iHitStack(_bool bState)
{
	if (!bState)
		++m_iHitStack;
	else
		m_iHitStack = 0;

	return m_iHitStack;
}

void CSphereColl::Set_Invisible(_bool bState)
{
	m_bInvisible = bState;
}

_vec3 CSphereColl::Set_KnockBackDist(_bool bCheck, _vec3 fDist)
{
	if(bCheck)
		m_vKnockBackDir = fDist;
	m_vKnockBackDir.y = 0.f;

	return m_vKnockBackDir;
}

_bool CSphereColl::Get_HitState()
{
	return m_bHit;
}

_bool CSphereColl::Get_KnockBackState()
{
	return m_bKnockBack;
}

_bool CSphereColl::Get_AirboneState()
{
	return m_bAirbone;
}

HRESULT CSphereColl::Ready_SphereColl(_float fRadius, _int iHp)
{
	m_iHp = iHp;
	m_fRadius = fRadius;

	FAILED_CHECK_RETURN(Make_SphereMesh(fRadius), E_FAIL);
	FAILED_CHECK_RETURN(Make_Texture(), E_FAIL);

	return S_OK;
}

void CSphereColl::Render_SphereColl(const _matrix * pCollMatrix, _float fY)
{
	_matrix pMatrix = *pCollMatrix;
	pMatrix._42 += fY;
	memcpy(m_vCollPos, &pMatrix.m[3][0], sizeof(_vec3));

#ifdef _DEBUG
	_ulong iNumVertice = m_pMesh->GetNumVertices();
	_ulong iNumFaces = m_pMesh->GetNumFaces();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTexture(0, m_pTexture);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTX_SPHERE));
	m_pGraphicDev->SetFVF(VTXFVF_SPHERE);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, iNumVertice, 0, iNumFaces);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#endif
}

//_bool CSphereColl::Check_Collision(const _vec3 vTargetPos, _float fTargetRadius)
//{
//	_float fDist = 0.f;
//	_vec3 vDiff = { 0.f, 0.f, 0.f };
//
//	vDiff = vTargetPos - m_vCollPos;
//	fDist = D3DXVec3Length(&vDiff);
//
//	if (fDist <= fTargetRadius + m_fRadius)
//		return TRUE;
//
//	return FALSE;
//}

_bool CSphereColl::Check_ComponentColl(CSphereColl * pSphere)
{
	_float fDist = 0.f;
	_vec3 vDiff = {0.f, 0.f, 0.f};

	_vec3 vTarget = pSphere->Get_CollPos();
	_float fTargetRad = pSphere->Get_Radius();

	vDiff = vTarget - m_vCollPos;
	fDist = D3DXVec3Length(&vDiff);

	if (fDist <= (fTargetRad + (m_fRadius* m_fScale)))
		return TRUE;

	return FALSE;
}

HRESULT CSphereColl::Make_SphereMesh(_float fRadius)
{
	LPD3DXMESH pMesh;
	D3DXCreateSphere(m_pGraphicDev, fRadius, 10, 10, &pMesh, nullptr);

	//Clone
	pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, VTXFVF_SPHERE, m_pGraphicDev, &m_pMesh);
	pMesh->Release();

	m_pMesh->GetIndexBuffer(&m_pIB);
	m_pMesh->GetVertexBuffer(&m_pVB);

	//_ulong iNumVertice = m_pMesh->GetNumVertices();
	//_ulong iNumFaces = m_pMesh->GetNumFaces();
	//FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTX_SPHERE) * iNumVertice, 0,
	//	VTXFVF_SPHERE,	D3DPOOL_MANAGED, &m_pVB, NULL), E_FAIL);
	//FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX32) * iNumFaces, 0,
	//	D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, NULL), E_FAIL);

	return S_OK;
}

HRESULT CSphereColl::Make_Texture()
{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

		m_pTexture->UnlockRect(0);
	
		return S_OK;
}

CSphereColl * CSphereColl::Create(LPDIRECT3DDEVICE9 pDevice, _float fRadius, _int iHp)
{
	CSphereColl* pInstance = new CSphereColl(pDevice);

	if (FAILED(pInstance->Ready_SphereColl(fRadius, iHp)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSphereColl::Free()
{
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pMesh);
	Safe_Release(m_pTexture);

}
