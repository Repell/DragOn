#include "Camera.h"

USING(ENGINE)

CCamera::CCamera(LPDIRECT3DDEVICE9 pDev)
	:CGameObject(pDev)
{
	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	m_vUp = { 0.f, 1.f, 0.f };

	m_fFovY = 60.f;
	m_fAspect = 0.f;
	m_fNear = 0.f;
	m_fFovY = 0.f;

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCamera::~CCamera()
{
}

int CCamera::Update_Object(const _double& TimeDelta)
{
	Make_ViewMatrix(&m_vEye, &m_vAt, &m_vUp);

	return NO_EVENT;
}



void CCamera::Make_ViewMatrix(const D3DXVECTOR3 * pEye,
	const D3DXVECTOR3 * pAt,
	const D3DXVECTOR3 * pUp)
{
	D3DXMatrixLookAtLH(&m_matView, pEye, pAt, pUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::Make_ProjectionMatrix(const float & fFovY,
	const float & fAspect, 
	const float & fNear, 
	const float & fFar)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFovY, fAspect, fNear, fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}
