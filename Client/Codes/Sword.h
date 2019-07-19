#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"

namespace ENGINE
{
	class CRenderer;
	class CStaticMesh;
	class CTransform;
	class CGameObject;
	class CCollider;
}

class CSword : public ENGINE::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSword();

public:
	void Set_bAttack(_bool bState);

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
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);
	virtual void Free() override;



};

#endif // Sword_h__
