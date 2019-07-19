#include "stdafx.h"
#include "Snowman.h"

#include "Export_Function.h"

#define SPEED 5.f
#define ANGLE 60.f
#define  RADIUS 125.f

CSnowman::CSnowman(LPDIRECT3DDEVICE9 pDevice)
	:	CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	
}

CSnowman::~CSnowman()
{

}

HRESULT CSnowman::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMesh->Set_AnimationSet(6);

	m_pNaviMesh->Set_CurrentIdx(3);

	m_pTransform->m_vAngle.y = 0.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;

	return S_OK;
}

HRESULT CSnowman::Late_Init()
{
	m_pTransform->m_vAngle.y = 180.f;
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	//m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 40.f, 0.1f, 3.f };
	m_pSphereColl->Set_Scale(0.01f);

	return S_OK;
}

_int CSnowman::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);
	
	if(bAttack = m_pMesh->Is_AnimationSetEnd())
		m_pMesh->Set_AnimationSet(6);

	m_pMesh->Play_AnimationSet(TimeDelta);


	Chase_Target();

	if (m_pSphereColl->Get_iHp() <= 0)
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CSnowman::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CSnowman::Render_Object()
{
	//Render_Set();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();

	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);

	_tchar szStr[MAX_PATH] = L"";
	swprintf_s(szStr, L"iHp: %d", m_pSphereColl->Get_iHp());
	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CSnowman::Render_Set()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CSnowman::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CSnowman::Chase_Target()
{
	ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_SphereColl", ENGINE::COMP_STATIC));


	_vec3 vDist = pTrans->m_vInfo[ENGINE::INFO_POS] - m_pTransform->m_vInfo[ENGINE::INFO_POS];
	_float fDist = D3DXVec3Length(&vDist);

	//if (fDist > 1.f)
	bAttack = m_pSphereColl->Check_ComponentColl(pSphere);
	
	if (bAttack)
	{
		m_pMesh->Set_AnimationSet(8);
	}
	else if(!bAttack && fDist < 10.f)
		m_pTransform->Chase_Target(&pTrans->m_vInfo[ENGINE::INFO_POS], ENGINE::Get_DeltaTime());

}


HRESULT CSnowman::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh =dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Snowman"));
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

	//NaviMesh
	pComponent = m_pNaviMesh = dynamic_cast<ENGINE::CNaviMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Navi"));
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_NaviMesh", pComponent);

	////Collider
	//pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev,
	//	m_pMesh->Get_VtxMeshPos(), m_pMesh->Get_NumVtx(), m_pMesh->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);


	////////////////////////////
	return S_OK;
}

CSnowman * CSnowman::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSnowman* pInstance = new CSnowman(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CSnowman::Free()
{
	ENGINE::CGameObject::Free();
}
