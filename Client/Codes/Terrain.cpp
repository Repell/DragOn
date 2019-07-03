#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice),
	m_pBuffer(nullptr), m_pTexture(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransform->m_vScale = {1.f, 1.f, 1.f};

	return S_OK;
}

HRESULT CTerrain::Late_Init()
{

	return S_OK;
}

_int CTerrain::Update_Object(const _float & fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);
	
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);

	return NO_EVENT;
}

void CTerrain::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CTerrain::Render_Object()
{
	Render_Set();

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

	Render_ReSet();
}

HRESULT CTerrain::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	//Buffer Component
	pComponent = m_pBuffer = dynamic_cast<ENGINE::CTerrainTex*>
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Buffer_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Buffer", pComponent);

	//Texture Componet
	pComponent = m_pTexture = dynamic_cast<ENGINE::CTexture*>
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Texture_Terrain"));
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

void CTerrain::Render_Set()
{
	//Set Matarial
	Setup_Material();

	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CTerrain::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CTerrain::Setup_Material()
{
	D3DMATERIAL9 dMatrial;
	ZeroMemory(&dMatrial, sizeof(D3DMATERIAL9));

	dMatrial.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//��ǻ��(�ֺ���) :: ǥ���� ��� ���鿡 �����ϰ� �������� ��
	dMatrial.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	//�����Ʈ(Ȯ�걤) :: ���� ��� ���(������ ������ ��� �鿡�� ������ ��)
	dMatrial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//����ŧ��(�ݻ籤) :: Ư���� �������� �ݻ��ϴ� ��, ������ ��ġ�� ī�޶��� ��ġ�� ���� �޶���
	dMatrial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	//�̹̽ú�(���Ɽ) :: �޽� ǥ�鿡�� ��ü������ �����ϴ� ��, �ٸ� �޽ÿ� ������ ���� ����
	dMatrial.Power = 1.f;

	m_pGraphicDev->SetMaterial(&dMatrial);
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain* pInstance = new CTerrain(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free()
{
	ENGINE::CGameObject::Free();
}
