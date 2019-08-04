#include "stdafx.h"
#include "Enemy_Swordman.h"

#include "Export_Function.h"

#define _SPEED 1.5f
#define _ANGLE 60.f
#define  _RADIUS 100.f

//STATE DEFINE
#define _IDLE								0
#define _CHASE_RUN					1
#define _CHASE_WALK				2
#define _ATTACK_START			6
#define	_ATTACK_END				7
#define _AWAKEN						16
#define _HIT									18
#define _AIRBORNE						20
#define _KnockBack_A				26
#define _KnockBack_B				27
#define _KnockBack_C				28
#define _KnockBack_D				29
#define  _DEAD							30
#define _RISEUP							35	//기상


CEnemy_Swordman::CEnemy_Swordman(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr),
	m_AttackTime(0.0)
{
	m_bAttack = FALSE;
	m_bSleep = TRUE;
	m_bDead = FALSE;
	m_bTwice = FALSE;

	m_iCurAniSet = _IDLE;
	m_iPreAniSet = _IDLE;

	m_HitTime = 0.0;
	m_AttackTime = 0.0;
	m_AirTime = 0.0;
	m_KnockTime = 0.0;
	m_TimeDelta = 0.0;
	m_AccTime = 1.0;
}

CEnemy_Swordman::~CEnemy_Swordman()
{

}

HRESULT CEnemy_Swordman::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_FSM(_IDLE);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { 0.006f, 0.006f, 0.006f };
	m_pSphereColl->Set_Scale(0.004f);
	//m_pCollider->Set_Scale(0.005f);

	Set_Animation();

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

	//Dead State
	if (m_pSphereColl->Get_iHp() <= 0)
	{
		m_bDead = TRUE;
		m_pTransform->Get_Dead(TRUE);
	}

	if (m_bDead && m_iCurAniSet == _DEAD && m_pMesh->Is_AnimationSetEnd())
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
	//Total Enemy Behavior
	Set_Behavior_Progress();

	//Play Animation
	m_pMesh->Play_AnimationSet(m_TimeDelta *  m_AccTime);
	/////////////////////////////////////////////	Shader Transform

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	////////////////////////////////////////	Shader Target

	m_pMesh->Render_Meshes();

	////////////////////////////////////////

	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	////////////////////////////////////////	Shader End
	Get_WeaponMatrix("R_Hand");
	m_pWeapon->Render_Weapon(&m_pBoneMatrix);

	if (!m_bDead)
		m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld, 0.75f);

	m_pShadow->Render_Shadow(&m_pTransform->m_matWorld, 0.45f);

	////////////////////////////////////////

	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"AngleY : %3.2f", m_pTransform->m_vAngle.y);
	////swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

//void CEnemy_Swordman::Render_Set()
//{
//	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//	//Alpha Test Begin
//	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
//	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
//
//	//Render State
//	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//	//SetTransform
//	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
//}
//
//void CEnemy_Swordman::Render_ReSet()
//{
//	//Alpha Test End
//	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	//FillMode :: Default == D3DFILL_SOLID
//	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
//}


void CEnemy_Swordman::Check_EnemyGroup()
{
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	_vec3 vRevDir = { 0.f, 0.f, 0.f };	//주변에 걸리적 거리는 놈 있으면 밀어내는 거리

	if (Check_EnemyColl(&vRevDir, L"Enemy_Swordman"))	//객체 별 충돌 체크
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Spearman"))	//객체 별 충돌 체크
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Shieldman"))	//객체 별 충돌 체크
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Bowman"))	//객체 별 충돌 체크
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

}

_bool CEnemy_Swordman::Check_EnemyColl(_vec3 * vRevDir, const _tchar* szTag)
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(szTag))
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

		*vRevDir = m_pTransform->Get_TargetReverseDir(pTrans);
		vRevDir->y = 0.f;

		if (m_pSphereColl->Check_ComponentColl(pSphere))
		{
			_vec3 vTargetRevDir = pTrans->Get_TargetReverseDir(m_pTransform);
			vTargetRevDir.y = 0.f;

			if (pTrans->m_bAttackState == FALSE)
				pTrans->m_vInfo[ENGINE::INFO_POS] += vTargetRevDir * m_TimeDelta * 1.1f;
			return TRUE;
		}

	}

	return FALSE;
}

VOID CEnemy_Swordman::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}

void CEnemy_Swordman::Set_Animation()
{
	m_iKnockCnt = 0;
	m_iKnockIdx[0] = _KnockBack_A;
	m_iKnockIdx[1] = _KnockBack_B;
	m_iKnockIdx[2] = _KnockBack_C;
	m_iKnockIdx[3] = _KnockBack_D;
	m_iKnockIdx[4] = _RISEUP;
}

