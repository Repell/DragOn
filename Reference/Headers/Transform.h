#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTransform :
	public CComponent
{
public:
	enum eDirect
	{
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT
	};


private:
	explicit CTransform();
	virtual ~CTransform();

public:
	void Set_ParentMatrix(const _matrix* pParentMat)
	{		m_matWorld *= *pParentMat; 	}

	_vec3 Get_vLookDir();	//Y°ª 0.f;
	_vec3 Get_vInfoPos(ENGINE::INFO eInfo);

public:
	HRESULT Ready_Trasnform(_vec3 vLook);
	virtual _int Update_Component(const _double& TimeDelta) override;
	virtual void Late_Update_Component() override;
	void Chase_Target(const _vec3* pTarget, const _float& fSpeed);
	_matrix* Compute_LockTarget(const _vec3* pTargetPos);
	void Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	_float Fix_TargetLookAngleY(CTransform* pTarget, _float fSearchDist);
	void Stalk_Target(CTransform* pTransform, const _double& fTime, const _float fSpeed);
	void Fix_AngleY(_float fAngleY, eDirect eDir);

private:
	void Rotation_AngleY(const _double& TimeDelta);

public:
	_vec3 m_vLook;
	_vec3 m_vDir;
	_vec3 m_vInfo[INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix	m_matWorld;
	_bool bCamTarget;
	_float m_fJump;

	_bool bRotate;
	_double m_RotTime;
	_float m_fFixAngleY;
	eDirect m_eCurDir;
	eDirect m_ePreDir;

public:
	static CTransform*	Create(_vec3& vLook);

protected:
	virtual void Free() override;

};

NS_END

#endif // Transform_h__
