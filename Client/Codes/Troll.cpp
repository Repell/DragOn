#include "stdafx.h"
#include "Troll.h"

#include "Export_Function.h"

#define _SPEED 1.4f
#define _ANGLE 60.f
#define  _RADIUS 140.f

//STATE DEFINE
#define _IDLE								0
#define _CHASE_RUN					2
#define _CHASE_WALK				4
#define _ATTACK_START			8
#define	_ATTACK_END				9
#define _HIT									18
#define _AIRBORNE						18
#define _KnockBack_A				0
#define _KnockBack_B				0
#define _KnockBack_C				0
#define _KnockBack_D				0
#define  _DEAD							21
#define _RISEUP							19	//���
//Unique State
#define  _SLEEP							14
#define _AWAKEN						15
#define _GROWL						5


CTroll::CTroll(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	m_bAttack = FALSE;
	m_bSleep = TRUE;
	m_bDead = FALSE;
	m_bHit = FALSE;
	m_bKnockBack = FALSE;
	m_bAirborne = FALSE;
	m_bTwice = FALSE;

	m_iCurAniSet = 0;
	m_iPreAniSet = 0;

	m_HitTime = 0.0;
	m_AttackTime = 0.0;
	m_AirTime = 0.0;
	m_KnockTime = 0.0;
	m_TimeDelta = 0.0;
	m_AccTime = 1.0;

}

CTroll::~CTroll()
{

}

HRESULT CTroll::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_FSM(_SLEEP);

	m_pNaviMesh->Set_CurrentIdx(3);
	m_pTransform->m_vAngle.y = 0.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;

	return S_OK;
}

HRESULT CTroll::Late_Init()
{
	//m_pTransform->m_vAngle.y = 180.f;
	m_pTransform->m_vScale = { 0.008f, 0.008f, 0.008f };
	//m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 40.f, 0.1f, 3.f };
	m_pSphereColl->Set_Scale(0.007f);
	m_pCollider->Set_Scale(0.007f);

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

	if (m_bDead && m_iCurAniSet == _DEAD && m_pMesh->Is_AnimationSetEnd())
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
	//Total Enemy Behavior
	Set_Behavior_Progress();

	if (m_iCurAniSet == 2)
		m_pMesh->Play_AnimationSet(m_TimeDelta * 1.5);
	else
		m_pMesh->Play_AnimationSet(m_TimeDelta * 1.1);
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

	Get_WeaponMatrix("L_IRON1");
	if (m_bAttack)
		m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix);

	if (!m_bDead)
		m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);

	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"AngleY : %3.2f", m_pTransform->m_vAngle.y);
	////swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CTroll::Set_Animation()
{
	m_iKnockCnt = 0;
	m_iKnockIdx[0] = _KnockBack_A;
	m_iKnockIdx[1] = _KnockBack_B;
	m_iKnockIdx[2] = _KnockBack_C;
	m_iKnockIdx[3] = _KnockBack_D;
	m_iKnockIdx[4] = _RISEUP;
}

void CTroll::Get_WeaponMatrix(const char * tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

void CTroll::Check_EnemyGroup()
{
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	_vec3 vRevDir = { 0.f, 0.f, 0.f };	//�ֺ��� �ɸ��� �Ÿ��� �� ������ �о�� �Ÿ�

	if (Check_EnemyColl(&vRevDir, L"Troll"))	//��ü �� �浹 üũ
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Swordman"))	//��ü �� �浹 üũ
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Spearman"))	//��ü �� �浹 üũ
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Shieldman"))	//��ü �� �浹 üũ
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

	if (Check_EnemyColl(&vRevDir, L"Enemy_Bowman"))	//��ü �� �浹 üũ
	{
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRevDir * _SPEED * m_TimeDelta;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] =
			m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	}

}

_bool CTroll::Check_EnemyColl(_vec3 * vRevDir, const _tchar * szTag)
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

		//*vRevDir = m_pTransform->Get_TargetReverseDir(pTrans);

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

