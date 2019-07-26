#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"

namespace ENGINE
{
	class CTransform;
}

class CStaticCamera : public ENGINE::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera() = default;
	
public:
	HRESULT Ready_Object(
		ENGINE::CTransform* pTarget,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;

private:
	ENGINE::CTransform*				m_pTarget = nullptr;
	ENGINE::CTransform*				m_pTransform = nullptr;
	_float							m_fCamSpeed = 0.f;
	_float							m_fCamDist = 0.f;
	_vec3							m_vCamAngle;

private:
	HRESULT		Add_Component();
	void		Check_InputDev(const _float& fTimeDelta);
	void		Move_Mouse(const _float& fTimeDelta);
	void		Fix_Mouse();

public:
	static CStaticCamera*		Create(
		LPDIRECT3DDEVICE9 pGraphicDev,
		ENGINE::CTransform* pTarget,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);
	virtual void				Free();

};

#endif // StaticCamera_h__
