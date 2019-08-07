#include "stdafx.h"
#include "Boss_Blue.h"

#include "Export_Function.h"

#define _SPEED 1.4f
#define _ANGLE 60.f
#define  _RADIUS 300.f
#define  _SCALE 0.035f

//STATE DEFINE
#define _IDLE								21
#define _CHASE_RUN					2
#define _CHASE_WALK				0

#define _ATTACK_MID_START				8
#define	_ATTACK_MID_END					7
#define _ATTACK_UP_START				4
#define	_ATTACK_UP_END						3
#define _ATTACK_DOWN_START			6
#define	_ATTACK_DOWN_END				5

#define _HIT									9
#define _BIGHIT							20

#define _AIRBORNE						18
#define _KnockBack_A				0
#define _KnockBack_B				0
#define _KnockBack_C				0
#define _KnockBack_D				0
#define  _DEAD							12
#define _RISEUP							19	//기상
#define _BLOCK							19

//Unique State
#define  _SLEEP							14
#define _AWAKEN						10
#define _GROWL							16

//Quake State
#define	_QUAKESTART				11
#define _QUAKEEND					10
#define _QUAKETOIDLE				21


CBlue::CBlue(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr), m_pTower(nullptr)
{
	m_bQuake = FALSE;
	m_bAttack = FALSE;
	m_bSleep = TRUE;
	m_bDead = FALSE;
	m_bHit = FALSE;
	m_bKnockBack = FALSE;
	m_bAirborne = FALSE;
	m_bTwice = FALSE;
	m_bRelicTwice = FALSE;
	m_bWeak = FALSE;
	m_bFire = FALSE;

	m_iCurAniSet = 0;
	m_iPreAniSet = 0;

	m_fScale = 1.f;

	m_HitTime = 0.0;
	m_AttackTime = 0.0;
	m_AirTime = 0.0;
	m_WeakTime = 0.0;
	m_TimeDelta = 0.0;
	m_AccTime = 1.5;
	m_FuryTime = 1.0;
	m_fReturnDist = 0.f;
}

CBlue::~CBlue()
{

}

HRESULT CBlue::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_Twice(_SLEEP);
	Set_Animation();
	m_pTransform->Set_StartPos(vPos);
	m_pTransform->m_vAngle.y = 0.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { _SCALE, _SCALE, _SCALE };
	m_pSphereHead->Set_Scale(_SCALE);
	m_pSphereBody->Set_Scale(_SCALE);
	m_pCollider_Right->Set_Scale(_SCALE);

	return S_OK;
}

HRESULT CBlue::Late_Init()
{

	m_pTargetTransform = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player_Dragon", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransform, E_FAIL);

	m_pTargetSphereColl = dynamic_cast<ENGINE::CSphereColl*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player_Dragon", L"Com_SphereColl", ENGINE::COMP_STATIC));
	NULL_CHECK_RETURN(m_pTargetSphereColl, E_FAIL);

	m_pRelicSphere = dynamic_cast<ENGINE::CSphereColl*>
		(ENGINE::Get_Component(ENGINE::CLayer::ENVIRONMENT, L"Mesh_Relic", L"Com_SphereColl", ENGINE::COMP_STATIC));
	NULL_CHECK_RETURN(m_pRelicSphere, E_FAIL);

	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::ENVIRONMENT);

	if (pLayer->Get_MapObject(L"Mesh_Structure").empty())
		return FALSE;

	for (auto& pList : pLayer->Get_MapObject(L"Mesh_Structure"))
	{
		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (m_pTransform->Get_TargetDistance(pTrans) < 24.f && m_pTower == nullptr)
		{
			m_pTower = dynamic_cast<CStatic_Tower*>(pList);
		}

	}
	
	return S_OK;
}

