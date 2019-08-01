#ifndef Enemy_Spearman_h__
#define Enemy_Spearman_h__

#include "GameObject.h"
#include "Effect_Tex.h"

namespace ENGINE
{
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CSphereColl;
	class CShader;
	class CWeapon;
}

class CEnemy_Spearman : public ENGINE::CGameObject
{
private:
	explicit CEnemy_Spearman(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEnemy_Spearman();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();
	void Render_BoneMatrix(const char* tBone);
	void Chase_Target(const _double& TimeDelta);
	void Attack_Target(const _double& TimeDelta);
	_bool Check_EnemyColl(_vec3* vRevDir, const _tchar* szTag);

private:
	VOID Animate_FSM(_uint iAniState);
	void Set_Animation();

private:
	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void Get_WeaponMatrix(const char* tBone);

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
	_bool m_bAirbone;

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

private:
	const _matrix* m_pBoneMatrix;

public:
	static CEnemy_Spearman* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Enemy_Spearman_h__
