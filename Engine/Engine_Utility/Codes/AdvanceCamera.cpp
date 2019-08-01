#include "AdvanceCamera.h"
#include "Transform.h"

USING(ENGINE)

CAdvanceCamera::CAdvanceCamera(LPDIRECT3DDEVICE9 pDevice)
{
	m_pGraphicDev = pDevice;
	m_pGraphicDev->AddRef();

	ZeroMemory(&m_matView, sizeof(_matrix));
	ZeroMemory(&m_matProj, sizeof(_matrix));

	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	m_vUp = { 0.f, 1.f, 0.f };
	m_fCamDist = 3.f;

}

void CAdvanceCamera::Set_Transform_Pos(ENGINE::INFO eInfo, const _vec3* vPos)
{
	m_pTransform->m_vInfo[eInfo] = *vPos;
}


_vec3 CAdvanceCamera::Get_INFO(ENGINE::INFO eInfo)
{
	return m_pTransform->Get_vInfoPos(eInfo);
}

_vec3 CAdvanceCamera::Get_LookDir()
{
	return m_pTransform->Get_vLookDir();
}

_vec3 CAdvanceCamera::Get_vNewPos(const _float fDist)
{
	return m_pTransform->Get_NewPlayerPos(fDist);
}

_matrix* CAdvanceCamera::Get_ViewMatrix()
{
	return &m_matView;
}

_matrix * CAdvanceCamera::Get_ProjMatrix()
{
	return &m_matProj;
}

HRESULT CAdvanceCamera::Ready_Component(ENGINE::CTransform * pTarget)
{
	m_pTarget = pTarget;
	NULL_CHECK_RETURN(m_pTarget, E_FAIL);

	m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);

	m_pTransform->m_vScale = {1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pTarget->Get_vInfoPos(ENGINE::INFO_POS);
	m_pTransform->Update_Component(0.f);
		
	m_vAt = m_pTarget->m_vInfo[ENGINE::INFO_POS];
	m_vAt.y += 1.f;
	m_vEye = (m_vAt - m_pTransform->m_vInfo[ENGINE::INFO_LOOK] * m_fCamDist);
	m_vEye.y += 2.f;
	m_vUp = { 0.f, 1.f, 0.f };

	Make_Camera();

	return S_OK;
}


_int CAdvanceCamera::Update_Component(const _double & TimeDelta)
{
	//Update Transform
	//////////////////////////////////////
	m_pTransform->Update_Component(TimeDelta);

	//Update Camera
	//////////////////////////////////////
	m_vAt = m_pTarget->Get_vInfoPos(ENGINE::INFO_POS);
	m_vAt.y += 1.f;
	m_vEye = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	m_vEye.y += 2.f;

	Make_ViewMatrix(&m_vEye, &m_vAt, &m_vUp);
	
	return NO_EVENT;
}

void CAdvanceCamera::Late_Update_Component()
{
	m_pTransform->Late_Update_Component();


}

HRESULT CAdvanceCamera::Make_Camera()
{
	Make_ViewMatrix(&m_vEye, &m_vAt, &m_vUp);
	Make_ProjectionMatrix(D3DXToRadian(60.f), WINCX / float(WINCY), 0.1f, 1000.f);
	
	return S_OK;
}

void CAdvanceCamera::Update_Target(const _double & TimeDelta)
{
	m_pTarget->m_vInfo[ENGINE::INFO_POS] = m_pTransform->Get_NewPlayerPos(m_fCamDist);
	m_pTarget->Update_Component(TimeDelta);
}

void CAdvanceCamera::Make_ViewMatrix(const D3DXVECTOR3 * pEye, const D3DXVECTOR3 * pAt, const D3DXVECTOR3 * pUp)
{
	D3DXMatrixLookAtLH(&m_matView, pEye, pAt, pUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CAdvanceCamera::Make_ProjectionMatrix(const float & fFovY, const float & fAspect, const float & fNear, const float & fFar)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFovY, fAspect, fNear, fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

CAdvanceCamera * CAdvanceCamera::Create(LPDIRECT3DDEVICE9 pGraphic, ENGINE::CTransform * pTarget)
{
	CAdvanceCamera* pInstance = new CAdvanceCamera(pGraphic);

	if (FAILED(pInstance->Ready_Component(pTarget)))
		Safe_Release(pInstance);

	return pInstance;
}

void CAdvanceCamera::Free()
{
	Safe_Release(m_pGraphicDev);
}
