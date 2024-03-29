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
	void Set_LifeTime(_double fLife, _double fDelay = 0.0);

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();
	void Update_Frame(const _double& TimeDelta);
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
	_double m_Frame;
	_double m_MaxFrame;
	_double m_LifeTime;
	_double m_Time;

public:
	static CEffect_Tex* Create(LPDIRECT3DDEVICE9 pDevice);
	static CEffect_Tex* Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vPos, _double fLife, _double fDelay = 0);
	virtual void Free();

};

#endif // Effect_Tex_h__
