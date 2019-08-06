#ifndef Boss_Keroberos_h__
#define Boss_Keroberos_h__

#include "GameObject.h"
#include "Effect_Tex.h"
#include "Effect_KeroFire.h"
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

class CKeroberos : public ENGINE::CGameObject
{
	enum JUMPSTATE
	{
		JUNPFORNT,
		JUMPBACK,
		JUMPLEFT,
		JUMPRIGHT,
		JUMPEND

	};

private:
	explicit CKeroberos(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CKeroberos();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Set_Animation();
	_float Get_PlayerAngleY();
	void Get_WeaponMatrix(const char* tBone);
	void Get_HeadMatrix(const char* tBone, _matrix* pMatrix);
	void Check_EnemyGroup();
	_bool Check_EnemyColl(_vec3* vRevDir, const _tchar* szTag);

private:
	VOID Animate_FSM(_uint iAniState);
	
private: //Func Pointer
		 //Set Behavior
	VOID Set_Behavior_Progress();

	//Behavior to State Func
	typedef VOID(CKeroberos::*BEHAVIOR)();
	BEHAVIOR AiState;
	VOID State_Awaken();
	VOID State_Hit();	//Airbone to Hit
	VOID State_KnockBack();
	VOID State_Idle();
	VOID State_Chase();
	VOID State_Chase_Jump();
	VOID State_Attack_Firebreath();
	VOID State_Attack_Bite();
	VOID State_Attack_BiteCombo();
	VOID State_Attack_Sweep();
	VOID State_Attack_TailCombo();
	VOID State_Dead();

private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	//Temp Angle
	_float m_fAngle;
	_uint iOldRnd;
	//Time
	_double m_CheckState;
	_double m_AttackTime;
	_double m_AccTime;
	_double m_FuryTime;
	_double m_TimeDelta;
	_double m_HitTime;
	_double m_BreathTime;

	//Animation
	_uint m_iCurAniSet;
	_uint m_iPreAniSet;
	_uint m_iAttackAniset;

private:
	//Atack State
	_bool m_bAttack;
	_bool m_bBite;
	_bool m_bTwice;
	_bool m_bSweep;
	_bool m_bFireBreath;
	_bool m_bBreathDelay;
	_bool m_bAttStart;
	_bool m_bAttEnd;

	//Tail Combo
	_uint m_iTailIdx[4];
	_uint m_iTailCnt;
	_bool m_bTailCombo;
	
	//Bite Combo
	_uint m_iBiteIdx[6];
	_uint m_iBiteCnt = 0;
	_bool m_bBiteCombo;

	_float m_fDist;
	_float m_fReturnDist;

private:
	//Default State
	_bool m_bDead;
	_bool m_bSleep;
	_bool m_bHit;
	_bool m_bJump[3];
	JUMPSTATE m_eJumpState;
	
	//KnockBack
	_uint m_iKnockIdx[6];
	_uint m_iKnockCnt;
	_bool m_bKnockBack;

private:
	_matrix m_pHeadMatrix;
	_matrix m_pTailMatrix1;
	_matrix m_pTailMatrix2;

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTransform*	m_pTargetTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CCollider*			m_pCollider_Head;
	ENGINE::CCollider*			m_pCollider_Tail1;
	ENGINE::CCollider*			m_pCollider_Tail2;

	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CSphereColl*	m_pTargetSphereColl;

	ENGINE::CShader*			m_pShader;
	ENGINE::CShadow*			m_pShadow;

public:
	static CKeroberos* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Boss_Keroberos_h__

