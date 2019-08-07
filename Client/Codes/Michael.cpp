#include "stdafx.h"
#include "Michael.h"


#include "Export_Function.h"

#define _SPEED 15.f
#define _ANGLE 15.f
#define  _RADIUS 200.f

///////////// State Define

#define _FLYIDLE			22		//
#define _FLYUP			0			//FLYUP
#define _FLYDOWN		25		//FLYDOWN
#define _TURNLEFT		20	//FLYLEFT
#define _TURNRIGHT	21	//FLYRIGHT
#define _TURNBACK	24	//FLYBACK

#define _ATTACKFIREBALL 6

#define _FLYINGA 11
#define _FLYINGB 12


CMichael::CMichael(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr),
	m_bAI(FALSE), m_AiFireTime(0.0)
{
	bAttack = FALSE;
	
	iCurAniSet = 0;
	m_TimeAccel = 0.0;
	m_fFireGauge = 0.f;
}

CMichael::~CMichael()
{

}

HRESULT CMichael::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMesh->Set_AnimationSet(11);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vScale = { 0.006f, 0.006f, 0.006f };
	m_pSphereColl->Set_Scale(0.006f);
	m_pCollider->Set_Scale(0.006f);

	m_pAi_Transform->m_vInfo[ENGINE::INFO_POS] = m_pAi_Transform->m_vStartPos + (m_pAi_Transform->Get_vLookDir() * 20.f);
	return S_OK;
}

HRESULT CMichael::Late_Init()
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);
	list<CGameObject*> pList =  pLayer->Get_MapObject(L"Player");
	m_pPlayer = dynamic_cast<CNewPlayer*>(pList.front());
	
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

	return S_OK;
}

_int CMichael::Update_Object(const _double& TimeDelta)
{
	CGameObject::Late_Init();
	////////////////////////		▼최우선 함수

	if (!m_bAI)
	{
		MouseFunc();

		Key_check(TimeDelta);

		Set_Behavior_Progress(TimeDelta);
		
		m_AiFireTime = 0;
	}
	else if (m_bAI)	//Ai Update
	{
		m_AiFireTime += TimeDelta;
		
		m_pAi_Transform->m_vAngle.y += 45.f * TimeDelta;
		m_pAi_Transform->m_vInfo[ENGINE::INFO_POS] = m_pAi_Transform->m_vStartPos + (m_pAi_Transform->Get_vLookDir() * 80.f);
		m_pAi_Transform->Update_Component(TimeDelta);
		memcpy(m_vChasePos, m_pAi_Transform->m_matWorld.m[3], sizeof(_vec3));

		Ai_State();

		if (m_AiFireTime > 2.5)
			Ai_Fire();

	}
	////////////////////////		▼조건 함수
	RelicColl_Check(TimeDelta);

	if(!m_bAI)
		Set_AdvanceCamera();

	//if (m_pSphereColl->Get_iHp() <= 0)
	//	return END_EVENT;

	//////////////////////// ▼상시 함수
	CGameObject::Update_Object(TimeDelta);

	m_TimeAccel = 1.5;
	m_pMesh->Play_AnimationSet(TimeDelta * m_TimeAccel);
	//////////////////////// ▼최후미 함수
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CMichael::Late_Update_Object()
{
	CGameObject::Late_Update_Object();
	//////////////////////
	
	if (ENGINE::Key_Down(ENGINE::dwKEY_F3))
	{
		if (!m_bAI)
		{
			m_bAI = TRUE;
			m_pPlayer->Set_PlayerActive(TRUE);
			m_pAdvance->Set_ActiveCamera(FALSE);
		}
		else
		{
			m_bAI = FALSE;
			m_pPlayer->Set_PlayerActive(FALSE);
			m_pAdvance->Set_ActiveCamera(TRUE);
		}

		m_pTransform->m_bAi = m_bAI;
	}

}

void CMichael::Render_Object()
{
	//Render_Set();
	if (nullptr == m_pShader)
		return;

	
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();

	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	///////////////////////////////////

	m_pMesh->Render_Meshes();

	///////////////////////////////////
	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	///////////////////////////////////

	//Find_BoneMatrix("HEAD");
	//m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pBoneMatrix);

	m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pAi_Transform->m_matWorld);
	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld, 1.5f);

	//_tchar szStr[MAX_PATH] = L"";

	//swprintf_s(szStr, L"PlayerAngle | X : %3.2f, Y : %3.2f,  Z : %3.2f ", m_pTransform->m_vAngle.x, m_pTransform->m_vAngle.y, m_pTransform->m_vAngle.z);
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 30.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//swprintf_s(szStr, L"PlayerPos | X : %3.2f, Y : %3.2f,  Z : %3.2f ", m_pTransform->m_vInfo[ENGINE::INFO_POS].x, m_pTransform->m_vInfo[ENGINE::INFO_POS].y, m_pTransform->m_vInfo[ENGINE::INFO_POS].z);
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 50.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//swprintf_s(szStr, L"FireGauge : %4.2f ", m_fFireGauge);
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


}