VOID CEnemy_Swordman::Set_Behavior_Progress()
{
	/// 추후 Bool 값으로 제어되는 상태를 하나의 단일 FLAG 연산으로 개선하여 단일 변수로 수정 _uint m_StateFlag , Swith로 제어
	//Check State
	m_bHit = m_pSphereColl->Get_HitState();
	m_bKnockBack = m_pSphereColl->Get_KnockBackState();
	m_bAirborne = m_pSphereColl->Get_AirboneState();

	if (!m_bDead)	//안죽었을 때
	{
		//State Awaken
		if (m_bSleep)	//생성 후 기본 대기 루프
			AiState = &CEnemy_Swordman::State_Awaken;
		//State Hit
		else if (m_bHit && !m_bKnockBack && !m_bAirborne)	//공격 명중 시, 경직
			AiState = &CEnemy_Swordman::State_Hit;
		//State KnockBack
		else if (m_bKnockBack)		//넉백 공격 받음
			AiState = &CEnemy_Swordman::State_KnockBack;
		//State Airbone
		else if (m_bAirborne && !m_bKnockBack)	//에어본 공격받음
			AiState = &CEnemy_Swordman::State_Airborne;
		//State Chase
		else if (!m_bAttack && !m_bHit && m_fDist < 10.f)	//플레이어 추적 시작
			AiState = &CEnemy_Swordman::State_Chase;
		//State Attack
		else if (m_bAttack && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CEnemy_Swordman::State_Attack;

		//State Idle
		if (m_fDist > 10.f && !m_bHit && !m_bKnockBack && !m_bAirborne && !m_bSleep)	//아무일도... 없엇따!
			AiState = &CEnemy_Swordman::State_Idle;
	}
	else if (m_bDead)		//으앙 쥬금
	//State Dead	
		AiState = &CEnemy_Swordman::State_Dead;

	//스테이트 설정 완료 시 Func Pointer Start
	(this->*AiState)();
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
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 20);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	ENGINE::UNITINFO tInfo =
	{ FALSE, _vec3(0.f, 0.f, -100.f), _vec3{ 0.006f, 1.f, 1.f }, _vec3(90.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 50.f };
	pComponent = m_pWeapon = ENGINE::CWeapon::Create(m_pGraphicDev, m_pTransform, tInfo, L"Mesh_Enemy_Sword");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Weapon", pComponent);

	//Shadow Component
	pComponent = m_pShadow = ENGINE::CShadow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shadow", pComponent);

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

VOID CEnemy_Swordman::State_Awaken()
{
	if (m_fDist < 10.f && m_iCurAniSet == _IDLE) //대기 상태에서 플레이어 발견
	{
		Animate_FSM(_AWAKEN);	//사기 진작
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 20.f);

	}
	else if (m_iCurAniSet == _AWAKEN && m_pMesh->Is_AnimationSetEnd())	//사기 진작 종료, 행동 시작
	{
		Animate_FSM(_IDLE);
		m_bSleep = FALSE;
	}
}

VOID CEnemy_Swordman::State_Hit()
{
	m_HitTime += m_TimeDelta;
	Animate_FSM(_HIT);
	m_AccTime = 3.0;

	if (m_HitTime > 0.5f && m_pMesh->Is_AnimationSetEnd())	//1초 지나면 경직해제
	{
		m_pSphereColl->m_bHit = FALSE;
		m_HitTime = 0.0;
		m_AccTime = 1.0;

		Animate_FSM(_IDLE);
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CEnemy_Swordman::State_KnockBack()
{
	Animate_FSM(m_iKnockIdx[m_iKnockCnt]);	//첫번째 애니 재생

	if (m_iKnockCnt == 4 && !m_pMesh->Is_AnimationSetEnd())	//경직 후 기상 모션
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	else if (m_iKnockCnt == 4 && m_pMesh->Is_AnimationSetEnd())	//경직 종료
	{
		m_pSphereColl->m_bHit = FALSE;
		m_pSphereColl->m_bKnockBack = FALSE;
		m_pSphereColl->m_bAirbone = FALSE;

		m_iKnockCnt = 0;
		m_AccTime = 1.0;
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}
	else if (m_iKnockCnt >= 0 && m_iKnockCnt <= 2 && !m_pMesh->Is_AnimationSetEnd())	//경직 중 날아감
	{
		_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		(&vPos, &(m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f));
	}
	else if (m_iCurAniSet == m_iKnockIdx[m_iKnockCnt] && m_pMesh->Is_AnimationSetEnd())	//경직 카운트 증가
	{
		m_AccTime = 2.0;
		++m_iKnockCnt;
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	}

}

VOID CEnemy_Swordman::State_Airborne()
{
	if (m_iCurAniSet == _RISEUP && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_IDLE);
		m_pSphereColl->m_bAirbone = FALSE;
		m_pSphereColl->m_bHit = FALSE;
		m_pSphereColl->m_bKnockBack = FALSE;
		m_AirTime = 0.0;
	}
	else if (m_iCurAniSet != _RISEUP)
	{
		m_AirTime += m_TimeDelta;
		Animate_FSM(_AIRBORNE);
		m_AccTime = 2.0;
	}

	if (m_AirTime > 1.5f && m_iCurAniSet == _AIRBORNE && m_pMesh->Is_AnimationSetEnd())	//1초 지나면 경직해제
	{
		m_AccTime = 1.0;
		Animate_FSM(_RISEUP);
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CEnemy_Swordman::State_Idle()
{
	Animate_FSM(_IDLE);
	m_bAttack = FALSE;
	m_bTwice = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

}

VOID CEnemy_Swordman::State_Chase()
{

	//플레이어가 거리 내에 있으면 무적권 시야 고정
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_fDist > 3.f) //플레이어가 멀리 있음, 달려가 추적
	{
		Animate_FSM(_CHASE_RUN);

		Check_EnemyGroup();

		_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));

		m_AttackTime = 0.0;
		m_fWaitTime = ENGINE::CWell512::Get_Instance()->Get_floatValues(1.f, 3.f);
	}
	else if (m_fDist < 3.f)
	{
		Animate_FSM(_CHASE_WALK);

		Check_EnemyGroup();

		if (m_fDist > 1.75f)
		{
			_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
			m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
			(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 0.2f));
			
			_vec3 vRight = m_pTransform->m_vInfo[ENGINE::INFO_RIGHT];
			D3DXVec3Normalize(&vRight, &vRight);
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRight * m_TimeDelta * 0.5f;
		}
		else if (m_fDist < 1.75f)	//플레이어와 거리 유지
		{
			m_AttackTime += m_TimeDelta;	//공격 시도전 대기 시간

			if (m_AttackTime > m_fWaitTime)	// 플레이어와 일정거리를 유지한체 시간을 채움
			{
				Animate_FSM(_IDLE);
				m_bAttack = TRUE;	//공격 시작
				m_pTransform->m_bAttackState = m_bAttack;
				m_AttackTime = 0.0;
			}
		}

	}

}

