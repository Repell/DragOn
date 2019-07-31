#include "stdafx.h"
#include "Enemy_Swordman.h"

#include "Export_Function.h"

#define _SPEED 1.5f
#define _ANGLE 60.f
#define  _RADIUS 100.f

CEnemy_Swordman::CEnemy_Swordman(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	m_bAttack = FALSE;
	m_bSleep = TRUE;
	m_bDead = FALSE;

	m_iCurAniSet = 0;
	m_iPreAniSet = 0;
}

CEnemy_Swordman::~CEnemy_Swordman()
{

}

HRESULT CEnemy_Swordman::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_FSM(0);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { 0.006f, 0.006f, 0.006f };
	m_pSphereColl->Set_Scale(0.005f);
	//m_pCollider->Set_Scale(0.005f);

	return S_OK;
}

HRESULT CEnemy_Swordman::Late_Init()
{
	//m_pTransform->m_vAngle.y = 180.f;
	//m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 40.f, 0.1f, 3.f };

	m_pTargetTransform = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransform, E_FAIL);

	m_pTargetSphereColl = dynamic_cast<ENGINE::CSphereColl*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_SphereColl", ENGINE::COMP_STATIC));
	NULL_CHECK_RETURN(m_pTargetSphereColl, E_FAIL);


	return S_OK;
}

_int CEnemy_Swordman::Update_Object(const _double& TimeDelta)
{
	m_TimeDelta = TimeDelta;
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_pSphereColl->Get_iHp() <= 0)
	{
		m_bDead = TRUE;
		m_pTransform->Get_Dead(TRUE);
	}

	if (m_bDead && m_iCurAniSet == 30 && m_pMesh->Is_AnimationSetEnd())
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CEnemy_Swordman::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	m_fDist = m_pTransform->Get_TargetDistance(m_pTargetTransform);
}

void CEnemy_Swordman::Render_Object()
{
	//Render_Set();
	Get_WeaponMatrix("R_Hand");

	//if (m_bSleep && !m_bDead)
	//{
	//	if (m_fDist < 10.f && m_iCurAniSet == 0)	//WakeUp
	//		Animate_FSM(0);
	//	else if (m_iCurAniSet == 0 && m_pMesh->Is_AnimationSetEnd())	//WakeUpEnd
	//	{
	//		m_pTransform->Fix_TargetLook(m_pTargetTransform, 20.f);
	//		Animate_FSM(16);
	//	}
	//	else if (m_iCurAniSet == 16 && m_pMesh->Is_AnimationSetEnd())
	//		m_bSleep = FALSE;
	//}
	//else if (!m_bDead)
	//{
	//	if (m_iCurAniSet != 7 && m_fDist < 10.f && !m_bAttack)
	//		Chase_Target(m_TimeDelta);
	//	else if (m_bAttack && m_fDist < 5.f)
	//		Attack_Target();
	//	else
	//	{
	//		m_bAttack = FALSE;
	//		m_pTransform->m_bAttackState = m_bAttack;
	//		Animate_FSM(0);
	//	}
	//}

	if (m_bSleep && !m_bDead)	//생성 후 기본 대기 루프
	{
		if (m_fDist < 10.f && m_iCurAniSet == 0) //대기 상태에서 플레이어 발견
		{
			Animate_FSM(16);	//사기 진작
			m_pTransform->Fix_TargetLook(m_pTargetTransform, 20.f);
			
		}
		else if (m_iCurAniSet == 16 && m_pMesh->Is_AnimationSetEnd())	//사기 진작 종료, 행동 시작
		{
			m_bSleep = FALSE;
			Animate_FSM(0);
		}

	}
	else if (!m_bAttack && !m_bDead)	//플레이어 추적 시작
	{
		if (m_fDist < 10.f && !m_bAttack)
		{
			Chase_Target(m_TimeDelta);
		}


	}





	if (m_bDead)
	{
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		Animate_FSM(30);
	}

	m_pMesh->Play_AnimationSet(m_TimeDelta * 0.8);
	/////////////////////////////////////////////
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	////////////////////////////////////////

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();


	////////////////////////////////////////
	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	//if (m_bAttack && !m_bDead)
	m_pWeapon->Render_Weapon(m_pBoneMatrix);

	if (!m_bDead)
		m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);
	//if(m_bAttack)
	//	m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix);


	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"AngleY : %3.2f", m_pTransform->m_vAngle.y);
	////swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CEnemy_Swordman::Render_Set()
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

void CEnemy_Swordman::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CEnemy_Swordman::Render_BoneMatrix(const char* tBone)
{
	//if(nullptr == m_pBoneMatrix)
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);
	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;

	//m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix);
}

