#include "stdafx.h"
#include "Boss_Keroberos.h"

#include "Export_Function.h"

#define _SPEED 2.4f
#define _ANGLE 60.f
#define  _RADIUS 300.f
#define  _SCALE 0.006f

//STATE DEFINE
#define _IDLE								0
//CHASE
#define _CHASE_RUN					2
#define _CHASE_WALK				2
//JUMP
#define _JUMP_FRONT				3
#define _JUMP_BACK					4
#define _JUMP_LEFT					7
#define _JUMP_RIGHT				8
//ATTACK_TAIL_SWEEP
#define _ATTACK_TAIL_SWEEP					9
//ATTACK_TAIL_COMBO
#define _ATTACK_TAIL_COMBO1				17
#define _ATTACK_TAIL_COMBO2				18
#define _ATTACK_TAIL_COMBO3				19
#define _ATTACK_TAIL_COMBO4				20
#define _ATTACK_BITE_START					21
#define _ATTACK_BITE_END						22
//ATTACK_BITE_COMBO
#define _ATTACK_BITE_COMBO1				23
#define _ATTACK_BITE_COMBO2				24
#define _ATTACK_BITE_COMBO3				25
#define _ATTACK_BITE_COMBO4				26
#define _ATTACK_BITE_COMBO5				27
#define _ATTACK_BITE_COMBO6				28
#define _ATTACK_BITE_COMBO7				29
//ATTACK_BREATH
#define _ATTACK_BREATH							10


#define _HIT									30
#define _BIGHIT							32

#define _AIRBORNE						18
#define _KnockBack_A				34
#define _KnockBack_B				35
#define _KnockBack_C				36
#define _KnockBack_D				37
#define _KnockBack_E				38
#define _RISEUP							39	//기상
#define _DEADING						46
#define  _DEAD							50

//Unique State
#define  _SLEEP							0
#define _AWAKEN						51
#define _GROWL							16



CKeroberos::CKeroberos(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	m_bAttack = FALSE;
	m_bTwice = FALSE;
	m_bFireBreath = FALSE;
	m_bBreathDelay = FALSE;

	m_bAttStart = FALSE;
	m_bAttEnd = FALSE;

	m_bSweep = FALSE;
	m_bTailCombo = FALSE;
	m_bBite = FALSE;
	m_bBiteCombo = FALSE;

	m_bDead = FALSE;
	m_bSleep = TRUE;
	m_bHit = FALSE;
	
	for (UINT i = 0; i < 3; ++i)
		m_bJump[i] = FALSE;

	m_bKnockBack = FALSE;

	m_iCurAniSet = 0;
	m_iPreAniSet = 0;
	m_iAttackAniset = 0;

	m_CheckState = 0;
	m_HitTime = 0.0;
	m_AttackTime = 0.0;
	m_TimeDelta = 0.0;
	m_fReturnDist = 0.f;
	m_BreathTime = 0.0;
	m_AccTime = 1.0;
	m_FuryTime = 1.0;
}

CKeroberos::~CKeroberos()
{

}

HRESULT CKeroberos::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_FSM(_SLEEP);
	Set_Animation();
	m_pTransform->Set_StartPos(vPos);
	m_pTransform->m_vAngle.y = 0.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { _SCALE, _SCALE, _SCALE };
	m_pSphereColl->Set_Scale(_SCALE);
	m_pCollider_Head->Set_Scale(_SCALE);
	m_pCollider_Tail1->Set_Scale(_SCALE);

	return S_OK;
}

HRESULT CKeroberos::Late_Init()
{

	m_pTargetTransform = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransform, E_FAIL);

	m_pTargetSphereColl = dynamic_cast<ENGINE::CSphereColl*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_SphereColl", ENGINE::COMP_STATIC));
	NULL_CHECK_RETURN(m_pTargetSphereColl, E_FAIL);

	return S_OK;
}

_int CKeroberos::Update_Object(const _double& TimeDelta)
{
	m_TimeDelta = TimeDelta * m_FuryTime;

	ENGINE::CGameObject::Late_Init();

	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_bDead && m_iCurAniSet == _DEAD && m_pMesh->Is_AnimationSetEnd())
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CKeroberos::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	//m_fDist = m_pTransform->Get_BossDistance(m_pTargetTransform, m_pSphereColl->Get_CollPos().y);
	m_fDist = m_pTransform->Get_TargetDistance(m_pTargetTransform);
}

