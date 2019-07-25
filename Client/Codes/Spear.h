#ifndef Spear_h__
#define Spear_h__

#include "GameObject.h"

namespace ENGINE
{
	class CRenderer;
	class CStaticMesh;
	class CTransform;
	class CGameObject;
	class CCollider;
}

class CSpear : public ENGINE::CGameObject
{
private:
	explicit CSpear(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSpear();

public:
	void Set_AttackState(_bool bState);

public:
	virtual HRESULT Ready_Object(const _uint& iFlag);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();
	void Check_EnemyColl(const _tchar* pObjTag, const _tchar* pCompTag);
	void Get_ParentMatrix();

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CStaticMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CCollider*			m_pCollider;

private:
	_bool bAttack;
	_uint				m_iFlag;
	const _matrix*		m_pParentBoneMatrix;
	const _matrix*		m_pParentWorldMatrix;


public:
	static CSpear* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);
	virtual void Free() override;



};

#endif // Spear_h__
