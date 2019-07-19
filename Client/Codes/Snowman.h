#ifndef Snowman_h__
#define Snowman_h__

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

class CSnowman : public ENGINE::CGameObject
{
private:
	explicit CSnowman(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSnowman();

public:
	virtual HRESULT Ready_Object(_vec3 vPos);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();
	void Chase_Target();

private:
	//Key Input, Camera, NaviMesh
	_bool bAttack;

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CDynamicMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CNaviMesh*		m_pNaviMesh;
	//ENGINE::CCollider*			m_pCollider;
	ENGINE::CSphereColl*	m_pSphereColl;

public:
	static CSnowman* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void Free() override;


};

#endif // Snowman_h__
