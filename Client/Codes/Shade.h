#ifndef Shade_h__
#define Shade_h__

#include "GameObject.h"
#include "Loading.h"
//#include "Shader.h"

namespace ENGINE
{
	class CRcTex;
	class CGameObject;
	class CTransform;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CShade : public ENGINE::CGameObject
{
private:
	explicit CShade(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CShade();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();

	HRESULT Ready_LogoLoad(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
	void Update_Frame(const _float& fTimeDelta);
	void Update_Frame2(const _float& fTimeDelta);

private:
	void Render_Set();
	void Render_ReSet();

private:
	ENGINE::CRcTex*					m_pBuffer;
	ENGINE::CTransform*		m_pTransform;
	ENGINE::CTexture*				m_pTexture;
	ENGINE::CRenderer*			m_pRenderer;
	ENGINE::CShader*				m_pShader;

public:
	static CShade* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // Shade_h__
