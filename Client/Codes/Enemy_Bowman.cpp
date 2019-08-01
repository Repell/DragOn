#include "stdafx.h"
#include "Enemy_Bowman.h"

#include "Export_Function.h"

#define _SPEED 1.5f
#define _ANGLE 60.f
#define  _RADIUS 100.f

//STATE DEFINE
#define _IDLE								42
#define _CHASE_RUN					1
#define _CHASE_WALK				2
#define _ATTACK_START			5
#define	_ATTACK_END				6
#define _ATTACK2_START		8
#define	_ATTACK2_END				9
#define _AWAKEN						33
#define _HIT									14
#define _AIRBORNE						17
#define _KnockBack_A				23
#define _KnockBack_B				24
#define _KnockBack_C				25
#define _KnockBack_D				26
#define  _DEAD							27
#define _RISEUP							32	//���
//Unique State
#define  _FALLBACK						34


CEnemy_Bowman::CEnemy_Bowman(LPDIRECT3DDEVICE9 pDevice)
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

CEnemy_Bowman::~CEnemy_Bowman()
{

}

HRESULT CEnemy_Bowman::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(5);
	//m_iCurAniSet = 5;
	Animate_FSM(_IDLE);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { 0.006f, 0.006f, 0.006f };
	m_pSphereColl->Set_Scale(0.005f);
	//m_pCollider->Set_Scale(0.005f);

	Set_Animation();

	return S_OK;
}

HRESULT CEnemy_Bowman::Late_Init()
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

_int CEnemy_Bowman::Update_Object(const _double& TimeDelta)
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

void CEnemy_Bowman::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	m_fDist = m_pTransform->Get_TargetDistance(m_pTargetTransform);
}

void CEnemy_Bowman::Render_Object()
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
	Get_WeaponMatrix("L_Hand");
	m_pWeapon->Render_Weapon(&m_pBoneMatrix);

	if (!m_bDead)
		m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld, 0.75f);

	////////////////////////////////////////

	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"AngleY : %3.2f", m_pTransform->m_vAngle.y);
	////swprintf_s(szStr, L"Monster HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CEnemy_Bowman::Check_EnemyGroup()
{
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	_vec3 vRevDir = { 0.f, 0.f, 0.f };	//�ֺ��� �ɸ��� �Ÿ��� �� ������ �о�� �Ÿ�

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

_bool CEnemy_Bowman::Check_EnemyColl(_vec3 * vRevDir, const _tchar* szTag)
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

VOID CEnemy_Bowman::Animate_FSM(_uint iAniState)
{
	m_iCurAniSet = iAniState;

	if (m_iCurAniSet != m_iPreAniSet)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniSet);
		m_iPreAniSet = m_iCurAniSet;
	}
}

void CEnemy_Bowman::Set_Animation()
{
	m_iKnockCnt = 0;
	m_iKnockIdx[0] = _KnockBack_A;
	m_iKnockIdx[1] = _KnockBack_B;
	m_iKnockIdx[2] = _KnockBack_C;
	m_iKnockIdx[3] = _KnockBack_D;
	m_iKnockIdx[4] = _RISEUP;
}

