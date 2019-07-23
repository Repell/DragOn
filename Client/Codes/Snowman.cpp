#include "stdafx.h"
#include "Snowman.h"

#include "Export_Function.h"

#define _SPEED 3.f
#define _ANGLE 60.f
#define  _RADIUS 125.f

CSnowman::CSnowman(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	bAttack = FALSE;
	iCurAniSet = 0;
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
	//m_pTransform->m_vAngle.y = 180.f;
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	//m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 40.f, 0.1f, 3.f };
	m_pSphereColl->Set_Scale(0.01f);
	m_pCollider->Set_Scale(0.01f);

	m_pTargetTransform = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransform, E_FAIL);

	m_pTargetSphereColl = dynamic_cast<ENGINE::CSphereColl*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_SphereColl", ENGINE::COMP_STATIC));
	NULL_CHECK_RETURN(m_pTargetSphereColl, E_FAIL);


	return S_OK;
}

_int CSnowman::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	//if (bAttack = m_pMesh->Is_AnimationSetEnd())
	//m_pMesh->Set_AnimationSet(6);

	if (iCurAniSet != 8)
		Chase_Target(TimeDelta);
	else if (iCurAniSet == 8)
		Attack_Target();

	if (m_pSphereColl->Get_iHp() <= 0)
		return END_EVENT;

	m_pMesh->Play_AnimationSet(TimeDelta);
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
	Find_BoneMatrix();
	m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix, _vec3(0.f, 0.f, 0.f));

	_tchar szStr[MAX_PATH] = L"";
	swprintf_s(szStr, L"Distnace : %3.2f", fDist);
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

void CSnowman::Find_BoneMatrix()
{
	//if(nullptr == m_pBoneMatrix)
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName("L_IRON2");
	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CSnowman::Chase_Target(const _double& TimeDelta)
{

	_bool bColl = m_pSphereColl->Check_ComponentColl(m_pTargetSphereColl);
	fDist = m_pTransform->Fix_TargetLookAngleY(m_pTargetTransform, 10.f);	//거리 반환, 반환값 안받아도상관없음

	if (!bAttack && fDist < 3.f || bColl)	//공격
	{
		iCurAniSet = 8;
		m_pMesh->Set_AnimationSet(8);
		bAttack = TRUE;
	}
	else	if (fDist < 10.f && !bColl)
	{
		m_pTransform->Stalk_Target(m_pTargetTransform, TimeDelta, _SPEED);
		m_pMesh->Set_AnimationSet(7);
	}
	else
		m_pMesh->Set_AnimationSet(6);
	
	m_pMesh->Is_AnimationSetEnd();
}

void CSnowman::Attack_Target()
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


HRESULT CSnowman::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Snowman"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, -1.f));
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
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 10);
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
