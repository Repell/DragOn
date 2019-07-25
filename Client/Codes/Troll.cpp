#include "stdafx.h"
#include "Troll.h"

#include "Export_Function.h"

#define _SPEED 2.f
#define _ANGLE 60.f
#define  _RADIUS 140.f

CTroll::CTroll(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	m_bAttack = FALSE;
	m_bSleep = TRUE;
	m_bDead = FALSE;

	m_iCurAniSet = 0;
	m_iPreAniSet = 0;
}

CTroll::~CTroll()
{

}

HRESULT CTroll::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_FSM(14);

	m_pNaviMesh->Set_CurrentIdx(3);
	m_pTransform->m_vAngle.y = 0.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;

	return S_OK;
}

HRESULT CTroll::Late_Init()
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

_int CTroll::Update_Object(const _double& TimeDelta)
{
	m_TimeDelta = TimeDelta;
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_pSphereColl->Get_iHp() <= 0)
	{
		m_bDead = TRUE;
		m_pTransform->Get_Dead(TRUE);
	}

	if (m_bDead && m_iCurAniSet == 21 && m_pMesh->Is_AnimationSetEnd())
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CTroll::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	m_fDist = m_pTransform->Get_TargetDistance(m_pTargetTransform);
}

void CTroll::Render_Object()
{
	//Render_Set();

	if (m_bSleep && !m_bDead)
	{
		if (m_fDist < 10.f && m_iCurAniSet == 14)	//WakeUp
			Animate_FSM(15);
		else if (m_iCurAniSet == 15 && m_pMesh->Is_AnimationSetEnd())	//WakeUpEnd
		{
			m_pTransform->Fix_TargetLook(m_pTargetTransform, 20.f);
			Animate_FSM(5);
		}
		else if (m_iCurAniSet == 5 && m_pMesh->Is_AnimationSetEnd())
			m_bSleep = FALSE;
	}
	else if(!m_bDead)
	{
		if (m_iCurAniSet != 8 && m_fDist < 10.f && !m_bAttack)
			Chase_Target(m_TimeDelta);
		else if (m_bAttack && m_fDist < 10.f)
			Attack_Target();
		else
		{
			Animate_FSM(0);
		}
	}

	if (m_bDead)
			Animate_FSM(21);

	if(m_iCurAniSet == 2)
		m_pMesh->Play_AnimationSet(m_TimeDelta * 1.5);
	else
		m_pMesh->Play_AnimationSet(m_TimeDelta);
	/////////////////////

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();

	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);
	Find_BoneMatrix();
	m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix, _vec3(0.f, 0.f, 0.f));

	_tchar szStr[MAX_PATH] = L"";
	swprintf_s(szStr, L"AngleY : %3.2f", m_pTransform->m_vAngle.y);
	//swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CTroll::Render_Set()
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

void CTroll::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CTroll::Find_BoneMatrix()
{
	//if(nullptr == m_pBoneMatrix)
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName("L_IRON1");
	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CTroll::Chase_Target(const _double& TimeDelta)
{

	_bool bColl = m_pSphereColl->Check_ComponentColl(m_pTargetSphereColl);
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);	//거리 반환, 반환값 안받아도상관없음
	//m_bFront = m_pTransform->Check_TargetFront();

	if (!m_bAttack && m_fDist < 4.f /*&& m_bFront */&& bColl)	//공격
	{
		Animate_FSM(8);

		m_bAttack = TRUE;
	}
	else	if (m_fDist < 10.f && !bColl)	//추적
	{
		_vec3 vRevDir = {};
			
		if (Check_EnemyColl(&vRevDir))
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * TimeDelta;
		else
			m_pTransform->Stalk_Target(m_pTargetTransform, TimeDelta, _SPEED);

		Animate_FSM(2);
	}

}

void CTroll::Attack_Target()
{
	_bool bColl = m_pCollider->Check_ComponentColl(m_pTargetSphereColl);

	if (m_iCurAniSet == 8 && bColl && m_bAttack)
	{
		m_pTargetSphereColl->Get_iHp(2);
		m_pTargetSphereColl->m_bHit = TRUE;
		Animate_FSM(9);
		//m_bAttack = FALSE;
	}

	if (m_iCurAniSet == 8 && m_pMesh->Is_AnimationSetEnd())
		Animate_FSM(9);
	else if (m_iCurAniSet == 9 && m_pMesh->Is_AnimationSetEnd())
		m_bAttack = FALSE;

	
}

_bool CTroll::Check_EnemyColl(_vec3 * vRevDir)
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(L"Troll"))
	{
		if (pList == this)
			continue;

		ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pSphere == nullptr)
			return FALSE;

		*vRevDir = m_pTransform->Get_TargetReverseDir(pTrans) * 0.01f;

		if (m_pSphereColl->Check_ComponentColl(pSphere))
		{
			_vec3 vTargetRevDir = pTrans->Get_TargetReverseDir(m_pTransform)  * 0.01f;
			pTrans->m_vInfo[ENGINE::INFO_POS] += vTargetRevDir;
			return TRUE;
		}
		else
			continue;
	}

	return FALSE;
}

VOID CTroll::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (/*!m_bAnimate && */m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}


HRESULT CTroll::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Troll"));
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

CTroll * CTroll::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CTroll* pInstance = new CTroll(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CTroll::Free()
{
	ENGINE::CGameObject::Free();
}