VOID CEnemy_Bowman::Set_Behavior_Progress()
{
	/// ���� Bool ������ ����Ǵ� ���¸� �ϳ��� ���� FLAG �������� �����Ͽ� ���� ������ ���� _uint m_StateFlag , Swith�� ����
	//Check State
	m_bHit = m_pSphereColl->Get_HitState();
	m_bKnockBack = m_pSphereColl->Get_KnockBackState();
	m_bAirborne = m_pSphereColl->Get_AirboneState();

	if (!m_bDead)	//���׾��� ��
	{
		//State Awaken
		if (m_bSleep)	//���� �� �⺻ ��� ����
			AiState = &CEnemy_Bowman::State_Awaken;
		//State Hit
		else if (m_bHit && !m_bKnockBack && !m_bAirborne)	//���� ���� ��, ����
			AiState = &CEnemy_Bowman::State_Hit;
		//State KnockBack
		else if (m_bKnockBack)		//�˹� ���� ����
			AiState = &CEnemy_Bowman::State_KnockBack;
		//State Airbone
		else if (m_bAirborne && !m_bKnockBack)	//��� ���ݹ���
			AiState = &CEnemy_Bowman::State_Airborne;
		//State Fallback
		else if (!m_bAttack && !m_bHit && m_fDist < 5.0f)	//�÷��̾ �ʹ� ������!
			AiState = &CEnemy_Bowman::State_Fallback;
		//State Chase
		else if (!m_bAttack && !m_bHit && m_fDist < 12.f)	//�÷��̾� ���� ����
			AiState = &CEnemy_Bowman::State_Chase;
		//State Attack
		else if (m_bAttack && !m_bHit)		//�÷��̾� ���� �԰���
			AiState = &CEnemy_Bowman::State_Attack;

		//State Idle
		if (m_fDist > 10.f && !m_bHit && !m_bKnockBack && !m_bAirborne)	//�ƹ��ϵ�... ������!
			AiState = &CEnemy_Bowman::State_Idle;
	}
	else if (m_bDead)		//���� ���
	//State Dead	
		AiState = &CEnemy_Bowman::State_Dead;

	//������Ʈ ���� �Ϸ� �� Func Pointer Start
	(this->*AiState)();
}

HRESULT CEnemy_Bowman::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CEnemy_Bowman::Get_WeaponMatrix(const char* tBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(tBone);

	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}



HRESULT CEnemy_Bowman::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////
	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Enemy_Bowman"));
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
	{ FALSE, _vec3(0.f, 0.f, 0.f), _vec3{ 0.01f, 1.f, 1.f }, _vec3(90.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 20.f};
	pComponent = m_pWeapon = ENGINE::CWeapon::Create(m_pGraphicDev, m_pTransform, tInfo, L"Mesh_Enemy_Bow");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Weapon", pComponent);

	////////////////////////////
	return S_OK;
}

CEnemy_Bowman * CEnemy_Bowman::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEnemy_Bowman* pInstance = new CEnemy_Bowman(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CEnemy_Bowman::Free()
{
	ENGINE::CGameObject::Free();
}

VOID CEnemy_Bowman::State_Awaken()
{
	if (m_fDist < 10.f && m_iCurAniSet == _IDLE) //��� ���¿��� �÷��̾� �߰�
	{
		Animate_FSM(_AWAKEN);	//��� ����
		m_pTransform->Fix_TargetLook(m_pTargetTransform, 20.f);

	}
	else if (m_iCurAniSet == _AWAKEN && m_pMesh->Is_AnimationSetEnd())	//��� ���� ����, �ൿ ����
	{
		Animate_FSM(_IDLE);
		m_bSleep = FALSE;
	}
}

VOID CEnemy_Bowman::State_Hit()
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
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CEnemy_Bowman::State_KnockBack()
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
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}
	else if (m_iKnockCnt >= 0 && m_iKnockCnt <= 2 && !m_pMesh->Is_AnimationSetEnd())	//���� �� ���ư�
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pSphereColl->Set_KnockBackDist(FALSE) * m_TimeDelta * 2.f;
	else if (m_iCurAniSet == m_iKnockIdx[m_iKnockCnt] && m_pMesh->Is_AnimationSetEnd())	//���� ī��Ʈ ����
	{
		m_AccTime = 2.0;
		++m_iKnockCnt;
		Animate_FSM(m_iKnockIdx[m_iKnockCnt]);
	}

}

VOID CEnemy_Bowman::State_Airborne()
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

	if (m_AirTime > 1.5f && m_iCurAniSet == _AIRBORNE && m_pMesh->Is_AnimationSetEnd())	//1�� ������ ��������
	{
		m_AccTime = 1.0;
		Animate_FSM(_RISEUP);
		m_bAttack = FALSE;
		m_pTransform->m_bAttackState = m_bAttack;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	}

}

VOID CEnemy_Bowman::State_Idle()
{
	Animate_FSM(_IDLE);
	m_bAttack = FALSE;
	m_bTwice = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);

}