void CKeroberos::Render_Object()
{
	//Total Enemy Behavior
	Set_Behavior_Progress();

	m_pMesh->Play_AnimationSet(m_TimeDelta * m_AccTime);
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

	//m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();



	////////////////////////////////////////
	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	///////////////////////////////////////

	if (!m_bDead)
		m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld, 1.f);

	Get_HeadMatrix("C_HEAD", &m_pHeadMatrix);
	m_pCollider_Head->Render_Collider(ENGINE::COL_TRUE, &m_pHeadMatrix);

	Get_HeadMatrix("TAIL3", &m_pTailMatrix1);
	m_pCollider_Tail1->Render_Collider(ENGINE::COL_TRUE, &m_pTailMatrix1);
	Get_HeadMatrix("TAILl5", &m_pTailMatrix2);
	m_pCollider_Tail2->Render_Collider(ENGINE::COL_TRUE, &m_pTailMatrix2);


	_tchar szStr[MAX_PATH] = L"";
	swprintf_s(szStr, L"Distance : %4.2f", m_fDist);
	//swprintf_s(szStr, L"Angle : %4.2f", m_fAngle);
	////swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));



	//Render_ReSet();
}

void CKeroberos::Set_Animation()
{
	m_iKnockCnt = 0;
	m_iKnockIdx[0] = _KnockBack_A;
	m_iKnockIdx[1] = _KnockBack_B;
	m_iKnockIdx[2] = _KnockBack_C;
	m_iKnockIdx[3] = _KnockBack_D;
	m_iKnockIdx[4] = _KnockBack_E;
	m_iKnockIdx[5] = _RISEUP;

	m_iTailCnt = 0;
	m_iTailIdx[0] = _ATTACK_TAIL_COMBO1;
	m_iTailIdx[1] = _ATTACK_TAIL_COMBO2;
	m_iTailIdx[2] = _ATTACK_TAIL_COMBO3;
	m_iTailIdx[3] = _ATTACK_TAIL_COMBO4;

	m_iBiteCnt = 0;
	m_iBiteIdx[0] = _ATTACK_BITE_COMBO1;
	m_iBiteIdx[1] = _ATTACK_BITE_COMBO2;
	m_iBiteIdx[2] = _ATTACK_BITE_COMBO3;
	m_iBiteIdx[3] = _ATTACK_BITE_COMBO4;
	m_iBiteIdx[4] = _ATTACK_BITE_COMBO5;
	m_iBiteIdx[5] = _ATTACK_BITE_COMBO6;
	m_iBiteIdx[6] = _ATTACK_BITE_COMBO7;

}

_float CKeroberos::Get_PlayerAngleY()
{
	_vec3 vNewDir = m_pTargetTransform->m_vInfo[ENGINE::INFO_POS] - m_pTransform->m_vInfo[ENGINE::INFO_POS];
	_float fDistance = D3DXVec3Length(&vNewDir);
	_vec3 vLook = m_pTransform->Get_vLookDir();

	D3DXVec3Normalize(&vNewDir, &vNewDir);
	D3DXVec3Normalize(&vLook, &vLook);
	_float fRad = D3DXVec3Dot(&vLook, &vNewDir);

	_float Angley = D3DXToDegree(acosf(fRad));

	if (m_pTargetTransform->m_vInfo[ENGINE::INFO_POS].x > m_pTransform->m_vInfo[ENGINE::INFO_POS].x)
		Angley *= -1.f;

	return m_fAngle = Angley;

}

void CKeroberos::Get_WeaponMatrix(const char * tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pHeadMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CKeroberos::Get_HeadMatrix(const char * tBone, _matrix* pMatrix)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	*pMatrix = pFrame->combinedTransformMatrix* m_pTransform->m_matWorld;
}

void CKeroberos::Check_EnemyGroup()
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

_bool CKeroberos::Check_EnemyColl(_vec3 * vRevDir, const _tchar * szTag)
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

		if (m_pSphereColl->Check_ComponentColl(pSphere))
		{
			_vec3 vTargetRevDir = pTrans->Get_TargetReverseDir(m_pTransform);
			vTargetRevDir.y = 0.f;

			if (pTrans->m_bAttackState == FALSE)
				pTrans->m_vInfo[ENGINE::INFO_POS] += vTargetRevDir * m_TimeDelta;
			return TRUE;
		}

	}

	return FALSE;
}