VOID CEnemy_Swordman::State_Attack()
{

	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_fDist < 1.25f && m_iCurAniSet == _CHASE_WALK || m_iCurAniSet == _IDLE) // 거리가 됐네 공격
	{
		Animate_FSM(_ATTACK_START);
	}
	else if (m_iCurAniSet == _ATTACK_START && !m_pMesh->Is_AnimationSetEnd())
	{
		m_AttackTime += m_TimeDelta;

		if (m_pWeapon->Check_ComponentColl(m_pTargetSphereColl) && m_AttackTime > 1.25)
		{
			m_pWeapon->Set_AttackState(m_bAttack, m_iCurAniSet, 2);

			if (m_pTargetSphereColl->m_bHit && !m_bTwice)
			{
				m_pTargetSphereColl->m_bKnockBack = TRUE;
				_vec3 vKnockDir = m_pTargetSphereColl->Get_CollPos() - m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
				vKnockDir.y = 0.f;
				D3DXVec3Normalize(&vKnockDir, &vKnockDir);
				m_pTargetSphereColl->Set_KnockBackDist(TRUE, vKnockDir);
				m_bTwice = TRUE;
			}
			if (!m_bTwice)
			{
				m_pTargetSphereColl->m_bHit = TRUE;
				m_bTwice = TRUE;
			}

			if (!m_pTargetSphereColl->m_bInvisible)
				m_pTargetSphereColl->Get_iHp(2);

		}

	}
	else if (m_iCurAniSet == _ATTACK_START && m_pMesh->Is_AnimationSetEnd())
	{
		m_pTransform->m_bAttackState = FALSE;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

		Animate_FSM(_ATTACK_END);
		m_AttackTime = 0.0;
	}
	else if (m_iCurAniSet == _ATTACK_END && m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttack = FALSE;
		m_bTwice = FALSE;

		m_AttackTime = 0.0;
		Animate_FSM(_IDLE);
		return;
	}

	if (m_fDist > 1.25f && !m_bTwice)	//공격 준비됐는데 거리가 좀 머네
	{
		m_pTransform->m_bAttackState = FALSE;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
		m_AttackTime = 0.0;
		Animate_FSM(_CHASE_WALK);

		_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));

		//Check_EnemyGroup();
	}


}

VOID CEnemy_Swordman::State_Dead()
{
	m_AttackTime += m_TimeDelta;
	m_bAttack = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	Animate_FSM(_DEAD);

	if (m_AttackTime > 1.0 && m_AttackTime < 5.6)
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->Get_vLookDir() * m_TimeDelta * 0.25;

}
