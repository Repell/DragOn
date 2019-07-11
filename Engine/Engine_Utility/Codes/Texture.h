#ifndef Texture_h__
#define Texture_h__

#include "Resourceses.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CTexture :
	public CResourceses
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT Ready_Texture(const _tchar* pPath, TEXTURETYPE eID, const _uint iCnt);
	void Set_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex = 0);
	void Render_Texture(const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*> m_vecTexture;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eID, const _uint& iCnt = 1);

public:
	virtual CResourceses* Clone(void) override;
	virtual void Free(void) override;

};

NS_END

#endif // Texture_h__
