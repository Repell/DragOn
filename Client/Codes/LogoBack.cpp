#include "stdafx.h"
#include "LogoBack.h"
#include "Export_Utility.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{
}

CLogoBack::~CLogoBack()
{
}

HRESULT CLogoBack::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };
	
	return S_OK;
}

_int CLogoBack::Update_Object(const _float & fTimeDelta)
{
	//ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);

	return NO_EVENT;
}

void CLogoBack::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CLogoBack::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
}

HRESULT CLogoBack::Add_Component()
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
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Texture_Title"));
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

	////////////////////////////
	return S_OK;
}

CLogoBack * CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack* pInstance = new CLogoBack(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CLogoBack::Free()
{
	ENGINE::CGameObject::Free();
}
