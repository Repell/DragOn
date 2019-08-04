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
	void Set_ParentMatrix(const _matrix* pParentMat)
	{		m_matWorld *= *pParentMat; 	}
	void Set_AniState(_uint iAni)
	{		m_iCurAniIndex = iAni;	}
	void Set_StartPos(_vec3 vPos)
	{
		m_vStartPos = vPos;
	}


	_vec3 Get_vLookDir();	//Y°ª 0.f;
	_vec3 Get_vLookRealDir();	
	_vec3 Get_vInfoPos(ENGINE::INFO eInfo);
	_float Get_BossDistance(CTransform* pTarget, _float fY);
	_float Get_TargetDistance(CTransform* pTarget);
	_vec3 Get_TargetReverseDir(CTransform* pTarget);
	_bool Get_Dead(_bool bDead = FALSE);
	_vec3 Get_NewPlayerPos(_float fDist);
	_float Fall_BackBoss(_float fSpeed, const _double& TimeDelta);

public:
	HRESULT Ready_Trasnform(_vec3 vLook);
	virtual _int Update_Component(const _double& TimeDelta) override;
	virtual void Late_Update_Component() override;

	_bool Check_TargetFront();

	void Stalk_Target(CTransform* pTransform, const _double& fTime, const _float fSpeed);
	_vec3 Stalk_TargetDir(CTransform* pTransform, const _double& fTime, const _float fSpeed);
	_float Fix_TargetLookAngleY(CTransform* pTarget, _float fSearchDist);
	_float Fix_TargetLook(CTransform* pTarget, _float fSearchDist);
	_float Fix_TargetRevLook(CTransform* pTarget, _float fSearchDist);

	_matrix* Compute_LockTarget(const _vec3* pTargetPos);
	void Chase_Target(const _vec3* pTarget, const _float& fSpeed);
	void Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

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
	_bool m_bAttackState;
	_float m_fJump;

	_bool m_bDead;
	_bool m_bFront;
	_bool bRotate;
	_bool m_bWeak;
	_double m_TimeDelta;
	_float m_fRotate;
	_float m_fFixAngleY;
	_uint m_iCurAniIndex;
	_vec3 m_vStartPos;
	

public:
	static CTransform*	Create(_vec3& vLook);

protected:
	virtual void Free() override;

};

NS_END

#endif // Transform_h__
