#ifndef Boss_Blue_h__
#define Boss_Blue_h__

#include "GameObject.h"
#include "Effect_Tex.h"
#include "Static_Tower.h"

namespace ENGINE
{
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CCollider;
	class CSphereColl;
	class CShader;
	class CShadow;
}

class CBlue : public ENGINE::CGameObject
{
private:
	explicit CBlue(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBlue();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Set_Animation();
	void Get_WeaponMatrix(const char* tBone);
	void Get_WeaponMatrix2(const char* tBone);
	void Get_WeaponMatrix3(const char* tBone);
	void Check_EnemyGroup();
	_bool Check_EnemyColl(_vec3* vRevDir, const _tchar* szTag);
	_bool Check_EnemySphereColl(const _tchar* szTag);

private:
	VOID Animate_FSM(_uint iAniState);
	VOID Animate_Twice(_uint iAniState);

private: //Func Pointer
		 //Set Behavior
	VOID Set_Behavior_Progress();

	//Behavior to State Func
	typedef VOID(CBlue::*BEHAVIOR)();
	BEHAVIOR AiState;
	VOID State_Awaken();
	VOID State_Weak();
	VOID State_Hit();
	VOID State_KnockBack();
	VOID State_Airborne();
	VOID State_Idle();
	VOID State_Chase();
	VOID State_Quake();
	VOID State_Attack();
	VOID State_Dead();


private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	//Key Input, Camera, NaviMesh
private:
	//Key Input, Camera, NaviMesh
	_bool m_bAttack;
	_bool m_bQuake;
	_bool m_bAttStart;
	_bool m_bAttEnd;
	_double m_AttackTime;
	_float m_fDist;
	_float m_fReturnDist;
	_uint m_iCurAniSet;
	_uint m_iPreAniSet;

private:
	_bool m_bWeak;
	_bool m_bFire;
	_bool m_bSleep;
	_bool m_bFront;
	_bool m_bDead;
	_bool m_bTwice;
	_bool m_bRelicTwice;

	_bool m_bHit;
	_bool m_bKnockBack;
	_bool m_bAirborne;

private:
	_double m_FuryTime;
	_double m_TimeDelta;
	_double m_HitTime;
	_double m_AirTime;
	_double m_WeakTime;
	_double m_AccTime;

	//KnockBack
	_uint m_iKnockIdx[5];
	_uint m_iKnockCnt = 0;

	//Quake
	_uint m_iQuakeIdx[3];
	_uint m_iQuakeCnt = 0;

	//Scale
	_float m_fScale;

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CDynamicMesh* m_pMesh2;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTransform*	m_pTargetTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CCollider*			m_pCollider_Left;
	ENGINE::CCollider*			m_pCollider_Right;

	ENGINE::CSphereColl*	m_pSphereHead;
	ENGINE::CSphereColl*	m_pSphereBody;
	ENGINE::CSphereColl*	m_pTargetSphereColl;
	ENGINE::CSphereColl*	m_pRelicSphere;

	ENGINE::CShader*			m_pShader;
	ENGINE::CShadow*			m_pShadow;

	CStatic_Tower*				m_pTower;

private:
	_matrix m_pBody;
	_matrix m_pBoneMatrix_Right;
	_matrix m_pBoneMatrix_Left;

public:
	static CBlue* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Boss_Blue_h__
