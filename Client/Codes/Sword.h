#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"

namespace ENGINE
{
	class CRenderer;
	class CStaticMesh;
	class CTransform;
	class CGameObject;
}

class CSword : public ENGINE::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSword();

public:
	virtual HRESULT Ready_Object(const _uint& iFlag);
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();

private:
	HRESULT Add_Component();

private:
	ENGINE::CRenderer* m_pRenderer;
	ENGINE::CStaticMesh* m_pMesh;
	ENGINE::CTransform*	m_pTransform;

private:
	_vec3				m_vDir;
	_uint				m_iFlag;
	const _matrix*		m_pParentBoneMatrix;
	const _matrix*		m_pParentWorldMatrix;

public:
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);
	virtual void Free() override;



};

#endif // Sword_h__
