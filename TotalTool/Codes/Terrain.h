#ifndef MFCTERRAIN_H__
#define MFCTERRAIN_H__

#include "GameObject.h"

namespace ENGINE
{
	class CTerrainTex;
	class CGameObject;
}

class CTerrain :
	public ENGINE::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();

public:
	ENGINE::CTransform* Get_Transform()
	{	return m_pTransCom;}
	ENGINE::CTerrainTex* Get_TerrainTex()
	{		return m_pBufferCom;	}
	void Set_Reset(bool bSet) { bReset = bSet; }
	void Set_Transform(const _vec3 vPos, const _vec3 vRot);

public:
	virtual HRESULT Ready_Object() override;
	virtual _int Update_Object(const _double& TimeDelta) override;
	virtual void Late_Update_Object() override;
	virtual void Render_Object() override;

private:
	HRESULT Add_Component();
	void Render_Set();
	void Render_ReSet();
	void Render_Font(const _vec2* pPos);
	void LineMaker();

public:
	void Release_Buffer();
	void Release_Texture();
	
	HRESULT Create_Buffer();
	HRESULT Set_Texture(const _tchar* cloneKey, const _int iTex);

private:
	ENGINE::CTerrainTex* m_pBufferCom;
	ENGINE::CTransform*	m_pTransCom;
	ENGINE::CTexture* m_pTexture;
	ENGINE::CRenderer* m_pRenderCom;

	//D3DX Fonts and Sprite
	LPD3DXSPRITE						m_pSprite;
	LPD3DXLINE							m_pLine;

	//Values
	_int					m_iTex;

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

private:
	_bool bReset;

};

#endif // MFCTERRAIN_h__
