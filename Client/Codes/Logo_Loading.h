#ifndef Logo_Loading_h__
#define Logo_Loading_h__

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

class CLogo_Loading : public ENGINE::CGameObject
{
private:
	explicit CLogo_Loading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo_Loading();

public:
	void Set_Loading(_bool bb) { bLoading = bb; }

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
	ENGINE::CRcTex*					m_pBuffer;
	ENGINE::CTransform*		m_pTransform;
	ENGINE::CTexture*				m_pTexture;
	ENGINE::CRenderer*			m_pRenderer;
	ENGINE::CShader*				m_pShader;

	//Frame
	_float m_fFrame;
	_float m_fMaxFrame;
	_bool bLoading;

public:
	static CLogo_Loading* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // Logo_Loading_h__
