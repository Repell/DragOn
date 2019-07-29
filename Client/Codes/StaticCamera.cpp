#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Function.h"

#define _ANGLE 45.f
#define  _SPEED 2.f
#define AAA 0.00001f

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
	m_fCamDist = 5.f;
	
	m_pTarget = pTarget;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pTarget->Get_vInfoPos(ENGINE::INFO_POS) + _vec3(0.f, 1.f, 0.f);
	m_pTransform->m_vAngle = m_pTarget->m_vAngle;
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

	m_pTargetMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Get_Management()->Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Mesh", ENGINE::COMP_DYNAMIC));

	if (m_pTargetMesh == nullptr)
		return E_FAIL;


	return S_OK;
}

_int CStaticCamera::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	////////////////////////////////////////////////////////////////////////
	 //Key에 따른 Transform Update
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pTarget->Get_vInfoPos(ENGINE::INFO_POS) + _vec3(0.f, 1.f, 0.f);
	//m_pTransform->m_vAngle = m_pTarget->m_vAngle;
	Check_InputDev(TimeDelta);
	ENGINE::CGameObject::Update_Object(TimeDelta);

	_vec3 vDir;
	vDir = m_pTransform->Get_vInfoPos(ENGINE::INFO_LOOK);
	//vDir = m_pTransform->m_vLook;
	D3DXVec3Normalize(&vDir, &vDir);

	m_vEye = m_pTransform->m_vInfo[ENGINE::INFO_POS] + (-vDir * m_fCamDist);
	m_vEye.y += 3.f;
	m_vAt = m_pTransform->m_vInfo[ENGINE::INFO_POS];

	////////////////////////////////////////////////////////////////////////

	//Player_Controll(TimeDelta);
	//Update_PlayerDir(TimeDelta);
	//m_vAt = m_pTarget->Get_vInfoPos(ENGINE::INFO_POS);
	//m_vEye = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	//m_vEye.y += 3.5f;


	// Update 최종 적용 (뷰 매트릭스 생성)
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

void CStaticCamera::Check_InputDev(const _double & TimeDelta)
{
	if (ENGINE::Get_DIKeyState(DIK_LSHIFT) & 0x80)
	{
		Move_Mouse(TimeDelta);
		Fix_Mouse();
	}

	_long	dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Z))
	{
		dwMouseMove > 0 ? m_fCamDist -= 1.f : m_fCamDist += 1.f;
		ENGINE::LimitNumber(m_fCamDist, 1.f, 10.f);
	}

	//if (ENGINE::Key_Press(ENGINE::dwKEY_Q))
	//	m_pTransform->m_vAngle.y += 5.f * TimeDelta;

}

void CStaticCamera::Move_Mouse(const _double & TimeDelta)
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

void CStaticCamera::Player_Controll(const _double & TimeDelta)
{

	if (ENGINE::Key_Combined(ENGINE::dwKEY_W, ENGINE::dwKEY_A))
	{
	}
	else if (ENGINE::Key_Combined(ENGINE::dwKEY_W, ENGINE::dwKEY_D))
	{
	}
	else if (ENGINE::Key_Combined(ENGINE::dwKEY_S, ENGINE::dwKEY_A))
	{
	}
	else if (ENGINE::Key_Combined(ENGINE::dwKEY_S, ENGINE::dwKEY_D))
	{
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		m_pTransform->m_vAngle.y -= _ANGLE * TimeDelta;
		m_pTransform->Update_Component(TimeDelta);
		m_pTarget->m_vInfo[ENGINE::INFO_POS] = m_pTransform->Get_NewPlayerPos(m_fCamDist);
		m_eCurDir = LEFT;
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		m_pTransform->m_vAngle.y += _ANGLE * TimeDelta;
		m_pTransform->Update_Component(TimeDelta);
		m_pTarget->m_vInfo[ENGINE::INFO_POS] = m_pTransform->Get_NewPlayerPos(m_fCamDist);
		m_eCurDir = RIGHT;
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_W))
	{
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->Get_vLookDir() * TimeDelta * _SPEED;
		m_pTransform->Update_Component(TimeDelta);
		m_pTarget->m_vInfo[ENGINE::INFO_POS] = m_pTransform->Get_NewPlayerPos(m_fCamDist);
		m_eCurDir = UP;
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_S))
	{
		m_pTransform->m_vInfo[ENGINE::INFO_POS] -= m_pTransform->Get_vLookDir() * TimeDelta * _SPEED;
		m_pTransform->Update_Component(TimeDelta);
		m_pTarget->m_vInfo[ENGINE::INFO_POS] = m_pTransform->Get_NewPlayerPos(m_fCamDist);
		m_eCurDir = DOWN;
	}

	if (m_eCurDir != DIR_END)
		m_pTargetMesh->Set_AnimationSet(107);
	else
		m_eCurDir = DIR_END;


}

