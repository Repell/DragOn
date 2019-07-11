#include "stdafx.h"
#include "Sword.h"

#include "Export_Function.h"

CSword::CSword(LPDIRECT3DDEVICE9 pDevice)
	:	CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr),
	m_pParentBoneMatrix(nullptr), m_pParentWorldMatrix(nullptr)
{
}

CSword::~CSword()
{
}

HRESULT CSword::Ready_Object(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(57);
	m_iFlag = iFlag;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	m_pTransform->m_vAngle.x = 90.f;

	return S_OK;
}

HRESULT CSword::Late_Init()
{

	//m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 4.f, 2.f, 4.f };

	return S_OK;
}

_int CSword::Update_Object(const _float & fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();

	if (nullptr == m_pParentBoneMatrix)
	{
		ENGINE::CDynamicMesh* pPlayerMesh = dynamic_cast<ENGINE::CDynamicMesh*>
			(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Mesh", ENGINE::COMP_DYNAMIC));

		if (nullptr == pPlayerMesh)
			return 0;

		const ENGINE::D3DXFRAME_DERIVED* pFrame = pPlayerMesh->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->combinedTransformMatrix;

		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (nullptr == pTrans)
			return 0;

		m_pParentWorldMatrix = &(pTrans->m_matWorld);
	}


	ENGINE::CGameObject::Update_Object(fTimeDelta);

	//m_pMesh->Play_AnimationSet(fTimeDelta);

	m_pTransform->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
	
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CSword::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CSword::Render_Object()
{
	//Render_Set();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
	m_pMesh->Render_Meshes();

	//Render_ReSet();
}

void CSword::Render_Set()
{
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CSword::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CSword::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh =dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Sword"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	////////////////////////////
	return S_OK;
}

CSword * CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CSword* pInstance = new CSword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CSword::Free()
{
	ENGINE::CGameObject::Free();
}
