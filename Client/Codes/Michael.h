#ifndef Michael_h__
#define Michael_h__

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

class CMichael : public ENGINE::CGameObject
{
private:
	explicit CMichael(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMichael();

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

private:
	//Key Input, Camera, NaviMesh
	_bool bAttack;
	_float fDist;
	_uint iCurAniSet;
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
	static CMichael* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Michael_h__