VOID CKeroberos::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_pTransform->Set_AniState(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}


VOID CKeroberos::Set_Behavior_Progress()
{
	/// 추후 Bool 값으로 제어되는 상태를 하나의 단일 FLAG 연산으로 개선하여 단일 변수로 수정 _uint m_StateFlag , Swith로 제어
	//Check State
	m_bHit = m_pSphereColl->Get_AirboneState();
	m_bKnockBack = m_pSphereColl->Get_KnockBackState();

	if (!m_bDead)	//안죽었을 때
	{
		//State Idle
		if (m_fDist > 10.f && !m_bHit && !m_bKnockBack && !m_bSleep && !m_bJump)	//아무일도... 없엇따!
			AiState = &CKeroberos::State_Idle;

		//State Awaken
		if (m_bSleep)	//생성 후 기본 대기 루프
			AiState = &CKeroberos::State_Awaken;
		//State Hit(Airborne)
		else if (m_bHit && !m_bKnockBack)	//공격 명중 시, 경직
			AiState = &CKeroberos::State_Hit;
		//State KnockBack
		else if (m_bKnockBack) 	//넉백 공격 받음
			AiState = &CKeroberos::State_KnockBack;
		//State Back_Jump
		else if (m_bJump[0])	//플레이어 반대로 백점프
			AiState = &CKeroberos::State_Chase_Jump;
		//State Chase
		else if (!m_bAttack && !m_bHit && m_fDist < 12.f)	//플레이어 추적 시작
			AiState = &CKeroberos::State_Chase;
		//State Attack_Sweep
		else if (m_bAttack && m_bBite && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CKeroberos::State_Attack_Bite;
		//State Attack_Sweep
		else if (m_bAttack && m_bSweep && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CKeroberos::State_Attack_Sweep;
		//State Attack_Breath
		else if (m_bAttack && m_bFireBreath && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CKeroberos::State_Attack_Firebreath;
		//State Attack_TailCombo
		else if (m_bAttack&& m_bTailCombo && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CKeroberos::State_Attack_TailCombo;
		//State Attack_BiteCombo
		else if (m_bAttack && m_bBiteCombo && !m_bHit)		//플레이어 공격 쌉가능
			AiState = &CKeroberos::State_Attack_BiteCombo;

	}
	else if (m_bDead)		//으앙 쥬금
							//State Dead	
		AiState = &CKeroberos::State_Dead;

	//스테이트 설정 완료 시 Func Pointer Start
	(this->*AiState)();
}

VOID CKeroberos::State_Awaken()
{
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_fDist < 10.f && m_iCurAniSet == _SLEEP) //대기 상태에서 플레이어 발견
		Animate_FSM(_AWAKEN);
	else if (m_iCurAniSet == _AWAKEN && m_pMesh->Is_AnimationSetEnd())	//사기 진작 종료, 행동 시작
	{
		Animate_FSM(_IDLE);
		m_bSleep = FALSE;
	}

}

VOID CKeroberos::State_Hit()
{
	m_HitTime += m_TimeDelta;
	Animate_FSM(_HIT);

	if (m_HitTime > 1.5f && m_iCurAniSet == _HIT && m_pMesh->Is_AnimationSetEnd())	//1초 지나면 경직해제
	{
		m_pSphereColl->m_bHit = FALSE;
		m_pSphereColl->m_bAirbone = FALSE;

		m_HitTime = 0.0;

		Animate_FSM(_IDLE);
		m_bAttack = FALSE;
		m_bAttStart = FALSE;
		m_bAttEnd = FALSE;
		m_bTwice = FALSE;

		m_bBite = FALSE;
		m_bSweep = FALSE;
		m_bBiteCombo = FALSE;
		m_bTailCombo = FALSE;
		m_bFireBreath = FALSE;
		m_pCollider_Tail2->Set_Scale(_SCALE);
		m_pTransform->m_bAttackState = m_bAttack;
	}

}

VOID CKeroberos::State_KnockBack()
{
	Animate_FSM(m_iKnockIdx[m_iKnockCnt]);	//첫번째 애니 재생

	if (m_iKnockCnt == 5 && !m_pMesh->Is_AnimationSetEnd())	//경직 후 기상 모션
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	else if (m_iKnockCnt == 5 && m_pMesh->Is_AnimationSetEnd())	//경직 종료
	{
		Animate_FSM(_IDLE);
		m_pSphereColl->m_bHit = FALSE;
		m_pSphereColl->m_bAirbone = FALSE;
		m_pSphereColl->m_bKnockBack = FALSE;

		m_iKnockCnt = 0;

		m_bAttack = FALSE;
		m_bAttStart = FALSE;
		m_bAttEnd = FALSE;
		m_bTwice = FALSE;

		m_bSweep = FALSE;
		m_bBiteCombo = FALSE;
		m_bTailCombo = FALSE;
		m_bFireBreath = FALSE;
		m_pCollider_Tail2->Set_Scale(_SCALE);

		m_pTransform->m_bAttackState = m_bAttack;

	}
	else if (m_iKnockCnt >= 0 && m_iKnockCnt <= 3 && !m_pMesh->Is_AnimationSetEnd())	//경직 중 날아감
	{
		_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		(&vPos, &(m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 1.5f));
	}
	else if (m_iCurAniSet == m_iKnockIdx[m_iKnockCnt] && m_pMesh->Is_AnimationSetEnd())	//경직 카운트 증가
	{
		++m_iKnockCnt;
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	}

}

VOID CKeroberos::State_Idle()
{
	Animate_FSM(_IDLE);
	m_bAttack = FALSE;
	m_bAttStart = FALSE;
	m_bAttEnd = FALSE;
	m_bTwice = FALSE;

	m_bSweep = FALSE;
	m_bBiteCombo = FALSE;
	m_bTailCombo = FALSE;
	m_bFireBreath = FALSE;

}

VOID CKeroberos::State_Chase()
{
	//플레이어가 근접해서 존나게 떄리고 있음 회피 동작
	if (m_fDist < 2.f && Get_PlayerAngleY() > 150.f)
	{
		m_eJumpState = JUMPBACK;
		m_bJump[0] = TRUE;
		m_CheckState = 0;
		return;
	}
	else if (m_fDist < 2.f && Get_PlayerAngleY() < -150.f)
	{
		m_eJumpState = JUMPBACK;
		m_bJump[0] = TRUE;
		m_CheckState = 0;
		return;
	}
	else if (m_fDist < 2.f && Get_PlayerAngleY() > 30.f)	//오른쪽 점프
	{
		m_eJumpState = JUMPRIGHT;
		m_bJump[0] = TRUE;
		m_CheckState = 0;
		return;
	}
	else if (m_fDist < 2.f && Get_PlayerAngleY() < -30.f)	//왼쪽 점프
	{
		m_eJumpState = JUMPLEFT;
		m_bJump[0] = TRUE;
		m_CheckState = 0;
		return;
	}
	else if (m_fDist < 2.f && Get_PlayerAngleY() < 30.f)
	{
		m_eJumpState = JUNPFORNT;
		m_bJump[0] = TRUE;
		m_CheckState = 0;
		return;
	}
	else if (m_fDist < 2.f && Get_PlayerAngleY() > -30.f)
	{
		m_eJumpState = JUNPFORNT;
		m_bJump[0] = TRUE;
		m_CheckState = 0;
		return;
	}

	//플레이어가 거리 내에 있으면 무적권 시야 고정
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);
	m_CheckState += m_TimeDelta;

	ENGINE::CWell512* Rand = ENGINE::CWell512::Get_Instance();
	_uint iRand = Rand->Get_Value(0, 5);
	
	while(iOldRnd == iRand)
		iRand = Rand->Get_Value(0, 5);

	iOldRnd = iRand;

	if (m_CheckState > 3.0)
	{
		switch (iRand)
		{
		case 0:
			m_bBiteCombo = TRUE;
			m_bAttack = TRUE;
			m_CheckState = 0;
			break;
		case 1:
			m_bBite = TRUE;
			m_bAttack = TRUE;
			m_CheckState = 0;
			break;
		case 2:
			Animate_FSM(_ATTACK_BREATH);
			m_bFireBreath = TRUE;
			m_bAttack = TRUE;
			m_CheckState = 0;
			break;
		case 3:
			Animate_FSM(_ATTACK_TAIL_SWEEP);
			m_bSweep = TRUE;
			m_bAttack = TRUE;
			m_CheckState = 0;
			break;
		case 4:
			m_bTailCombo = TRUE;
			m_bAttack = TRUE;
			m_CheckState = 0;
			break;
		}
	}
		
	/*if (m_fDist < 4.5f && m_CheckState > 3.f && m_iAttackAniset == _ATTACK_BITE_START)
	{
		m_bBiteCombo = TRUE;
		m_bAttack = TRUE;
		m_CheckState = 0;
		m_iAttackAniset = _ATTACK_BITE_COMBO1;
		return;
	}
	else if (m_fDist < 4.5f && m_CheckState > 3.f)
	{
		m_bBite = TRUE;
		m_bAttack = TRUE;
		m_CheckState = 0;
		m_iAttackAniset = _ATTACK_BITE_START;
		return;
	}*/
	
	if (m_fDist > 4.5f && m_CheckState > 1.f)
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 3.5f);
}

VOID CKeroberos::State_Chase_Jump()
{
	//플레이어가 거리 내에 있으면 무적권 시야 고정
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	_vec3 vJumpDir = {};

	switch (m_eJumpState)
	{
	case CKeroberos::JUNPFORNT:
		vJumpDir = m_pTransform->Get_vLookDir();
		Animate_FSM(_JUMP_FRONT);
		break;
	case CKeroberos::JUMPBACK:
		vJumpDir = m_pTransform->Get_TargetReverseDir(m_pTargetTransform);
		Animate_FSM(_JUMP_BACK);
		break;
	case CKeroberos::JUMPLEFT:
		vJumpDir = m_pTransform->Get_TargetReverseDir(m_pTargetTransform);
		Animate_FSM(_JUMP_LEFT);
		break;
	case CKeroberos::JUMPRIGHT:
		vJumpDir = m_pTransform->Get_TargetReverseDir(m_pTargetTransform);
		Animate_FSM(_JUMP_RIGHT);
		break;
	}

	m_bJump[1] = TRUE;
	vJumpDir.y = 0.f;

	if (m_bJump[1] && !m_bJump[2] && !m_pMesh->Is_AnimationSetEnd())
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += vJumpDir * 2.25f * m_TimeDelta;
	else if (m_bJump[1] && !m_bJump[2] && m_pMesh->Is_AnimationSetEnd())
		m_bJump[2] = TRUE;
	else if (m_bJump[1] && m_bJump[2] && m_pMesh->Is_AnimationSetEnd())
	{
		for (_uint i = 0; i < 3; ++i)
			m_bJump[i] = FALSE;

		m_eJumpState = JUMPEND;

		if (!m_bBreathDelay)
		{
			m_bFireBreath = TRUE;
			m_bBreathDelay = TRUE;
			Animate_FSM(_ATTACK_BREATH);
		}
		else if (m_bBreathDelay)
		{
			m_bSweep = TRUE;
			m_bBreathDelay = FALSE;
			Animate_FSM(_ATTACK_TAIL_SWEEP);
		}

		m_bAttack = TRUE;
		m_pTransform->m_bAttackState = TRUE;
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);
	}


}

