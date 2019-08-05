#include "Aura.h"
#include "Export_Utility.h"

USING(ENGINE)

CAura::CAura(LPDIRECT3DDEVICE9 pDevice)
	: m_pGraphicDev(pDevice)
{
	m_pGraphicDev->AddRef();
}


HRESULT CAura::Ready_Component(wstring str)
{
	//Shader 
	m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	m_pBuffer = dynamic_cast<ENGINE::CRcTex*>(ENGINE::Clone_Resources(0, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);

	m_pTexture = dynamic_cast<ENGINE::CTexture*>(ENGINE::Clone_Resources(1, str.c_str()));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);

	return S_OK;
}

HRESULT CAura::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_matWorld);

	pEffect->SetMatrix("g_matView", &matView);

	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTexture->Set_Texture(pEffect, "g_BaseTexture");

	ENGINE::Safe_Release(pEffect);

	return S_OK;
}

_int CAura::Update_Component(const _double & TimeDelta)
{
	return NO_EVENT;
}

void CAura::Render_Shadow(const _matrix * pMatrix, _float fSize, _float fY)
{
	D3DXMatrixIdentity(&m_matWorld);
	_matrix matScale, matRot;
	D3DXMatrixScaling(&matScale, fSize, fSize, fSize);
	D3DXMatrixRotationX(&matRot, D3DXToRadian	(90.f));
	memcpy(&m_matWorld.m[3][0], &pMatrix->m[3][0], sizeof(_vec3));
	m_matWorld._42 = fY;
	m_matWorld = matScale * matRot * m_matWorld;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	///////////////////////////////////////////	Shader Transform

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	///////////////////////////////////////

	pEffect->Begin(0, 0);
	pEffect->BeginPass(0);

	////////////////////////////////////////	Shader Target

	m_pBuffer->Render_Buffer();

	////////////////////////////////////////

	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	//////////////////////////////////////	Shader End

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}


CAura * CAura::Create(LPDIRECT3DDEVICE9 pGraphic, wstring str)
{
	CAura* pInstance = new CAura(pGraphic);

	if (FAILED(pInstance->Ready_Component(str)))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CAura::Free()
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pGraphicDev);

}