_int CBlue::Update_Object(const _double& TimeDelta)
{
	m_TimeDelta = TimeDelta * m_FuryTime;

	ENGINE::CGameObject::Late_Init();

	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_pSphereBody->Get_iHp() < 0)
	{
		m_bWeak = TRUE;
		m_pTransform->m_bWeak = TRUE;
	}

	if (m_pSphereHead->Get_iHp() <= 0)
	{
		m_bDead = TRUE;
		m_pTransform->Get_Dead(TRUE);

		if (m_pTransform->m_vScale.x < _SCALE)
		{
			m_pTransform->m_vScale += _vec3(0.0004f, 0.0004f, 0.0004f);
			m_fScale += 0.002f;
		}
	}

	if (!m_pTransform->m_bWeak)
	{
		m_FuryTime = 1.5;
		m_pSphereHead->Set_Invisible(TRUE);
	}
	else
	{
		m_FuryTime = 3.0;
		//m_pSphereHead->Set_Invisible(FALSE);
	}

	if (m_bDead && m_iCurAniSet == _DEAD && m_pMesh->Is_AnimationSetEnd())
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CBlue::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	m_fDist = m_pTransform->Get_BossDistance(m_pTargetTransform, m_pSphereBody->Get_CollPos().y);
}

void CBlue::Render_Object()
{
	//Total Enemy Behavior
	Set_Behavior_Progress();

	m_pMesh->Play_AnimationSet(m_TimeDelta * m_AccTime);
	m_pMesh2->Play_AnimationSet(m_TimeDelta * m_AccTime);
	/////////////////////////////////////////////
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	if (m_bWeak)
	{
		if (m_pTransform->m_vScale.x > (_SCALE * 0.4f))
			m_pTransform->m_vScale -= _vec3(0.0002f, 0.0002f, 0.0002f);
	}
	else if (!m_bWeak)
	{
		if (m_pTransform->m_vScale.x < (_SCALE * m_fScale))
			m_pTransform->m_vScale += _vec3(0.0003f, 0.0003f, 0.0003f);
	}

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	////////////////////////////////////////

	//m_pNaviMesh->Render_NaviMesh();

	if (m_bWeak)
		m_pMesh2->Render_Meshes();
	else
		m_pMesh->Render_Meshes();



	////////////////////////////////////////
	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	///////////////////////////////////////
	Get_WeaponMatrix("BODY1");
	m_pShadow->Render_Shadow(&m_pBoneMatrix_Right, (30.f * 0.65f * m_fScale), 1.f);

	//if (m_bAttack)
	Get_WeaponMatrix("R_Hand");
	m_pCollider_Right->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix_Right);

	Get_WeaponMatrix2("L_Hand");
	m_pCollider_Left->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix_Left);


	//if (m_pSphereHead->m_iHp > 0 && m_pTransform->m_bWeak);
	//{
	//	Get_WeaponMatrix3("HEAD");
	//	m_pSphereHead->Render_SphereColl(&m_pBoneMatrix_Right);
	//}

	if (m_pSphereBody->m_iHp != 0)
	{
		Get_WeaponMatrix3("BODY1");
		m_pSphereBody->Render_SphereColl(&m_pBody);
		m_pSphereBody->Set_WeakPos(&m_pBody);
	}
	
	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"fSize : %4.2f", m_fScale);
	//////swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CBlue::Set_Animation()
{
	//m_iKnockCnt = 0;
	//m_iKnockIdx[0] = _KnockBack_A;
	//m_iKnockIdx[1] = _KnockBack_B;
	//m_iKnockIdx[2] = _KnockBack_C;
	//m_iKnockIdx[3] = _KnockBack_D;
	//m_iKnockIdx[4] = _RISEUP;

	m_iQuakeCnt = 0;
	m_iQuakeIdx[0] = _QUAKESTART;
	m_iQuakeIdx[1] = _QUAKEEND;
	m_iQuakeIdx[2] = _QUAKETOIDLE;

}

