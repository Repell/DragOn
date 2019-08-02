#ifndef Shadow_h__
#define Shadow_h__

#include "Component.h"

NS_BEGIN(ENGINE)

class CRcTex;
class CShader;
class CTexture;
class ENGINE_DLL CShadow : public CComponent
{
private:
	explicit CShadow(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CShadow() = default;

public:
	virtual _int Update_Component(const _double& TimeDelta) override;
	void Render_Shadow(const _matrix* pMatrix, _float fSize);
	
private:
	HRESULT Ready_Component();

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
	static CShadow* Create(LPDIRECT3DDEVICE9 pGraphic);
	virtual void Free();
	
};

NS_END	

#endif // Shadow_h__
