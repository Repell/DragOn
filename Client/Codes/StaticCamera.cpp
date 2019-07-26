#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: ENGINE::CCamera(pGraphicDev)
{
	ZeroMemory(&m_vCamAngle, sizeof(_vec3));
}

HRESULT CStaticCamera::Ready_Object(ENGINE::CTransform * pTarget, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	NULL_CHECK_RETURN(pTarget, E_FAIL);

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_fCamSpeed = 10.f;
	m_fCamDist = 4.f;


	m_pTarget = pTarget;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pTarget->m_vInfo[ENGINE::INFO_POS] + _vec3(0.f, 1.f, 0.f);
	m_pTransform->Update_Component(0.f);

	m_vAt = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	m_vEye = (m_vAt - m_pTransform->m_vInfo[ENGINE::INFO_LOOK] * m_fCamDist);
	m_vUp = { 0.f, 1.f, 0.f };

	//FAILED_CHECK_RETURN(ENGINE::CCamera::Ready_Object(), E_FAIL);
	Make_ViewMatrix(&m_vEye, &m_vAt, &m_vUp);
	Make_ProjectionMatrix(fFovY, fAspect, fNear, fFar);

	return S_OK;
}

HRESULT CStaticCamera::Late_Init()
{
	m_pTarget = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Management()->Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));

	if (m_pTarget == nullptr)
		return E_FAIL;

	return S_OK;
}

_int CStaticCamera::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	// Key에 따른 Transform Update
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pTarget->m_vInfo[ENGINE::INFO_POS] + _vec3(0.f, 1.f, 0.f);
	Check_InputDev(TimeDelta);
	ENGINE::CGameObject::Update_Object(TimeDelta);

	_vec3 vDir;
	vDir = m_pTransform->Get_vInfoPos(ENGINE::INFO_LOOK);
	D3DXVec3Normalize(&vDir, &vDir);

	m_vEye = m_pTransform->m_vInfo[ENGINE::INFO_POS] + (-vDir * m_fCamDist);
	m_vEye.y += 2.5f;
	m_vAt = m_pTransform->m_vInfo[ENGINE::INFO_POS];

	// Update 최종 적용
	ENGINE::CCamera::Update_Object(TimeDelta);

	return NO_EVENT;
}

HRESULT CStaticCamera::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	//Transform
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CStaticCamera::Check_InputDev(const _float & fTimeDelta)
{
	if (ENGINE::Get_DIKeyState(DIK_LSHIFT) & 0x80)
	{
		Move_Mouse(fTimeDelta);
		Fix_Mouse();
	}

	_long	dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Z))
	{
		dwMouseMove > 0 ? m_fCamDist -= 1.f : m_fCamDist += 1.f;
		ENGINE::LimitNumber(m_fCamDist, 1.f, 10.f);
	}

}

void CStaticCamera::Move_Mouse(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;

	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	{
		//m_pTransform->Rotation(ENGINE::ROT_X, (dwMouseMove * 0.1f));
		m_pTransform->m_vAngle.x += (dwMouseMove * 0.1f);
	}

	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
	{
		//m_pTransform->Rotation(ENGINE::ROT_Y, (dwMouseMove * 0.1f));
		m_pTransform->m_vAngle.y += (dwMouseMove * 0.1f);
	}
	
	if (m_pTransform->m_vAngle.x <= 0.f)
		m_pTransform->m_vAngle.x = 0.f;

	if (m_pTransform->m_vAngle.x >= 90.f)
		m_pTransform->m_vAngle.x = 89.f;

}

void CStaticCamera::Fix_Mouse()
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

CStaticCamera * CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::CTransform * pTarget, const _float & fFovY, const _float & fAspect, const _float & fNear, const _float & fFar)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pTarget, fFovY, fAspect, fNear, fFar)))
	{
		ERR_BOX("CStaticCamera Create Failed");
		ENGINE::Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticCamera::Free()
{
	ENGINE::CCamera::Free();
}