VOID CKeroberos::State_Attack_Bite()
{
	if(m_iCurAniSet != _ATTACK_BITE_START)
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 5.f);

	_vec3 vDir = m_pTransform->Get_vLookDir();

	if (m_iCurAniSet == _ATTACK_BITE_START && !m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttStart = TRUE;
		m_AttackTime += m_TimeDelta;
		_bool bColl = m_pCollider_Head->Check_ComponentColl(m_pTargetSphereColl);

		if (bColl && m_AttackTime > 1.0)
		{
			m_pTransform->m_bAttackState = TRUE;
		
			if (!m_bTwice)
			{
				m_pTargetSphereColl->m_bHit = TRUE;

				m_bTwice = TRUE;
				m_pTargetSphereColl->Get_iHp(2);
			}


		}

	}
	else if (m_iCurAniSet == _ATTACK_BITE_START && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_ATTACK_BITE_END);
	}
	else if (m_iCurAniSet == _ATTACK_BITE_END && m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttack = FALSE;
		m_bBite = FALSE;
		m_bAttStart = FALSE;
		m_bTwice = FALSE;
		m_pTransform->m_bAttackState = FALSE;
		m_AttackTime = 0.0;
		Animate_FSM(_IDLE);
	}

	
	if (m_fDist < 4.f && !m_bAttStart)
		Animate_FSM(_ATTACK_BITE_START);
	else if (m_fDist > 4.f && !m_bAttStart)
	{
		Animate_FSM(_CHASE_RUN);
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 2.5f);
	}


}

