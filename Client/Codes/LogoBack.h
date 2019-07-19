#ifndef LogoBack_h__
#define LogoBack_h__

#include "GameObject.h"

namespace ENGINE
{
	class CRcTex;
	class CGameObject;
	class CTransform;
	class CTexture;
	class CRenderer;
}

class CLogoBack : public ENGINE::CGameObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogoBack();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();

private:
	ENGINE::CRcTex*					m_pBuffer;
	ENGINE::CTransform*		m_pTransform;
	ENGINE::CTexture*				m_pTexture;
	ENGINE::CRenderer*			m_pRenderer;

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // LogoBack_h__
