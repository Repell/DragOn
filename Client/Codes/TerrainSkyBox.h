#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
//#include "Camera.h"

namespace ENGINE
{
	class CSkyBox;
	class CTransform;
	class CTexture;
	class CRenderer;
	class CGameObject;
}

class CTerrainSkyBox :
	public ENGINE::CGameObject
{
private:
	explicit CTerrainSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrainSkyBox();

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	void Render_Set();
	void Render_ReSet();

private:
	void Get_CameraPos();

private:
	HRESULT Add_Component();

private:
	ENGINE::CSkyBox* m_pBuffer;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CTexture* m_pTexture;
	ENGINE::CRenderer* m_pRenderer;

public:
	static CTerrainSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free() override;

};

#endif // SkyBox_h__
