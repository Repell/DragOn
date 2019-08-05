#ifndef Aura_h__
#define Aura_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class CRcTex;
class CShader;
class CTexture;
class ENGINE_DLL CAura : public CComponent
{
private:
	explicit CAura(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CAura() = default;

public:
	virtual _int Update_Component(const _double& TimeDelta) override;
	void Render_Shadow(const _matrix* pMatrix, _float fSize, _float fY = 0.f);
	
private:
	HRESULT Ready_Component(wstring str);

	//Shader
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CRcTex*			m_pBuffer;
	CShader*		m_pShader;
	CTexture*		m_pTexture;

private:		//Transform
	_matrix m_matWorld;
	
public:
	static CAura* Create(LPDIRECT3DDEVICE9 pGraphic, wstring str);
	virtual void Free();
	
};

NS_END	

#endif // Aura_h__
