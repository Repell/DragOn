#ifndef Shader_h__
#define Shader_h__

#include "Engine_Define.h"
#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	LPD3DXEFFECT Get_EffectHandle() { return m_pEffect; }

public:
	HRESULT			Ready_Shader(const wstring pFilePath);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;
	LPD3DXBUFFER				m_pErrMsg;

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pDevice, const wstring pFilePath);
	virtual CComponent* Clone();
	virtual void Free();

};

NS_END

#endif // Shader_h__