VOID CKeroberos::State_Attack_Sweep()
{
	_matrix matTail;
	
	m_AttackTime += m_TimeDelta;

	if(m_AttackTime < 0.5f)
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_iCurAniSet == _ATTACK_TAIL_SWEEP && !m_pMesh->Is_AnimationSetEnd())
	{
		_bool bColl = m_pCollider_Tail1->Check_ComponentColl(m_pTargetSphereColl);
		_bool bColl2 = m_pCollider_Tail2->Check_ComponentColl(m_pTargetSphereColl);

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
		else if(bColl2 && m_AttackTime > 1.0)
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
	else if (m_iCurAniSet == _ATTACK_TAIL_SWEEP && m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttack = FALSE;
		m_bSweep = FALSE;
		m_bTwice = FALSE;
		m_pTransform->m_bAttackState = FALSE;
		m_AttackTime = 0.0;
		Animate_FSM(_IDLE);
	}

}

VOID CKeroberos::State_Attack_Firebreath()
{
	m_BreathTime += m_TimeDelta;

	_matrix matHead[3];
	Get_HeadMatrix("L_HEAD", &matHead[0]);
	Get_HeadMatrix("C_HEAD", &matHead[1]);
	Get_HeadMatrix("R_HEAD", &matHead[2]);

	if (m_iCurAniSet == _ATTACK_BREATH && !m_pMesh->Is_AnimationSetEnd())
	{
		if (m_BreathTime > 0.5)
		{
			for (int j = 0; j < 3; ++j)
			{
				_vec3 vHead = {};
				memcpy(vHead, matHead[j].m[3], sizeof(_vec3));

				_vec3 vDir = {};
				memcpy(vDir, matHead[j].m[1], sizeof(_vec3));
				vDir.y *= -1.f;

				CGameObject* pObject = CEffect_Kerofire::Create(m_pGraphicDev, vHead, _vec3(0.4f, 0.4f, 0.4f), 3.f, 0.25f);
				ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"Kerofire", pObject);
				CEffect_Kerofire* pTorch = dynamic_cast<CEffect_Kerofire*>(pObject);
				pTorch->Set_Move(TRUE, vDir);
			}
			m_BreathTime = 0.0;
		}
	}
	else if (m_iCurAniSet == _ATTACK_BREATH && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_IDLE);
		m_bFireBreath = FALSE;
		m_bBreathDelay = TRUE;
		m_bAttack = FALSE;
	}

}