void CEnemy_Swordman::Chase_Target(const _double& TimeDelta)
{

	_bool bColl = m_pSphereColl->Check_ComponentColl(m_pTargetSphereColl);
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);	//거리 반환, 반환값 안받아도상관없음
	//m_bFront = m_pTransform->Check_TargetFront();

	if (!m_bAttack && m_fDist < 2.5f /*&& m_bFront */&& bColl)	//공격
	{
		Animate_FSM(6);

		m_bAttack = TRUE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_pWeapon->Set_AttackState(m_bAttack, m_iCurAniSet, 2);
	}
	else	if (m_fDist < 4.f && !bColl && !m_bDead)	//근거리 추적
	{
		_vec3 vRevDir = {};

		if (Check_EnemyColl(&vRevDir))
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * 0.75f * TimeDelta;
		else
			m_pTransform->Stalk_Target(m_pTargetTransform, TimeDelta, 0.75f);

		Animate_FSM(2);
	}
	else	if (m_fDist < 10.f && !bColl && !m_bDead)	//먼거리 추적
	{
		_vec3 vRevDir = {};

		if (Check_EnemyColl(&vRevDir))
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * TimeDelta;
		else
			m_pTransform->Stalk_Target(m_pTargetTransform, TimeDelta, _SPEED);

		Animate_FSM(1);
	}


}

void CEnemy_Swordman::Attack_Target()
{
	/*_bool bColl = m_pCollider->Check_ComponentColl(m_pTargetSphereColl);*/
	_bool bColl = m_pWeapon->Check_ComponentColl(m_pTargetSphereColl);

	if (m_pTargetSphereColl->m_bInvisible)
	{
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		return;
	}

	if (m_iCurAniSet == 6 && bColl && m_bAttack)
	{

		if (m_pTargetSphereColl->m_bHit)
		{
			m_pTargetSphereColl->m_bKnockBack = TRUE;
			_vec3 vKnockDir = m_pTargetSphereColl->Get_CollPos() - m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
			vKnockDir.y = 0.f;
			D3DXVec3Normalize(&vKnockDir, &vKnockDir);
			m_pTargetSphereColl->Set_KnockBackDist(TRUE, vKnockDir);
		}
		else
			m_pTargetSphereColl->m_bHit = TRUE;

		if (!m_pTargetSphereColl->m_bInvisible)
			m_pTargetSphereColl->Get_iHp(2);

		Animate_FSM(7);
		//m_bAttack = FALSE;
	}

	if (m_iCurAniSet == 7 && m_pMesh->Is_AnimationSetEnd() && m_bAttack)
	{
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_pWeapon->Set_AttackState(m_bAttack, m_iCurAniSet);
		Animate_FSM(0);
		return;
	}
	//else if (m_iCurAniSet == 9 && m_pMesh->Is_AnimationSetEnd())
	//{
	//	m_bAttack = FALSE;
	//	m_pTransform->m_bAttackState = m_bAttack;
	//	Animate_FSM(0);
	//	return;
	//}


}

_bool CEnemy_Swordman::Check_EnemyColl(_vec3 * vRevDir)
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(L"Enemy_Swordman"))
	{
		if (pList == this)
			continue;

		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pTrans->Get_Dead())
			continue;

		ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (pSphere == nullptr)
			return FALSE;

		*vRevDir = m_pTransform->Get_TargetReverseDir(pTrans) * 0.05f;

		if (m_pSphereColl->Check_ComponentColl(pSphere))
		{

			_vec3 vTargetRevDir = pTrans->Get_TargetReverseDir(m_pTransform)  * 0.05f;
			vTargetRevDir.y = 0.f;

			if (pTrans->m_bAttackState == FALSE)
				pTrans->m_vInfo[ENGINE::INFO_POS] += vTargetRevDir;

			return TRUE;
		}
		else
			continue;
	}

	return FALSE;
}

VOID CEnemy_Swordman::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (/*!m_bAnimate && */m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}

HRESULT CEnemy_Swordman::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();
	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pTransform->m_matWorld);

	pEffect->SetMatrix("g_matView", &matView);

	pEffect->SetMatrix("g_matProj", &matProj);

	ENGINE::Safe_Release(pEffect);

	return S_OK;
}

void CEnemy_Swordman::Get_WeaponMatrix(const char* tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}


HRESULT CEnemy_Swordman::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Enemy_Swordman"));
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
	//pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev, 50.f, _vec3(0.f, 0.f, -100.f));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	ENGINE::UNITINFO tInfo =
	{ FALSE, _vec3(0.f, 0.f, -100.f), _vec3{ 0.01f, 1.f, 1.f }, _vec3(90.f, 0.f, 0.f), 100.f };
	pComponent = m_pWeapon = ENGINE::CWeapon::Create(m_pGraphicDev, m_pTransform, tInfo, L"Mesh_Enemy_Sword");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Weapon", pComponent);

	////////////////////////////
	return S_OK;
}

CEnemy_Swordman * CEnemy_Swordman::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEnemy_Swordman* pInstance = new CEnemy_Swordman(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CEnemy_Swordman::Free()
{
	ENGINE::CGameObject::Free();
}
