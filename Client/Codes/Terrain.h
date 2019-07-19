#ifndef CTerrain_h__
#define CTerrain_h__

#include "GameObject.h"

namespace ENGINE
{
	class CGameObject;
	class CTerrainTex;
	class CTexture;
	class CTransform;
	class CRenderer;
	class CShader;
}

class CTerrain : public ENGINE::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_Object() override;
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _double& TimeDelta);
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	void Render_Set();
	void Render_ReSet();
	void Setup_Material();

private:
	ENGINE::CTerrainTex*	 m_pBuffer;
	ENGINE::CTexture*			m_pTexture;
	ENGINE::CTransform*	m_pTransform;
	ENGINE::CRenderer*		m_pRenderer;
	ENGINE::CShader*			m_pShader;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free();

};

#endif // CTerrain_h__
