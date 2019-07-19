#include "stdafx.h"
#include "Shade.h"
#include "Export_Function.h"

CShade::CShade(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CShade::~CShade()
{
}

HRESULT CShade::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LogoLoad(m_pGraphicDev, LOADING_LOGO), E_FAIL);

	m_pTransform->m_vAngle.x += 90.f;
	m_pTransform->m_vScale = {0.5f, 0.5f, 0.5f};
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = {0.f, 0.f, 0.f};
	
	return S_OK;
}

_int CShade::Update_Object(const _double& TimeDelta)
{
	//ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_ALPHA, this);

	return NO_EVENT;
}

void CShade::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>( ENGINE::Get_Management()->Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = pTrans->m_vInfo[ENGINE::INFO_POS];
	
}

void CShade::Render_Object()
{
	Render_Set();

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint iPassCnt = 0;
	_matrix matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pTransform->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pTexture->Set_Texture(pEffect, "g_BaseTexture");
	////
	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	//m_pTexture->Render_Texture(_uint(m_fFrame));
	m_pBuffer->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
	////

	Render_ReSet();
}

HRESULT CShade::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	//Buffer Component
	pComponent = m_pBuffer = dynamic_cast<ENGINE::CRcTex*>
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Buffer", pComponent);

	//Texture Componet
	pComponent = m_pTexture = dynamic_cast<ENGINE::CTexture*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Texture_Shade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Texture", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Transform", pComponent);

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	//Shader Component
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Shade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	////////////////////////////
	return S_OK;
}

HRESULT CShade::Ready_LogoLoad(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{

	return S_OK;
}


void CShade::Render_Set()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Alpha Test Begin
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Alpha Blend
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CShade::Render_ReSet()
{
	//Alpha Blend End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//Alpha Test End
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CShade * CShade::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShade* pInstance = new CShade(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CShade::Free()
{
	ENGINE::CGameObject::Free();
}
