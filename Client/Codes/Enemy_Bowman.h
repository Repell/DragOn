#ifndef Enemy_Bowman_h__
#define Enemy_Bowman_h__

#include "GameObject.h"
#include "Enemy_Arrow.h"

namespace ENGINE
{
	class CManagement;
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CSphereColl;
	class CShader;
	class CWeapon;
}

class CEnemy_Bowman : public ENGINE::CGameObject
{
private:
	explicit CEnemy_Bowman(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEnemy_Bowman();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Set_Animation();
	void Get_WeaponMatrix(const char* tBone);
	void Check_EnemyGroup();
	_bool Check_EnemyColl(_vec3* vRevDir, const _tchar* szTag);

private:
	VOID Animate_FSM(_uint iAniState);

private: //Func Pointer
	//Set Behavior
	VOID Set_Behavior_Progress();

	//Behavior to State Func
	typedef VOID(CEnemy_Bowman::*BEHAVIOR)();
	BEHAVIOR AiState;
	VOID State_Awaken();
	VOID State_Hit();
	VOID State_KnockBack();
	VOID State_Airborne();
	VOID State_Idle();
	VOID State_Fallback ();
	VOID State_Chase();
	VOID State_Attack();
	VOID State_Dead();


private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);


private:
	//Key Input, Camera, NaviMesh
	_bool m_bAttack;
	_double m_AttackTime;
	_float m_fDist;
	_uint m_iCurAniSet;
	_uint m_iPreAniSet;

private:
	_bool m_bSleep;
	_bool m_bFront;
	_bool m_bDead;
	_bool m_bTwice;

	_bool m_bHit;
	_bool m_bKnockBack;
	_bool m_bAirborne;

private:
	_double m_TimeDelta;
	_double m_HitTime;
	_double m_AirTime;
	_double m_KnockTime;
	_double m_AccTime;

	//KnockBack
	_uint m_iKnockIdx[5];
	_uint m_iKnockCnt = 0;

private:
	_matrix m_pBoneMatrix;

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTransform*	m_pTargetTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CSphereColl*	m_pTargetSphereColl;
	ENGINE::CShader*			m_pShader;
	ENGINE::CWeapon*			m_pWeapon;

public:
	static CEnemy_Bowman* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Enemy_Bowman_h__