_vec3 CMichael::MouseFunc()
{
	POINT pt = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	return _vec3(_float(pt.x), _float(pt.y), 0.f);
}

_bool CMichael::Key_check(const _double & TimeDelta)
{
	m_eDir = FLYEND;
	_vec3 vNewDir = m_pTransform->Get_vLookRealDir();
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);

	_long dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
		m_pTransform->m_vAngle.y += dwMouseMove * _ANGLE * TimeDelta;

	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	{
			if (m_pTransform->m_vAngle.x > 60.f)
				m_pTransform->m_vAngle.x = 60.f;
			else if(m_pTransform->m_vAngle.x < -60.f)
				m_pTransform->m_vAngle.x = -60.f;
			else
				m_pTransform->m_vAngle.x -= dwMouseMove * _ANGLE * TimeDelta;
	}

	if (ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON) && m_fFireGauge > 200.f)
	{
		Find_BoneMatrix("JAW");
		m_fFireGauge -= 200.f;

		_vec3 vFirePos = {};
		memcpy(vFirePos, m_pBoneMatrix.m[3], sizeof(_vec3));
		ENGINE::UNITINFO tInfo =
		{ FALSE, vFirePos, _vec3(0.1f, 0.1f, 0.1f),m_pTransform->m_vAngle, m_pTransform->Get_vInfoPos(ENGINE::INFO_LOOK),  15.f };
		CGameObject* pObject = CDragon_GroundFire::Create(m_pGraphicDev, tInfo);
		ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"Dragon_Fireball", pObject);
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_SPACE))
	{
		m_eDir = FLYUP;
		m_pTransform->m_vInfo[ENGINE::INFO_POS].y += _SPEED * TimeDelta;

		if (m_iCurAniState == _FLYUP && m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_FLYINGB);
		else
			Animate_FSM(_FLYUP);
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_LCntl))
	{
		m_eDir = FLYDOWN;
		m_pTransform->m_vInfo[ENGINE::INFO_POS].y -= _SPEED * TimeDelta;

		if (m_iCurAniState == _FLYDOWN && m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_FLYINGB);
		else
			Animate_FSM(_FLYDOWN);
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_W))
	{
		m_eDir = FLYIDLE;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] -= vNewDir * _SPEED * TimeDelta;

		if(m_iCurAniState == _FLYINGB && m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_FLYINGA);
		else
			Animate_FSM(_FLYINGB);
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_S))
	{
		m_eDir = FLYBACK;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += vNewDir * _SPEED * TimeDelta;
	
		if (m_iCurAniState == _TURNBACK && m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_FLYINGA);
		else
			Animate_FSM(_TURNBACK);
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		m_eDir = FLYLEFT;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] -= vNewDir.NewDir(&vNewDir, &_vec3(0.f, 1.f, 0.f)) * _SPEED * TimeDelta;
	
		if (m_iCurAniState == _TURNLEFT && m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_FLYINGB);
		else
			Animate_FSM(_TURNLEFT);
	}
	else if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		m_eDir = FLYRIGHT;
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += vNewDir.NewDir(&vNewDir, &_vec3(0.f, 1.f, 0.f)) * _SPEED * TimeDelta;
		
		if (m_iCurAniState == _TURNRIGHT && m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_FLYINGB);
		else
			Animate_FSM(_TURNRIGHT);
	}

	

	if (m_eDir != FLYEND && m_eState != FIRE)
		m_eState = FLY;
	else if (m_eDir != FLYEND && m_eState == FIRE)
		m_eState = FIRE;
	else
		m_eState = IDLE;

	return FALSE;
}

