#pragma once

#ifndef CAMERA_H__
#define CAMERA_H__

#include "GameObject.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CCamera :
	public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pDev);
public:
	virtual ~CCamera();

public:
	_vec3& Get_vEye() { return m_vEye; }

public:
	virtual _int Update_Object();

protected:
	void Make_ViewMatrix(
		const D3DXVECTOR3* pEye,
		const D3DXVECTOR3* pAt,
		const D3DXVECTOR3* pUp);

	void Make_ProjectionMatrix(
		const float& fFovY,
		const float& fAspect,
		const float& fNear,
		const float& fFar);

protected:
	D3DXMATRIX	 m_matView;
	D3DXMATRIX	 m_matProj;
	D3DXVECTOR3	m_vEye;
	D3DXVECTOR3	m_vAt;

};

NS_END

#endif // !CAMERA_H__
