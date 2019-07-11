#include "Shader.h"

USING(ENGINE)

CShader::CShader(LPDIRECT3DDEVICE9 pDevice)
	: m_pGraphicDev(pDevice),
	m_pEffect(nullptr)
{
	m_pGraphicDev->AddRef();
}

CShader::CShader(const CShader & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev),
	m_pEffect(rhs.m_pEffect)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

CShader::~CShader()
{

}

HRESULT CShader::Ready_Shader(const wstring pFilePath)
{
	// pEffect , pErrMsg

	// 에러 : pEffect = nullptr, pErrMsg = 값이 있다
	// 경고 : pEffect = 값이 있다, pErrMsg = 값이 있다
	// 정상 : pEffect = 값이 있다, pErrMsg = nullptr

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath.c_str(), NULL, NULL,
		D3DXSHADER_DEBUG, NULL, &m_pEffect, &m_pErrMsg)))
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}
	else if(nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Warning", MB_OK);
	}

	return S_OK;
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pDevice, const wstring pFilePath)
{
	CShader* pInstance = new CShader(pDevice);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);
	
	return pInstance;
}

CComponent * CShader::Clone()
{
	return new CShader(*this);
}

void CShader::Free()
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}
