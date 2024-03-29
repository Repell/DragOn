#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

#define _SPEED 15.f
#define _ANGLE 60.f

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDev)
	:ENGINE::CCamera(pDev), m_pCamTarget(nullptr),
	m_fTargetDist(0.f), m_fCamAngle(0.f), m_fCamSpeed(0.f), m_fTargetDistMax(10.f), bSpectre(FALSE), m_fCamAngleY(180)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_Object()
{
	Add_Component();

	//m_pTransCom->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_fTargetDist = 20.f;
	m_fCamAngle = -10.f;
	m_fCamSpeed = 1.f;
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

HRESULT CDynamicCamera::Late_Init()
{
	m_pCamTarget = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Management()->Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	
	if (m_pCamTarget == nullptr)
	{
		bSpectre = TRUE;
		return E_FAIL;
	}

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	Key_Input(TimeDelta);

	if (bSpectre)
		Key_Spectre(TimeDelta);

	return NO_EVENT;
}

void CDynamicCamera::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	if (bSpectre)
		Target_Spectre();
	else
		Target_Renewal();

	if (!m_pCamTarget->bCamTarget)
	{
		bSpectre = true;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	}

}

HRESULT CDynamicCamera::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	//Transform
	pComponent = m_pTransCom = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CDynamicCamera::Reset_Camera()
{
	m_fTargetDist = 20.f;
	m_fCamAngle = -10.f;
	m_fCamSpeed = 1.f;

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CDynamicCamera::Key_Input(const _double& TimeDelta)
{
	if (ENGINE::Key_Down(ENGINE::dwKEY_F1))
		Reset_Camera();

	_long dwMouseMove = 0;
	if (dwMouseMove < ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Z))
		m_fTargetDist -= m_fCamSpeed;
	if (dwMouseMove > ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Z))
		m_fTargetDist += m_fCamSpeed;

	if (m_fTargetDist < 2.5f)
		m_fTargetDist = 3.f;

	if (ENGINE::Key_Press(ENGINE::dwKEY_Q))
		m_fCamAngle += _ANGLE * TimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_E))
		m_fCamAngle -= _ANGLE * TimeDelta;

	if (m_fCamAngle > 90.f)
		m_fCamAngle = 90.f;
	else if (m_fCamAngle < -90.f)
		m_fCamAngle = -90.f;

}

void CDynamicCamera::Key_Spectre(const _double& TimeDelta)
{
	_vec3 vNewDir = {};
	//Fix Mouse
	POINT pt = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	_long dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
		m_pTransCom->m_vAngle.y += dwMouseMove * _ANGLE * TimeDelta;

	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
		m_pTransCom->m_vAngle.x += dwMouseMove * _ANGLE * TimeDelta;


	//Key Input
	if (ENGINE::Key_Press(ENGINE::dwKEY_W))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] += m_pTransCom->m_vDir * _SPEED * TimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_S))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] -= m_pTransCom->m_vDir * _SPEED * TimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] +=
			vNewDir.NewDir(&m_pTransCom->m_vDir, &_vec3(0.f, 1.f, 0.f)) * _SPEED * TimeDelta;
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] -=
			vNewDir.NewDir(&m_pTransCom->m_vDir, &_vec3(0.f, 1.f, 0.f)) * _SPEED * TimeDelta;
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_SPACE))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS].y += _SPEED * TimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_Shift))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS].y -= _SPEED * TimeDelta;


	if (ENGINE::Key_Down(ENGINE::dwKEY_F4) && bSpectre)
	{
		Reset_Camera();
		bSpectre = false;
		m_pCamTarget->bCamTarget = true;
	}

}

void CDynamicCamera::Target_Renewal()
{
	_vec3 vEye = vEye.Reverse(&m_pCamTarget->Get_vLookRealDir());

	//카메라는 플레이어의 역방향
	vEye *= -m_fTargetDist;

	//높이는 카메라의 각도의 따라 조절될 것
	_vec3 vRight = {};
	memcpy(&vRight, m_pCamTarget->m_matWorld.m[0], sizeof(_vec3));

	//right 벡터 기준으로 축 회전
	D3DXMATRIX matRotAxis;

	//D3DXMatrixRotationAxis: 임의의 축회전
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_fCamAngle));
	D3DXVec3TransformNormal(&vEye, &vEye, &matRotAxis);

	//카메라의 위치
	vEye += m_pCamTarget->m_vInfo[ENGINE::INFO_POS];
	m_vEye = vEye;

	_vec3 vAt = m_pCamTarget->m_vInfo[ENGINE::INFO_POS];
	vAt.y += m_pCamTarget->m_fJump;
	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = vAt;		//Spectre Cam StartPos
	vAt.y += 1.25f;

	ENGINE::CCamera::Make_ViewMatrix(&vEye, &vAt, &_vec3(0.f, 1.f, 0.f));
}

void CDynamicCamera::Target_Spectre()
{
	_vec3 vEye = vEye.Reverse(&m_pTransCom->Get_vLookRealDir());
	//_vec3 vEye = vEye.Reverse(&m_pCamTarget->m_vDir);

	//카메라는 플레이어의 역방향
	vEye *= m_fTargetDist;

	//높이는 카메라의 각도의 따라 조절될 것
	_vec3 vRight = {};
	memcpy(&vRight, m_pTransCom->m_matWorld.m[0], sizeof(_vec3));

	//right 벡터 기준으로 축 회전
	D3DXMATRIX matRotAxis;

	//D3DXMatrixRotationAxis: 임의의 축회전
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_fCamAngle));
	D3DXVec3TransformNormal(&vEye, &vEye, &matRotAxis);

	//카메라의 위치
	vEye += m_pTransCom->m_vInfo[ENGINE::INFO_POS];
	m_vEye = vEye;

	_vec3 vAt = m_pTransCom->m_vInfo[ENGINE::INFO_POS];
	//vAt.y += 1.25f;

	ENGINE::CCamera::Make_ViewMatrix(&vEye, &vAt, &_vec3(0.f, 1.f, 0.f));
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		ENGINE::Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Make_ProjectionMatrix(D3DXToRadian(60.f), WINCX / float(WINCY), 0.1f, 1000.f);

	return pInstance;
}

void CDynamicCamera::Free()
{
	ENGINE::CGameObject::Free();
}