VOID CTroll::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}

VOID CTroll::Set_Behavior_Progress()
{
	/// ���� Bool ������ ����Ǵ� ���¸� �ϳ��� ���� FLAG �������� �����Ͽ� ���� ������ ���� _uint m_StateFlag , Swith�� ����
	//Check State
	//m_bHit = m_pSphereColl->Get_HitState();
	//m_bKnockBack = m_pSphereColl->Get_KnockBackState();
	m_bAirborne = m_pSphereColl->Get_AirboneState();

	if (!m_bDead)	//���׾��� ��
	{
		//State Awaken
		if (m_bSleep)	//���� �� �⺻ ��� ����
			AiState = &CTroll::State_Awaken;
		//State Hit
		//else if (m_bHit && !m_bKnockBack && !m_bAirborne)	//���� ���� ��, ����
		//	AiState = &CTroll::State_Hit;
		////State KnockBack
		//else if (m_bKnockBack)		//�˹� ���� ����
		//	AiState = &CTroll::State_KnockBack;
		//State Airbone -> Troll to Hit
		else if (m_bAirborne && !m_bKnockBack)	//��� ���ݹ���
			AiState = &CTroll::State_Airborne;
		//State Chase
		else if (!m_bAttack && !m_bHit && m_fDist < 10.f)	//�÷��̾� ���� ����
			AiState = &CTroll::State_Chase;
		//State Attack
		else if (m_bAttack && !m_bHit)		//�÷��̾� ���� �԰���
			AiState = &CTroll::State_Attack;

		//State Idle
		if (m_fDist > 10.f && !m_bHit && !m_bKnockBack && !m_bAirborne && !m_bSleep)	//�ƹ��ϵ�... ������!
			AiState = &CTroll::State_Idle;
	}
	else if (m_bDead)		//���� ���
							//State Dead	
		AiState = &CTroll::State_Dead;

	//������Ʈ ���� �Ϸ� �� Func Pointer Start
	(this->*AiState)();
}

VOID CTroll::State_Awaken()
{
	if (m_fDist < 10.f && m_iCurAniSet == _SLEEP) //��� ���¿��� �÷��̾� �߰�
	{
		Animate_FSM(_AWAKEN);	
	}
	else if (m_iCurAniSet == _AWAKEN && m_pMesh->Is_AnimationSetEnd())	//��� ���� ����, �ൿ ����
	{
		Animate_FSM(_GROWL);
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 20.f);
	}
	else if (m_iCurAniSet == _GROWL && m_pMesh->Is_AnimationSetEnd())	//��� ���� ����, �ൿ ����
	{
		Animate_FSM(_IDLE);
		m_bSleep = FALSE;
	}
}