VOID CKeroberos::State_Attack_TailCombo()
{
	if (m_iCurAniSet != _ATTACK_TAIL_COMBO1)
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 5.f);

	_vec3 vDir = m_pTransform->Get_vLookDir();

	m_pCollider_Tail2->Set_Scale(_SCALE * 2.f);
	if (m_iCurAniSet == _ATTACK_TAIL_COMBO1 && !m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttStart = TRUE;
		m_AttackTime += m_TimeDelta;
		_bool bColl = m_pCollider_Tail2->Check_ComponentColl(m_pTargetSphereColl);

		if (bColl && m_AttackTime > 1.0)
		{
			m_pTransform->m_bAttackState = TRUE;

			if (!m_bTwice)
			{
				m_pTargetSphereColl->m_bHit = TRUE;

				m_bTwice = TRUE;
				m_pTargetSphereColl->Get_iHp(2);
			}

		}

	}
	else if (m_iCurAniSet == _ATTACK_TAIL_COMBO1 && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_ATTACK_TAIL_COMBO2);
		m_AttackTime = 0;
		m_bTwice = FALSE;
	}
	else if (m_iCurAniSet == _ATTACK_TAIL_COMBO2 && !m_pMesh->Is_AnimationSetEnd())
	{
		m_AttackTime += m_TimeDelta;
		_bool bColl = m_pCollider_Tail2->Check_ComponentColl(m_pTargetSphereColl);

		if (bColl && m_AttackTime > 0.5)
		{
			m_pTransform->m_bAttackState = TRUE;

			if (!m_bTwice)
			{
				if(!m_pTargetSphereColl->Get_HitState())
					m_pTargetSphereColl->m_bHit = TRUE;
				else
				{
					m_pTargetSphereColl->m_bKnockBack = TRUE;

					_vec3 vKnockDir = m_pTargetSphereColl->Get_CollPos() - m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
					vKnockDir.y = 0.f;
					D3DXVec3Normalize(&vKnockDir, &vKnockDir);
					m_pTargetSphereColl->Set_KnockBackDist(TRUE, vKnockDir);
				}

				m_bTwice = TRUE;
				m_pTargetSphereColl->Get_iHp(2);
			}

		}
	}
	else if (m_iCurAniSet == _ATTACK_TAIL_COMBO2 && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_ATTACK_TAIL_COMBO3);
		m_AttackTime = 0;
		m_bTwice = FALSE;
	}
	else if (m_iCurAniSet == _ATTACK_TAIL_COMBO3 && !m_pMesh->Is_AnimationSetEnd())
	{
		m_AttackTime += m_TimeDelta;
		_bool bColl = m_pCollider_Tail2->Check_ComponentColl(m_pTargetSphereColl);

		if (bColl && m_AttackTime > 0.75)
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
	else if (m_iCurAniSet == _ATTACK_TAIL_COMBO3 && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_ATTACK_TAIL_COMBO4);
	}
	else if (m_iCurAniSet == _ATTACK_TAIL_COMBO4 && m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttack = FALSE;
		m_bTailCombo = FALSE;
		m_bAttStart = FALSE;
		m_bTwice = FALSE;
		m_pTransform->m_bAttackState = FALSE;
		m_AttackTime = 0.0;
	}


	if (m_fDist < 4.f && !m_bAttStart)
		Animate_FSM(_ATTACK_TAIL_COMBO1);
	else if (m_fDist > 4.f && !m_bAttStart)
	{
		Animate_FSM(_CHASE_RUN);
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 2.5f);
	}


}

