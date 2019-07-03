#include "stdafx.h"
#include "Logo_Loading.h"
#include "Export_Function.h"

CLogo_Loading::CLogo_Loading(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_fFrame(0.f), m_fMaxFrame(5.f)	//0 ~ 4 / 5~6
	,bLoading(FALSE)
{
}

CLogo_Loading::~CLogo_Loading()
{
}

HRESULT CLogo_Loading::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LogoLoad(m_pGraphicDev, LOADING_LOGO), E_FAIL);

	m_pTransform->m_vScale = {1.f, 1.f, 1.f};
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = {0.f, 0.f, 0.f};
	
	return S_OK;
}

_int CLogo_Loading::Update_Object(const _float & fTimeDelta)
{
	//ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	if (!bLoading)
		Update_Frame(fTimeDelta);
	else if(bLoading)
		Update_Frame2(fTimeDelta);

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);

	return NO_EVENT;
}

void CLogo_Loading::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CLogo_Loading::Render_Object()
{
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pTexture->Render_Texture(m_fFrame);
	m_pBuffer->Render_Buffer();

//	ENGINE::Render_Font(L"Sp", m_pLoading->Get_String(), &_vec2(10.f, 10.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

HRESULT CLogo_Loading::Add_Component()
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

HRESULT CLogo_Loading::Ready_LogoLoad(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{

	return S_OK;
}

void CLogo_Loading::Update_Frame(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxFrame * fTimeDelta * 0.25f;
	
	if (m_fFrame >= m_fMaxFrame)
		m_fFrame = 0.f;

}

void CLogo_Loading::Update_Frame2(const _float & fTimeDelta)
{
	m_fMaxFrame = 7.f;

	m_fFrame += m_fMaxFrame * fTimeDelta * 0.25f;

	if (m_fFrame >= m_fMaxFrame)
		m_fFrame = 5.f;
}

CLogo_Loading * CLogo_Loading::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo_Loading* pInstance = new CLogo_Loading(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CLogo_Loading::Free()
{
	ENGINE::CGameObject::Free();
}
