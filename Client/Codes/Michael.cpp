#include "stdafx.h"
#include "Michael.h"

#include "Export_Function.h"

#define SPEED 3.f
#define ANGLE 60.f
#define  RADIUS 200.f

CMichael::CMichael(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	bAttack = FALSE;
	
	iCurAniSet = 0;
}

CMichael::~CMichael()
{

}

HRESULT CMichael::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMesh->Set_AnimationSet(11);

	m_pNaviMesh->Set_CurrentIdx(0);
	m_pTransform->m_vAngle.y = 90.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_pSphereColl->Set_Scale(0.01f);
	m_pCollider->Set_Scale(0.01f);

	return S_OK;
}

HRESULT CMichael::Late_Init()
{


	return S_OK;
}

_int CMichael::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	//if (bAttack = m_pMesh->Is_AnimationSetEnd())
	//m_pMesh->Set_AnimationSet(6);

	if (m_pSphereColl->Get_iHp() <= 0)
		return END_EVENT;

	m_pMesh->Play_AnimationSet(TimeDelta);
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CMichael::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CMichael::Render_Object()
{
	//Render_Set();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();

	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);
	Find_BoneMatrix();
	m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix, _vec3(0.f, 1.5f, 0.f));

	//Render_ReSet();
}

void CMichael::Render_Set()
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

void CMichael::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CMichael::Find_BoneMatrix()
{
	//if(nullptr == m_pBoneMatrix)
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName("HEAD");
	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CMichael::Chase_Target(const _double& TimeDelta)
{

	_bool bColl = m_pSphereColl->Check_ComponentColl(m_pTargetSphereColl);
	fDist = m_pTransform->Fix_TargetLookAngleY(m_pTargetTransform, 10.f);

	if (!bAttack && fDist < 3.f || bColl)	//АјАн
	{
		iCurAniSet = 8;
		m_pMesh->Set_AnimationSet(8);
		bAttack = TRUE;
	}
	else	if (fDist < 10.f && !bColl)
	{
		m_pTransform->Stalk_Target(m_pTargetTransform, TimeDelta, SPEED);
		m_pMesh->Set_AnimationSet(7);
	}
	else
		m_pMesh->Set_AnimationSet(6);
	
	m_pMesh->Is_AnimationSetEnd();
}

void CMichael::Attack_Target()
{
	
	_bool bColl = m_pCollider->Check_ComponentColl(m_pTargetSphereColl);

	if (bColl && bAttack)
	{
		m_pTargetSphereColl->Get_iHp(2);
		bAttack = FALSE;
	}

	if (m_pMesh->Is_AnimationSetEnd())
	{
		m_pMesh->Set_AnimationSet(9);
		iCurAniSet = 9;
	}

	m_pMesh->Is_AnimationSetEnd();
}


HRESULT CMichael::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Michael"));
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

	//Collider
	pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev, 80.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);


	////////////////////////////
	return S_OK;
}

CMichael * CMichael::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CMichael* pInstance = new CMichael(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CMichael::Free()
{
	ENGINE::CGameObject::Free();
}