void CBlue::Get_WeaponMatrix(const char * tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pBoneMatrix_Right = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CBlue::Get_WeaponMatrix2(const char * tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pBoneMatrix_Left = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CBlue::Get_WeaponMatrix3(const char * tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pBody = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CBlue::Check_EnemyGroup()
{
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	_vec3 vRevDir = { 0.f, 0.f, 0.f };	//주변에 걸리적 거리는 놈 있으면 밀어내는 거리

	if (Check_EnemyColl(&vRevDir, L"Troll"))	//객체 별 충돌 체크
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

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

_bool CBlue::Check_EnemyColl(_vec3 * vRevDir, const _tchar * szTag)
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

		if (m_pSphereBody->Check_ComponentColl(pSphere))
		{
			_vec3 vTargetRevDir = pTrans->Get_TargetReverseDir(m_pTransform);
			vTargetRevDir.y = 0.f;

			if (pTrans->m_bAttackState == FALSE)
				pTrans->m_vInfo[ENGINE::INFO_POS] += vTargetRevDir * m_TimeDelta;
			return TRUE;
		}

		//Get_WeaponMatrix("R_Hand");
		//m_pCollider->Check_ComponentColl()

	}

	return FALSE;
}

_bool CBlue::Check_EnemySphereColl(const _tchar * szTag)
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
			(pList->Get_Component(L"Com_SphereBody", ENGINE::COMP_STATIC));

		if (pSphere == nullptr)
			return FALSE;

		if (m_pCollider_Left->Check_ComponentBossColl(pSphere) && !pTrans->m_bWeak)
		{
			pSphere->m_bHit = TRUE;
			pSphere->Get_iHp(10);

			return TRUE;
		}

		if (m_pCollider_Right->Check_ComponentBossColl(pSphere) && !pTrans->m_bWeak)
		{
			pSphere->m_bHit = TRUE;
			pSphere->Get_iHp(10);

			return TRUE;
		}

	}

	return FALSE;
}

VOID CBlue::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}