void CStaticCamera::Update_PlayerDir(const _double & TimeDelta)
{
	_matrix matView;
	_float fAngle = 0.f;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);	//카메라의 월드 행렬

	_vec3	vLook, vRight, vDir, vCross, vCamPos, vPlayerLook;
	_vec3	vLeftUp, vLeftDown, vRightUp, vRightDown;

	memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));
	memcpy(&vLook, &matView.m[2][0], sizeof(_vec3));
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	//_matrix matTemp = m_pTransform->m_matWorld;
	vDir = m_pTarget->m_vInfo[ENGINE::INFO_LOOK];
	vRight.y = m_pTarget->m_vInfo[ENGINE::INFO_POS].y;
	vLook.y = m_pTarget->m_vInfo[ENGINE::INFO_POS].y;
	vCamPos.y = m_pTarget->m_vInfo[ENGINE::INFO_POS].y;
	vPlayerLook = vCamPos - m_pTarget->m_vInfo[ENGINE::INFO_POS];

	switch (m_eCurDir)
	{
	case CStaticCamera::RIGHT:
		D3DXVec3Cross(&vCross, &vDir, &-vRight);
		fAngle = D3DXVec3Dot(&vDir, &-vRight);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	case CStaticCamera::LEFT:
		D3DXVec3Cross(&vCross, &vDir, &vRight);
		fAngle = D3DXVec3Dot(&vDir, &vRight);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;

	case CStaticCamera::UP:
		D3DXVec3Cross(&vCross, &vDir, &vPlayerLook);
		fAngle = D3DXVec3Dot(&-vLook, &vDir);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	case CStaticCamera::DOWN:
		D3DXVec3Cross(&vCross, &vDir, &vPlayerLook);
		fAngle = D3DXVec3Dot(&vLook, &vDir);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y >= 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	case CStaticCamera::LEFTUP:
		D3DXVec3Normalize(&vLeftUp, &(-vRight + vLook));
		D3DXVec3Cross(&vCross, &vDir, &vLeftUp);
		fAngle = D3DXVec3Dot(&vDir, &vLeftUp);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	case CStaticCamera::LEFTDOWN:
		D3DXVec3Normalize(&vLeftDown, &(-vRight + -vLook));
		D3DXVec3Cross(&vCross, &vDir, &vLeftDown);
		fAngle = D3DXVec3Dot(&vDir, &vLeftDown);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	case CStaticCamera::RIGHTUP:
		D3DXVec3Normalize(&vRightUp, &(vRight + vLook));
		D3DXVec3Cross(&vCross, &vDir, &vRightUp);
		fAngle = D3DXVec3Dot(&vDir, &vRightUp);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	case CStaticCamera::RIGHTDOWN:
		D3DXVec3Normalize(&vRightDown, &(vRight + -vLook));
		D3DXVec3Cross(&vCross, &vDir, &vRightDown);
		fAngle = D3DXVec3Dot(&vDir, &vRightDown);

		if (fAngle > (1.f - AAA))
			break;

		fAngle = D3DXToDegree(acosf(fAngle));

		if (vCross.y < 0.f)
			fAngle = -fAngle;

		m_pTarget->m_vAngle.y += _float(fAngle * TimeDelta * _SPEED);
		//vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vDir * TimeDelta * _SPEED));
		break;
	}

	m_eCurDir = DIR_END;
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
