#include "Camera.h"

USING(ENGINE)

CCamera::CCamera(LPDIRECT3DDEVICE9 pDev)
	:CGameObject(pDev)
{
	ZeroMemory(&m_vEye, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vAt, sizeof(D3DXVECTOR3));

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCamera::~CCamera()
{
}

int CCamera::Update_Object()
{
	return 0;
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