_bool CMichael::EnemyColl_Check()
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);
	if (pLayer->Get_MapObject(L"Boss_Blue").empty())
		return FALSE;

	return TRUE;
}

_bool CMichael::RelicColl_Check(const _double& TimeDelta)
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::ENVIRONMENT);
	
	if (pLayer->Get_MapObject(L"Mesh_Relic").empty())
		return FALSE;

	for (auto pList : pLayer->Get_MapObject(L"Mesh_Relic"))
	{
		ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (m_pSphereColl->Check_ComponentColl(pSphere))
		{
			if(m_fFireGauge < 1000.f)
				m_fFireGauge += 100.f * TimeDelta;

			return TRUE;
		}

	}

	return FALSE;
}

void CMichael::Set_AdvanceCamera()
{
	_vec3 vEye = vEye.Reverse(&m_pTransform->Get_vLookRealDir());

	vEye *= -15.f;
	vEye += m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
	
	m_pAdvance->Set_Transform_Pos(ENGINE::INFO_POS, &vEye);

	m_pAdvance->Set_PlayerTransform(m_pTransform);
}

void CMichael::Ai_State()
{
	m_pTransform->Chase_Target(&m_vChasePos, 0.75f);
	m_pTransform->Fix_TargetLook(m_pAi_Transform, 100.f);
}

void CMichael::Ai_Fire()
{
	m_AiFireTime = 0;

	Find_BoneMatrix("JAW");

	_vec3 vFirePos = {};
	memcpy(vFirePos, m_pBoneMatrix.m[3], sizeof(_vec3));
	_vec3 vLook = m_pTransform->Get_vLookDir();
	vLook.y += 0.35f;
	ENGINE::UNITINFO tInfo =
	{ FALSE, vFirePos, _vec3(0.1f, 0.1f, 0.1f),m_pTransform->m_vAngle, vLook,  30.f };
	CGameObject* pObject = CDragon_GroundFire::Create(m_pGraphicDev, tInfo);
	ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"Dragon_GroundFire", pObject);

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

void CMichael::Find_BoneMatrix(const char* szBone)
{
	const ENGINE::D3DXFRAME_DERIVED* pFrame = m_pMesh->Get_FrameByName(szBone);
	m_pBoneMatrix = pFrame->combinedTransformMatrix * m_pTransform->m_matWorld;
}

VOID CMichael::Animate_FSM(_uint iAniState)
{
	m_iCurAniState = iAniState;

	if (m_iCurAniState != m_iPreAniState)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniState);
		m_iPreAniState = m_iCurAniState;
	}
}

VOID CMichael::Animate_Quick(_uint iAniState)
{
	m_iCurAniState = iAniState;

	if (m_iCurAniState != m_iPreAniState)
	{
		m_pMesh->Set_QuickSet(m_iCurAniState);
		m_iPreAniState = m_iCurAniState;
	}
}

VOID CMichael::Set_Behavior_Progress(const _double & TimeDelta)
{
	if (m_eState == IDLE)
	{
		Animate_FSM(_FLYIDLE);
	}
}

HRESULT CMichael::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pTransform->m_matWorld);

	pEffect->SetMatrix("g_matView", &matView);

	pEffect->SetMatrix("g_matProj", &matProj);

	ENGINE::Safe_Release(pEffect);

	return S_OK;
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
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, -1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	m_pAi_Transform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, -1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pAi_Transform->Set_StartPos(_vec3(128.f, 32.f, 128.f));
	m_pAi_Transform->m_vScale = { 0.01f, 0.01f, 0.01f };

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	//AdvanceCamera
	pComponent = m_pAdvance = ENGINE::CAdvanceCamera::Create(m_pGraphicDev, m_pTransform);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_AdvCam", pComponent);
	m_pAdvance->Get_Transform()->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_pAdvance->Get_Transform()->m_vInfo[ENGINE::INFO_POS] = { 128.f, 32.f, 127.f};
	m_pAdvance->Set_ActiveCamera(TRUE);

	//Collider
	pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev, 80.f, _vec3(0.f, 1.5f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

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
