#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTransform :
	public CComponent
{
private:
	explicit CTransform();
	virtual ~CTransform();

public:
	HRESULT Ready_Trasnform(_vec3 vLook);
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void Late_Update_Component() override;
	void Chase_Target(const _vec3* pTarget, const _float& fSpeed);
	_matrix* Compute_LockTarget(const _vec3* pTargetPos);
	void Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

public:
	_vec3 m_vLook;
	_vec3 m_vDir;
	_vec3 m_vInfo[INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix	m_matWorld;
	_bool bCamTarget;
	_float m_fJump;

public:
	static CTransform*	Create(_vec3& vLook);

protected:
	virtual void Free() override;

};

NS_END

#endif // Transform_h__