VOID CBlue::Animate_Twice(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_pMesh2->Set_AnimationSet(m_iCurAniSet);
		m_pTransform->Set_AniState(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}

VOID CBlue::Set_Behavior_Progress()
{
	/// 추후 Bool 값으로 제어되는 상태를 하나의 단일 FLAG 연산으로 개선하여 단일 변수로 수정 _uint m_StateFlag , Swith로 제어
	//Check State

	m_bHit = m_pSphereBody->Get_HitState();

	//m_bKnockBack = m_pSphereColl->Get_KnockBackState();
	//m_bAirborne = m_pSphereBody->Get_AirboneState();

	if (!m_bDead)	//안죽었을 때
	{
		//State Awaken
		if (m_bSleep)	//생성 후 기본 대기 루프
			AiState = &CBlue::State_Awaken;
		//State Weak
		else if (m_bWeak)	//공격 명중 시, 경직
			AiState = &CBlue::State_Weak;
		//State Hit
		else if (m_bHit && !m_bKnockBack && !m_bAirborne)	//공격 명중 시, 경직
			AiState = &CBlue::State_Hit;
		////State KnockBack
		//else if (m_bKnockBack)		//넉백 공격 받음
		//	AiState = &CTroll::State_KnockBack;
		//State Airbone -> Troll to Hit
		//else if (m_bAirborne && !m_bKnockBack)	//에어본 공격받음
		//	AiState = &CBlue::State_Airborne;
		//State Chase
		else if (!m_bAttack && !m_bHit && m_fDist < 200.f)	//플레이어 추적 시작
			AiState = &CBlue::State_Chase;
		//State Attack
		else if (m_bQuake && m_bAttack && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CBlue::State_Quake;
		else if (!m_bQuake && m_bAttack && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CBlue::State_Attack;
		//State Idle
		//if (m_fDist > 100.f && !m_bHit && !m_bKnockBack && !m_bAirborne && !m_bSleep)	//아무일도... 없엇따!
		//	AiState = &CBlue::State_Idle;
	}
	else if (m_bDead)		//으앙 쥬금
							//State Dead	
		AiState = &CBlue::State_Dead;

	//스테이트 설정 완료 시 Func Pointer Start
	(this->*AiState)();
}

VOID CBlue::State_Awaken()
{
	if (m_fDist < 200.f && m_iCurAniSet == _SLEEP) //대기 상태에서 플레이어 발견
	{
		Animate_Twice(_AWAKEN);
	}
	else if (m_iCurAniSet == _AWAKEN && m_pMesh->Is_AnimationSetEnd())	//사기 진작 종료, 행동 시작
	{
		Animate_Twice(_GROWL);
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 200.f);
	}
	else if (m_iCurAniSet == _GROWL && m_pMesh->Is_AnimationSetEnd())	//사기 진작 종료, 행동 시작
	{
		Animate_Twice(_IDLE);
		m_bSleep = FALSE;
	}
}

VOID CBlue::State_Weak()
{
	m_WeakTime += m_TimeDelta;

	if (!m_bFire)
	{
		m_pTower->Set_Fire(TRUE);
		m_bFire = TRUE;
	}

	_float fdist = D3DXVec3Length(&(m_pTransform->m_vStartPos - m_pTransform->m_vInfo[ENGINE::INFO_POS]));

	if (fdist > 2.f)
	{
		m_pTransform->Fall_BackBoss(2.f, m_TimeDelta);
		Animate_Twice(_CHASE_RUN);
	}
	else if (m_iCurAniSet == _GROWL && m_pMesh->Is_AnimationSetEnd())
	{
		m_WeakTime = 0.0;
		m_bWeak = FALSE;
		m_bFire = FALSE;
		
		m_pTransform->m_bWeak = FALSE;

		if (m_pSphereHead->Get_iHp() != 0)
			m_pSphereBody->m_iHp = 6;

		m_pSphereHead->Set_Scale((_SCALE * m_fScale));
		m_pSphereBody->Set_Scale((_SCALE * m_fScale));
		m_pCollider_Right->Set_Scale((_SCALE * m_fScale));

		Animate_Twice(_IDLE);
	}
	else if (m_WeakTime > 15.0 || fdist < 2.f)
	{
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 200.f);
		if (m_pTransform->m_vScale.x < (_SCALE * m_fScale))
			m_pTransform->m_vScale += _vec3(0.0004f, 0.0004f, 0.0004f);
		Animate_Twice(_GROWL);
	}

}


VOID CBlue::State_Hit()
{
	m_HitTime += m_TimeDelta;
	Animate_Twice(_HIT);
	m_AccTime = 1.5;

	if (m_HitTime > 3.f && m_pMesh->Is_AnimationSetEnd())	//1초 지나면 경직해제
	{

		if (m_pSphereBody->Get_iHp() <= 0 && !m_bWeak)
		{
			m_bWeak = TRUE;
			m_pTransform->m_bWeak = TRUE;

			m_fScale -= 0.2f;

			if (m_fScale < 0.4f)
				m_fScale = 0.4f;
		}

		m_pSphereHead->m_bHit = FALSE;
		m_pSphereBody->m_bHit = FALSE;

		m_HitTime = 0.0;
		m_AccTime = 1.0;

		Animate_Twice(_IDLE);
		m_bAttack = FALSE;
		m_bQuake = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_iQuakeCnt = 0;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CBlue::State_KnockBack()
{
	Animate_Twice(m_iKnockIdx[m_iKnockCnt]);	//첫번째 애니 재생

	if (m_iKnockCnt == 4 && !m_pMesh->Is_AnimationSetEnd())	//경직 후 기상 모션
		Animate_Twice(m_iKnockIdx[m_iKnockCnt]);
	else if (m_iKnockCnt == 4 && m_pMesh->Is_AnimationSetEnd())	//경직 종료
	{
		m_pSphereBody->m_bHit = FALSE;
		m_pSphereBody->m_bKnockBack = FALSE;
		m_pSphereBody->m_bAirbone = FALSE;

		m_iKnockCnt = 0;
		m_AccTime = 1.0;
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}
	else if (m_iKnockCnt >= 0 && m_iKnockCnt <= 2 && !m_pMesh->Is_AnimationSetEnd())	//경직 중 날아감
	{
		_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		(&vPos, &(m_pSphereBody->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f));
	}
	else if (m_iCurAniSet == m_iKnockIdx[m_iKnockCnt] && m_pMesh->Is_AnimationSetEnd())	//경직 카운트 증가
	{
		m_AccTime = 2.0;
		++m_iKnockCnt;
		Animate_Twice(m_iKnockIdx[m_iKnockCnt]);
	}

}

VOID CBlue::State_Airborne()
{
	if (m_iCurAniSet == _RISEUP && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_Twice(_IDLE);
		m_pSphereBody->m_bAirbone = FALSE;
		m_pSphereBody->m_bHit = FALSE;
		m_pSphereBody->m_bKnockBack = FALSE;
		m_AirTime = 0.0;
	}
	else if (m_iCurAniSet != _RISEUP)
	{
		m_AirTime += m_TimeDelta;
		Animate_Twice(_AIRBORNE);
		m_AccTime = 2.0;

		if (m_AirTime < 0.75)
		{
			//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
			//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
			//(&vPos, &(m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f));
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pSphereBody->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f;
		}

		m_bAttack = FALSE;
		m_bTwice = FALSE;
		m_pTransform->m_bAttackState = FALSE;
	}

	if (m_AirTime > 1.5f && m_iCurAniSet == _AIRBORNE && m_pMesh->Is_AnimationSetEnd())	//1초 지나면 경직해제
	{
		m_AccTime = 1.0;
		Animate_Twice(_RISEUP);
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CBlue::State_Idle()
{

	Animate_Twice(_IDLE);
	m_bAttack = FALSE;
	m_bTwice = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

}

VOID CBlue::State_Chase()
{
	//플레이어가 거리 내에 있으면 무적권 시야 고정
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 200.f);

	if (m_fDist > (70.f * m_fScale)) //플레이어가 멀리 있음, 달려가 추적
	{
		if (m_iCurAniSet != _BLOCK)
		{
			Animate_Twice(_CHASE_RUN);
			m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 5.5f);
		}
		else if (m_iCurAniSet == _BLOCK && m_pMesh->Is_AnimationSetEnd())
			Animate_Twice(_IDLE);

		//Check_EnemyGroup();

		//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		//(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));


		m_AttackTime = 0.0;
	}
	else if (m_fDist < (70.f * m_fScale))
	{
		Animate_Twice(_BLOCK);
		m_AttackTime += m_TimeDelta;	//공격 시도전 대기 시간

		if (m_fDist < (40.f * m_fScale))
		{
			if (m_iCurAniSet == _BLOCK && m_pMesh->Is_AnimationSetEnd())	// 플레이어와 일정거리를 유지한체 시간을 채움
			{
				Animate_Twice(_IDLE);
				m_bAttack = TRUE;	//공격 시작
				m_bQuake = TRUE;
				m_pTransform->m_bAttackState = m_bAttack;
				m_AttackTime = 0.0;
				m_AccTime = 2.0;
			}

		}
		else  if (m_fDist > (40.f * m_fScale))	//플레이어와 거리 유지
		{

			if (m_iCurAniSet == _BLOCK && m_pMesh->Is_AnimationSetEnd())	// 플레이어와 일정거리를 유지한체 시간을 채움
			{
				Animate_Twice(_IDLE);
				m_bAttack = TRUE;	//공격 시작
				m_pTransform->m_bAttackState = m_bAttack;
				m_AttackTime = 0.0;
				m_AccTime = 2.0;
			}
		}

	}
}

VOID CBlue::State_Quake()
{
	Animate_Twice(m_iQuakeIdx[m_iQuakeCnt]);	//첫번째 애니 재생

	if (m_iCurAniSet == _QUAKEEND && !m_pMesh->Is_AnimationSetEnd())	//경직 후 기상 모션
		Animate_Twice(m_iQuakeIdx[m_iQuakeCnt]);
	else if (m_iCurAniSet == _QUAKEEND && m_pMesh->Is_AnimationSetEnd())	//경직 종료
	{
		m_pSphereBody->m_bHit = FALSE;
		m_pSphereBody->m_bKnockBack = FALSE;
		m_pSphereBody->m_bAirbone = FALSE;

		m_iQuakeCnt = 0;
		m_AccTime = 1.0;
		m_bQuake = FALSE;
		m_bAttack = FALSE;
		m_bTwice = FALSE;
		m_bRelicTwice = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}
	else if (m_iCurAniSet == _QUAKESTART && !m_pMesh->Is_AnimationSetEnd())	//경직 중 날아감
	{
		//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		//(&vPos, &(m_pSphereBody->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f));

		m_AttackTime += m_TimeDelta;
		_bool bColl = m_pCollider_Right->Check_ComponentColl(m_pTargetSphereColl);
		if(!bColl)
			bColl = m_pCollider_Left->Check_ComponentColl(m_pTargetSphereColl);

		_bool bRelicColl = m_pCollider_Right->Check_ComponentColl(m_pRelicSphere);
		if (!bRelicColl)
			bRelicColl = m_pCollider_Left->Check_ComponentColl(m_pRelicSphere);


		if (bRelicColl && m_AttackTime > 1.0)
		{
			if (!m_bRelicTwice)
			{
				m_bRelicTwice = TRUE;
				m_pRelicSphere->Get_iHp(10);
			}

		}

		if (bColl && m_AttackTime > 1.0)
		{
			m_pTransform->m_bAttackState = TRUE;

			if (!m_bTwice)
			{
				m_pTargetSphereColl->m_bHit = TRUE;
				m_pTargetSphereColl->m_bKnockBack = TRUE;

				_vec3 vKnockDir = m_pTargetSphereColl->Get_CollPos() - m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
				vKnockDir.y = 0.f;
				D3DXVec3Normalize(&vKnockDir, &vKnockDir);
				m_pTargetSphereColl->Set_KnockBackDist(TRUE, vKnockDir);

				m_bTwice = TRUE;
				m_pTargetSphereColl->Get_iHp(2);
			}

		}

	}
	else if (m_iCurAniSet == m_iQuakeIdx[m_iQuakeCnt] && m_pMesh->Is_AnimationSetEnd())	//경직 카운트 증가
	{
		m_AccTime = 2.0;
		++m_iQuakeCnt;
		Animate_Twice(m_iQuakeIdx[m_iQuakeCnt]);
	}

}

VOID CBlue::State_Attack()
{
	_bool bColl = FALSE;
	_bool bEnemyColl = FALSE;
	_bool bRelicColl = FALSE;

	if (!m_bAttack && m_fDist < (70.f * m_fScale) && m_iCurAniSet == _CHASE_WALK || m_iCurAniSet == _IDLE) // 거리가 됐네 공격
	{
		Get_WeaponMatrix("BODY1");
		_float fy = m_pBoneMatrix_Right._42;
		_vec3 vPlayerPos = m_pTargetTransform->Get_vInfoPos(ENGINE::INFO_POS);

		if (fy > vPlayerPos.y + 2.f)
			Animate_Twice(_ATTACK_DOWN_START);
		else if (fy < vPlayerPos.y + 2.f)
			Animate_Twice(_ATTACK_UP_START);
		else
			Animate_Twice(_ATTACK_MID_START);

		m_bAttStart = TRUE;
	}
	else if (m_bAttStart && !m_pMesh->Is_AnimationSetEnd())
	{
		m_AttackTime += m_TimeDelta;
		bColl = m_pCollider_Right->Check_ComponentColl(m_pTargetSphereColl);
		bRelicColl = m_pCollider_Right->Check_ComponentColl(m_pRelicSphere);
		bEnemyColl = Check_EnemySphereColl(L"Boss_Blue");

		if (bRelicColl && m_AttackTime > 0.5)
		{
			if (!m_bRelicTwice)
			{
				m_bRelicTwice = TRUE;
				m_pRelicSphere->Get_iHp(5);
			}

		}
		
		if (bColl && m_AttackTime > 0.5)
		{
			m_pTransform->m_bAttackState = TRUE;

			if (!m_bTwice)
			{
				m_pTargetSphereColl->m_bHit = TRUE;
				m_pTargetSphereColl->m_bKnockBack = TRUE;
				m_AccTime = 1.0;

				_vec3 vKnockDir = m_pTargetSphereColl->Get_CollPos() - m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
				vKnockDir.y = 0.f;
				D3DXVec3Normalize(&vKnockDir, &vKnockDir);
				m_pTargetSphereColl->Set_KnockBackDist(TRUE, vKnockDir);

				m_bTwice = TRUE;
				m_pTargetSphereColl->Get_iHp(2);


			}
		}

		
	}
	else if (m_bAttStart && m_pMesh->Is_AnimationSetEnd())
	{
		m_pTransform->m_bAttackState = FALSE;
		m_bAttStart = FALSE;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

		if (m_iCurAniSet == _ATTACK_MID_START)
			Animate_Twice(_ATTACK_MID_END);
		else if (m_iCurAniSet == _ATTACK_DOWN_START)
			Animate_Twice(_ATTACK_DOWN_END);
		else if (m_iCurAniSet == _ATTACK_UP_START)
			Animate_Twice(_ATTACK_UP_END);

		m_AttackTime = 0.0;
	}
	else if (!m_bAttStart && m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttack = FALSE;
		m_bTwice = FALSE;
		m_bRelicTwice = FALSE;

		m_AttackTime = 0.0;
		Animate_Twice(_IDLE);
		return;
	}

	if (!m_bAttack && m_fDist > (70.f * m_fScale) && !m_bTwice)	//공격 준비됐는데 거리가 좀 머네
	{
		m_pTransform->m_bAttackState = FALSE;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
		m_AttackTime = 0.0;
		Animate_Twice(_CHASE_WALK);

		//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		//(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 5.5f);

		Check_EnemyGroup();
	}
}

VOID CBlue::State_Dead()
{
	m_AttackTime += m_TimeDelta;
	m_bAttack = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	Animate_Twice(_DEAD);

	//if (m_AttackTime > 1.0 && m_AttackTime < 5.8)
	//	m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->Get_vLookDir() * m_TimeDelta * 0.25;

}

HRESULT CBlue::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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


HRESULT CBlue::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Boss_Blue"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMesh2 = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Boss_NudeBlue"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh2", pComponent);

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
	pComponent = m_pCollider_Left = ENGINE::CCollider::Create(m_pGraphicDev, 200.f, _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider_Left", pComponent);

	pComponent = m_pCollider_Right = ENGINE::CCollider::Create(m_pGraphicDev, 200.f, _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider_Right", pComponent);

	//Sphere Collider
	pComponent = m_pSphereHead = ENGINE::CSphereColl::Create(m_pGraphicDev, (_RADIUS - 50.f), 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereHead", pComponent);

	pComponent = m_pSphereBody = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 6);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereBody", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	//Shadow Component
	pComponent = m_pShadow = ENGINE::CShadow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shadow", pComponent);

	////////////////////////////
	return S_OK;
}

CBlue * CBlue::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBlue* pInstance = new CBlue(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CBlue::Free()
{
	ENGINE::CGameObject::Free();
}