VOID CTroll::State_Hit()
{
	m_HitTime += m_TimeDelta;
	Animate_FSM(_HIT);
	m_AccTime = 3.0;

	if (m_HitTime > 0.5f && m_pMesh->Is_AnimationSetEnd())	//1�� ������ ��������
	{
		m_pSphereColl->m_bHit = FALSE;
		m_HitTime = 0.0;
		m_AccTime = 1.0;

		Animate_FSM(_IDLE);
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CTroll::State_KnockBack()
{
	Animate_FSM(m_iKnockIdx[m_iKnockCnt]);	//ù��° �ִ� ���

	if (m_iKnockCnt == 4 && !m_pMesh->Is_AnimationSetEnd())	//���� �� ��� ���
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	else if (m_iKnockCnt == 4 && m_pMesh->Is_AnimationSetEnd())	//���� ����
	{
		m_pSphereColl->m_bHit = FALSE;
		m_pSphereColl->m_bKnockBack = FALSE;
		m_pSphereColl->m_bAirbone = FALSE;

		m_iKnockCnt = 0;
		m_AccTime = 1.0;
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}
	else if (m_iKnockCnt >= 0 && m_iKnockCnt <= 2 && !m_pMesh->Is_AnimationSetEnd())	//���� �� ���ư�
	{
		_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		(&vPos, &(m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f));
	}
	else if (m_iCurAniSet == m_iKnockIdx[m_iKnockCnt] && m_pMesh->Is_AnimationSetEnd())	//���� ī��Ʈ ����
	{
		m_AccTime = 2.0;
		++m_iKnockCnt;
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	}

}

VOID CTroll::State_Airborne()
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

		if (m_AirTime < 0.75)
		{
			//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
			//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
			//(&vPos, &(m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f));
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f;
		}

		m_bAttack = FALSE;
		m_bTwice = FALSE;
		m_pTransform->m_bAttackState = FALSE;
	}

	if (m_AirTime > 1.5f && m_iCurAniSet == _AIRBORNE && m_pMesh->Is_AnimationSetEnd())	//1�� ������ ��������
	{
		m_AccTime = 1.0;
		Animate_FSM(_RISEUP);
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CTroll::State_Idle()
{
	Animate_FSM(_IDLE);
	m_bAttack = FALSE;
	m_bTwice = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

}

VOID CTroll::State_Chase()
{
	//�÷��̾ �Ÿ� ���� ������ ������ �þ� ����
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_fDist > 3.f) //�÷��̾ �ָ� ����, �޷��� ����
	{
		Animate_FSM(_CHASE_RUN);

		Check_EnemyGroup();

		//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		//(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 1.5f);

		m_AttackTime = 0.0;
	}
	else if (m_fDist < 3.f)
	{
		Animate_FSM(_CHASE_WALK);

		Check_EnemyGroup();

		if (m_fDist > 2.75f)
		{
			//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
			//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
			//(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));
			m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 1.5f);
		}

		else if (m_fDist < 2.75f)	//�÷��̾�� �Ÿ� ����
		{
			m_AttackTime += m_TimeDelta;	//���� �õ��� ��� �ð�

			if (m_AttackTime > 1.5)	// �÷��̾�� �����Ÿ��� ������ü �ð��� ä��
			{
				Animate_FSM(_IDLE);
				m_bAttack = TRUE;	//���� ����
				m_pTransform->m_bAttackState = m_bAttack;
				m_AttackTime = 0.0;
			}
		}

	}
}

VOID CTroll::State_Attack()
{
	_bool bColl = FALSE;

	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_fDist < 2.25f && m_iCurAniSet == _CHASE_WALK || m_iCurAniSet == _IDLE) // �Ÿ��� �Ƴ� ����
	{
		Animate_FSM(_ATTACK_START);
	}
	else if (m_iCurAniSet == _ATTACK_START && !m_pMesh->Is_AnimationSetEnd())
	{
		m_AttackTime += m_TimeDelta;
		bColl = m_pCollider->Check_ComponentColl(m_pTargetSphereColl);

		if (bColl && m_AttackTime > 1.25)
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
	else if (m_iCurAniSet == _ATTACK_START && m_pMesh->Is_AnimationSetEnd())
	{
		m_pTransform->m_bAttackState = FALSE;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

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

	if (m_fDist > 2.25f && !m_bTwice)	//���� �غ�ƴµ� �Ÿ��� �� �ӳ�
	{
		m_pTransform->m_bAttackState = FALSE;
		//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
		m_AttackTime = 0.0;
		Animate_FSM(_CHASE_WALK);

		//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
		//m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh
		//(&vPos, &m_pTransform->Stalk_TargetDir(m_pTargetTransform, m_TimeDelta, 1.5f));
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 1.5f);

		Check_EnemyGroup();
	}
}

VOID CTroll::State_Dead()
{
	m_AttackTime += m_TimeDelta;
	m_bAttack = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	//m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	Animate_FSM(_DEAD);

	if (m_AttackTime > 1.0 && m_AttackTime < 5.8)
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->Get_vLookDir() * m_TimeDelta * 0.25;

}

HRESULT CTroll::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev, 100.f, _vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 20);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

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