VOID CEnemy_Bowman::State_Fallback()
{
	Animate_FSM(_CHASE_RUN);
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	_vec3 vRevDir = m_pTransform->Get_TargetReverseDir(m_pTargetTransform);
	
	m_pTransform->Fix_TargetRevLook(m_pTargetTransform, 20.f);
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vRevDir * _SPEED * m_TimeDelta));
	
}

VOID CEnemy_Bowman::State_Chase()
{

	//�÷��̾ �Ÿ� ���� ������ ������ �þ� ����
	m_pTransform->Fix_TargetLook(m_pTargetTransform, 12.f);

	if (m_fDist > 8.f) //�÷��̾ �ָ� ����, �޷��� ����
	{
		_vec3 vRevDir = { 0.f, 0.f, 0.f };	//�ֺ��� �ɸ��� �Ÿ��� �� ������ �о�� �Ÿ�
		Animate_FSM(_CHASE_RUN);

		Check_EnemyGroup();

		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, _SPEED);

		m_AttackTime = 0.0;
	}
	else if (m_fDist < 8.f)
	{
		_vec3 vRevDir = { 0.f, 0.f, 0.f };	//�ֺ��� �ɸ��� �Ÿ��� �� ������ �о�� �Ÿ�
		Animate_FSM(_CHASE_WALK);

		Check_EnemyGroup();

		if (m_fDist > 6.f)
			m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 0.75f);
		else if (m_fDist < 6.f)	//�÷��̾�� �Ÿ� ����
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

VOID CEnemy_Bowman::State_Attack()
{

	m_pTransform->Fix_TargetLook(m_pTargetTransform, 10.f);

	if (m_fDist < 5.5f && m_iCurAniSet == _CHASE_WALK || m_iCurAniSet == _IDLE) // �Ÿ��� �Ƴ� ����
	{
		//if(m_pTargetTransform->Get_vInfoPos(ENGINE::INFO_POS).y > 2.f)
		//	Animate_FSM(_ATTACK2_START);
		//else
			Animate_FSM(_ATTACK_START);
	}
	else if (!m_pMesh->Is_AnimationSetEnd() && m_iCurAniSet == _ATTACK_START)
	{
		//m_AccTime = 2.0;
		m_AttackTime += m_TimeDelta;

		if (m_AttackTime > 2.8 && !m_bTwice)
		{
			m_pWeapon->Set_AttackState(m_bAttack, m_iCurAniSet, 2);

			ENGINE::UNITINFO tInfo =
			{FALSE, m_pTransform->Get_vInfoPos(ENGINE::INFO_POS), _vec3(0.003f, 0.003f, 0.003f),m_pTransform->m_vAngle, m_pTransform->Get_vInfoPos(ENGINE::INFO_LOOK),  35.f};
			CGameObject* pObject = CEnemy_Arrow::Create(m_pGraphicDev, tInfo);
			ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"Enemy_Arrow", pObject);
			
			m_bTwice = TRUE;
		}

	}
	else if (m_pMesh->Is_AnimationSetEnd() && m_iCurAniSet == _ATTACK_START)
	{
		m_AccTime = 1.0;
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

	if (m_fDist > 5.5f && !m_bTwice)	//���� �غ�ƴµ� �Ÿ��� �� �ӳ�
	{
		_vec3 vRevDir = { 0.f, 0.f, 0.f };

		m_pTransform->m_bAttackState = FALSE;
		m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
		m_AttackTime = 0.0;
		Animate_FSM(_CHASE_WALK);
		m_pTransform->Stalk_Target(m_pTargetTransform, m_TimeDelta, 0.75f);

		Check_EnemyGroup();
	}


}

VOID CEnemy_Bowman::State_Dead()
{
	m_AttackTime += m_TimeDelta;
	m_bAttack = FALSE;
	m_pTransform->m_bAttackState = m_bAttack;
	m_pWeapon->Set_AttackState(FALSE, m_iCurAniSet);
	Animate_FSM(_DEAD);

	if (m_AttackTime > 1.0 && m_AttackTime < 5.8)
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->Get_vLookDir() * m_TimeDelta * 0.25;

}