VOID CKeroberos::State_Attack_BiteCombo()
{
	if (m_iCurAniSet != _ATTACK_BITE_COMBO1)
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 5.f);

	_vec3 vDir = m_pTransform->Get_vLookDir();

	if (m_iCurAniSet == _ATTACK_BITE_COMBO1 && !m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttStart = TRUE;
		m_AttackTime += m_TimeDelta;
		_bool bColl = m_pCollider_Head->Check_ComponentColl(m_pTargetSphereColl);

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
	else if (m_iCurAniSet == _ATTACK_BITE_COMBO1 && m_pMesh->Is_AnimationSetEnd())
	{
		Animate_FSM(_ATTACK_BITE_COMBO2);
	}
	else if (m_iCurAniSet == _ATTACK_BITE_COMBO2 && m_pMesh->Is_AnimationSetEnd())
	{
		m_bAttack = FALSE;
		m_bBiteCombo = FALSE;
		m_bAttStart = FALSE;
		m_bTwice = FALSE;
		m_pTransform->m_bAttackState = FALSE;
		m_AttackTime = 0.0;
		Animate_FSM(_IDLE);
	}


	if (m_fDist < 4.f && !m_bAttStart)
		Animate_FSM(_ATTACK_BITE_COMBO1);
	else if (m_fDist > 4.f && !m_bAttStart)
	{
		Animate_FSM(_CHASE_RUN);
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 2.5f);
	}

}

VOID CKeroberos::State_Dead()
{
	m_AttackTime += m_TimeDelta;
	m_bAttack = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	Animate_FSM(_DEAD);

	//if (m_AttackTime > 1.0 && m_AttackTime < 5.8)
	//	m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->Get_vLookDir() * m_TimeDelta * 0.25;

}

HRESULT CKeroberos::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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


HRESULT CKeroberos::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Boss_Keroberos"));
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

	//Collider_Head
	pComponent = m_pCollider_Head = ENGINE::CCollider::Create(m_pGraphicDev, 200.f, _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider_Head", pComponent);
	//Collider_Tail1
	pComponent = m_pCollider_Tail1 = ENGINE::CCollider::Create(m_pGraphicDev, 200.f, _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider_Tail1", pComponent);
	//Collider_Tail2
	pComponent = m_pCollider_Tail2 = ENGINE::CCollider::Create(m_pGraphicDev, 250.f, _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider_Tail2", pComponent);


	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 50);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

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

CKeroberos * CKeroberos::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CKeroberos* pInstance = new CKeroberos(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CKeroberos::Free()
{
	ENGINE::CGameObject::Free();
}
