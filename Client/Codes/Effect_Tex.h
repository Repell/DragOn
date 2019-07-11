#ifndef Effect_Tex_h__
#define Effect_Tex_h__

#include "GameObject.h"

namespace ENGINE
{
	class CRcTex;
	class CGameObject;
	class CTransform;
	class CTexture;
	class CRenderer;
}

class CEffect_Tex : public ENGINE::CGameObject
{
private:
	explicit CEffect_Tex(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect_Tex();

public:
	void Set_Pos(_vec3 vPos);
	void Set_LifeTime(_float fLife, _float fDelay = 0.f);

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();
	void Update_Frame(_float fTimeDelta);
	void Update_Billboard();

private:
	void Render_Set();
	void Render_ReSet();

private:
	ENGINE::CRcTex*					m_pBuffer;
	ENGINE::CTransform*		m_pTransform;
	ENGINE::CTexture*				m_pTexture;
	ENGINE::CRenderer*			m_pRenderer;

private:
	_float m_fFrame;
	_float m_fMaxFrame;
	_float m_fLifeTime;
	_float m_fTime;

public:
	static CEffect_Tex* Create(LPDIRECT3DDEVICE9 pDevice);
	static CEffect_Tex* Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vPos, _float fLife, _float fDelay = 0);
	virtual void Free();

};

#endif // Effect_Tex_h__
