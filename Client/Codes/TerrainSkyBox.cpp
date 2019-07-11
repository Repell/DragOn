#include "stdafx.h"
#include "TerrainSkyBox.h"

#include "Export_Function.h"

CTerrainSkyBox::CTerrainSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_pBuffer(nullptr), m_pTexture(nullptr), m_pRenderer(nullptr), m_pTransform(nullptr)
{
}

CTerrainSkyBox::~CTerrainSkyBox()
{
}

HRESULT CTerrainSkyBox::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransCom->m_vInfo[Engine::INFO_POS] = { TERRAIN_VTX_X * (TERRAIN_INTRV* 0.5f), 0.f, TERRAIN_VTX_Z * (TERRAIN_INTRV* 0.5f) };

	m_pTransform->m_vScale = {100.f, 100.f, 100.f};

	return S_OK;
}

_int CTerrainSkyBox::Update_Object(const _float& fTimeDelta)
{
	Get_CameraPos();
	// ▲ 프레임 갱신 전 렌더
	ENGINE::CGameObject::Update_Object(fTimeDelta);
	// ▼ 프레임 갱신 후 렌더

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);

	return 0;
}

void CTerrainSkyBox::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

}

void CTerrainSkyBox::Render_Object()
{
	//Render_Set();
	//Cull Mode			//SkyBox Only CW
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
	////////////////////

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

	////////////////////
	//Render_ReSet();

	//Cull Mode		//Default CCW
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTerrainSkyBox::Render_Set()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Cull Mode			//SkyBox Only CW
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//Z Buffer
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CTerrainSkyBox::Render_ReSet()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//Z Buffer 
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//Cull Mode		//Default CCW
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTerrainSkyBox::Get_CameraPos()
{
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = {matView._41, matView._42, matView._43};
	m_pTransform->m_vAngle.y += 0.036f;
}

HRESULT CTerrainSkyBox::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;

	//Buffer Compnent
	pComponent = m_pBuffer = dynamic_cast<ENGINE::CSkyBox*>
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Buffer_SkyBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Buffer", pComponent);

	//Texture Component
	pComponent = m_pTexture = dynamic_cast<ENGINE::CTexture*>
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Texture_SkyBox"));
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Texture", pComponent);

	//Transform
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CTerrainSkyBox * CTerrainSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainSkyBox* pInstance = new CTerrainSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CTerrainSkyBox::Free()
{
	ENGINE::CGameObject::Free();
}

