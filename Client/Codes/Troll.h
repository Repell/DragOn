#ifndef Troll_h__
#define Troll_h__

#include "GameObject.h"
#include "Effect_Tex.h"

namespace ENGINE
{
	class CRenderer;
	class CDynamicMesh;
	class CTransform;
	class CGameObject;
	class CNaviMesh;
	class CCollider;
	class CSphereColl;
}

class CTroll : public ENGINE::CGameObject
{
private:
	explicit CTroll(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTroll();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();
	void Find_BoneMatrix();
	void Chase_Target(const _double& TimeDelta);
	void Attack_Target();
	_bool Check_EnemyColl(_vec3* vRevDir);

private:
	VOID Animate_FSM(_uint iAniState);

private:
	//Key Input, Camera, NaviMesh
	_bool m_bAttack;
	_float m_fDist;
	_uint m_iCurAniSet;
	_uint m_iPreAniSet;

private:
	_bool m_bSleep;
	_bool m_bFront;
	_bool m_bDead;

private:
	_double m_TimeDelta;
private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTransform*	m_pTargetTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	ENGINE::CCollider*			m_pCollider;
	ENGINE::CSphereColl*	m_pSphereColl;
	ENGINE::CSphereColl*	m_pTargetSphereColl;

private:
	_matrix m_pBoneMatrix;

public:
	static CTroll* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Troll_h__
