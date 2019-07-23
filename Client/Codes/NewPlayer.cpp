#include "stdafx.h"
#include "NewPlayer.h"

#include "Export_Function.h"

#define _SPEED 3.f
#define _ANGLE 45.f
#define  _RADIUS 75.f
#define _GRAVITY 2.8f
#define _JUMPPOWER 0.75f
#define  _IDLE 110

CNewPlayer::CNewPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pTransform(nullptr), m_pNaviMesh(nullptr), m_pMesh(nullptr),
	m_pSphereColl(nullptr), m_pRenderer(nullptr), m_pSword(nullptr),
	m_bAnimate(FALSE), m_iCurAniState(0), m_iPreAniState(0), m_TimeAccel(1),
	m_bJump(FALSE), m_fPosY(0.f), m_JumpTime(0.0),
	m_bDash(FALSE), m_DashTime(0.0),
	 m_AttackTime(0.0)
{
	m_iAniSet[0] = 90;	// 89 / Attack 1 
	m_iAniSet[1] = 88;	// 83/ Attack 4
	m_iAniSet[2] = 86;	// 83/ Attack 4
	m_iAniSet[3] = 84;	// 77 / Attack 7
	m_iAniSet[4] = 82;	// 79 / Attack 6
	m_iAniSet[5] = 80;	// 66 / Attack 12

	for (int i = 0; i < 6; ++i)
		m_bAttack[i] = FALSE;
	

}

CNewPlayer::~CNewPlayer()
{

}

HRESULT CNewPlayer::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 50.f, 0.1f, 2.f };
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_pSphereColl->Set_Scale(0.01f);
	m_pSphereColl->Get_CollPos() = { 50.f, 0.1f, 2.f };
	Animate_FSM(110);

	return S_OK;
}

HRESULT CNewPlayer::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh = dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Player"));
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

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	////////////////////////////
	return S_OK;
}

HRESULT CNewPlayer::Late_Init()
{
	m_pSword = dynamic_cast<CSword*>
		(ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT)->Get_MapObject(L"Sword").front());

	if (nullptr == m_pSword)
		return E_FAIL;

	return S_OK;
}

_int CNewPlayer::Update_Object(const _double & TimeDelta)
{
	CGameObject::Late_Init();
	CGameObject::Update_Object(TimeDelta);
	//////////////////////// ▼최우선 함수
	switch (m_iCurAniState)
	{
	default:
		m_Delay = 0.0;
		break;
	}

	MouseFunc();
	//////////////////////// ▼조건 함수
	if (!m_bDash && m_pTransform->bCamTarget )
		m_bAnimate = Key_Check_Func(TimeDelta);

	//if (!m_bAttack[0] && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
	//{
	//	Animate_FSM(m_iAniSet[0]);
	//	m_bAttack[0] = TRUE;
	//	m_pSword->Set_bAttack(TRUE);
	//}

	if (!m_bAnimate && !m_bDash && !m_bAttack[0])
		Animate_FSM(_IDLE);

	if (m_bDash && m_iCurAniState > 101 && m_iCurAniState < 106)
	{
		if (m_pMesh->Is_AnimationSetEnd())
		{
			m_bAnimate = FALSE;
			m_bDash = FALSE;
			m_TimeAccel = 1;
			m_vDashDir = {0.f, 0.f, 0.f};
			m_pSphereColl->Set_Invisible(FALSE);
		}
	}

	if (m_bJump)
		Jump_Func(TimeDelta);

	if (m_bDash)
		Dash_Func(TimeDelta);

	if (m_bAttack[0])
		Attack_Func(TimeDelta);

	//////////////////////// ▼상시 함수
	m_pMesh->Play_AnimationSet(TimeDelta * m_TimeAccel);
	

	//////////////////////// ▼최후미 함수
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CNewPlayer::Late_Update_Object()
{
	CGameObject::Late_Update_Object();
	//////////////////////

}

void CNewPlayer::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();

	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);

	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"Player HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

void CNewPlayer::Render_Set()
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

void CNewPlayer::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

_vec3 CNewPlayer::MouseFunc()
{
	POINT pt = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	return _vec3(_float(pt.x), _float(pt.y), 0.f);
}

_bool CNewPlayer::Key_Check_Func(const _double & TimeDelta)
{
	//MouseFunc();

	_long dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
		m_pTransform->m_vAngle.y += dwMouseMove * _ANGLE *TimeDelta;

	//if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	//	m_pTransform->m_vAngle.x += dwMouseMove * _ANGLE *TimeDelta;
	
	_vec3 vNewDir = m_pTransform->Get_vLookDir();
	_vec3 vPos = m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);

	if (m_pTransform->bCamTarget && ENGINE::Key_Down(ENGINE::dwKEY_F4))
	{
		if (m_pTransform->bCamTarget)
			m_pTransform->bCamTarget = false;
	}


	if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash)
		{
			_vec3 vLeft = vNewDir.NewDir(&vNewDir, &_vec3(0.f, 1.f, 0.f));
			m_vDashDir = -vLeft;
			m_pTransform->m_vInfo[ENGINE::INFO_POS] -= vLeft * _SPEED * TimeDelta;
			m_pSphereColl->Set_Invisible(TRUE);
			Animate_FSM(102);
			m_bDash = TRUE;
			m_TimeAccel = 2;

			for (int i = 0; i < 5; ++i)
				m_bAttack[i] = FALSE;

			return TRUE;
		}
		//else
		//	m_pTransform->m_vAngle.y -= _ANGLE * TimeDelta;
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash)
		{
			_vec3 vRight = vNewDir.NewDir(&vNewDir, &_vec3(0.f, 1.f, 0.f));
			m_vDashDir = vRight;
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vRight * _SPEED * TimeDelta;
			m_pSphereColl->Set_Invisible(TRUE);
			Animate_FSM(103);
			m_TimeAccel = 2;
			m_bDash = TRUE;

			for (int i = 0; i < 5; ++i)
				m_bAttack[i] = FALSE;

			return TRUE;
		}
		//else
		//	m_pTransform->m_vAngle.y += _ANGLE * TimeDelta;
	}

	if (!m_bAttack[0] && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
	{
		//CGameObject* pObject = CEffect_Tex::Create(m_pGraphicDev, m_pTransform->m_vInfo[ENGINE::INFO_POS], 1.f, 0.5f);
		//ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"kaboom", pObject);
		//m_pMesh->Set_AnimationSet(90);
		Animate_FSM(m_iAniSet[0]);
		m_bAttack[0] = TRUE;
		m_pSword->Set_bAttack(TRUE);
	}


	if (!m_bAttack[0] && ENGINE::Key_Press(ENGINE::dwKEY_W))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash)
		{
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			m_vDashDir = -vNewDir;
			m_pTransform->m_vInfo[ENGINE::INFO_POS] -= vNewDir * _SPEED * TimeDelta;
			m_pSphereColl->Set_Invisible(TRUE);
			//Animate_FSM(105);
			m_pMesh->Set_QuickSet(105);
			m_iCurAniState = 105;
			m_iPreAniState = m_iCurAniState;
			m_bDash = TRUE;
			m_TimeAccel = 2;
			return TRUE;
		}
		else if(!m_bDash)
		{
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			m_pTransform->m_vInfo[ENGINE::INFO_POS] -= vNewDir * _SPEED * TimeDelta;
			Animate_FSM(107);
			return TRUE;
		}
	}

	if (!m_bAttack[0] && ENGINE::Key_Press(ENGINE::dwKEY_S))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash)
		{
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			m_vDashDir = vNewDir;
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vNewDir * _SPEED * TimeDelta;
			m_pSphereColl->Set_Invisible(TRUE);
			//Animate_FSM(104);
			m_pMesh->Set_QuickSet(104);
			m_iCurAniState = 104;
			m_iPreAniState = m_iCurAniState;
			m_bDash = TRUE;
			m_TimeAccel = 2;
			return TRUE;
		}
		else if (!m_bDash)
		{
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += vNewDir * 1.5f * TimeDelta;
			Animate_FSM(106);
			return TRUE;
		}
	}

	if (ENGINE::Key_Down(ENGINE::dwKEY_SPACE) && !m_bAnimate)
	{
		m_bJump = true;
		Animate_FSM(101);
	}

	return FALSE;
}


VOID CNewPlayer::Animate_FSM(_uint iAniState)
{
	m_iCurAniState = iAniState;

	if (/*!m_bAnimate && */m_iCurAniState != m_iPreAniState)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniState);
		m_iPreAniState = m_iCurAniState;
	}
}

VOID CNewPlayer::Animate_Quick(_uint iAniState)
{
	m_iCurAniState = iAniState;

	if (/*!m_bAnimate && */m_iCurAniState != m_iPreAniState)
	{
		m_pMesh->Set_QuickSet(m_iCurAniState);
		m_iPreAniState = m_iCurAniState;
	}
}

void CNewPlayer::Jump_Func(const _double & TimeDelta)
{
	_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	//y=-a*x+b에서 (a: 중력가속도, b: 초기 점프속도)
	//적분하여 y = (-a/2)*x*x + (b*x) 공식을 얻는다.(x: 점프시간, y: 오브젝트의 높이)
	//변화된 높이 height를 기존 높이 _posY에 더한다.
	float height = (m_JumpTime * m_JumpTime * (-_GRAVITY) / 2) + (m_JumpTime * _JUMPPOWER);
	//_transform.position = new Vector3(_transform.position.x, _posY + height, _transform.position.z);
	m_pTransform->m_fJump = height;
	//점프시간을 증가시킨다.
	m_JumpTime += TimeDelta;

	//처음의 높이 보다 더 내려 갔을때 => 점프전 상태로 복귀한다.
	if (m_pTransform->m_vInfo[ENGINE::INFO_POS].y < 0.1f)
	{
		m_bJump = FALSE;
		m_JumpTime = 0.0f;
		m_pTransform->m_fJump = 0.f;
		m_pTransform->m_vInfo[ENGINE::INFO_POS].y = 0.1f;
		m_pMesh->Set_AnimationSet(99);
	}
}

void CNewPlayer::Dash_Func(const _double & TimeDelta)
{
	m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_vDashDir * 6.f * TimeDelta;
}

void CNewPlayer::Attack_Func(const _double & TimeDelta)
{
	_uint iMax = 6;
	if (m_bAttack[0] && !m_bAttack[1])
	{
		if (m_iCurAniState == _uint(m_iAniSet[0] - 1) && m_pMesh->Is_AnimationSetEnd())
		{
			for (int i = 0; i < iMax; ++i)
				m_bAttack[i] = FALSE;
			Animate_FSM(_IDLE);
		}
		else if (m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_uint(m_iAniSet[0] - 1));
		//{
		//	m_pMesh->Set_QuickSet(_uint(m_iAniSet[0] - 1));
		//	m_iCurAniState = _uint(m_iAniSet[0] - 1);
		//	m_iPreAniState = m_iCurAniState;
		//}
		else if (m_bAttack[0] && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			Animate_FSM(m_iAniSet[1]);
			m_bAttack[1] = TRUE;
			m_pSword->Set_bAttack(TRUE);
		}

		return;
	}

	if (m_bAttack[0] && m_bAttack[1] && !m_bAttack[2])
	{
		if (m_iCurAniState == _uint(m_iAniSet[1] - 1) && m_pMesh->Is_AnimationSetEnd())
		{
			for (int i = 0; i < iMax; ++i)
				m_bAttack[i] = FALSE;
			Animate_FSM(_IDLE);
		}
		else if (m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_uint(m_iAniSet[1] - 1));
		else if (m_bAttack[1] && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			Animate_FSM(m_iAniSet[2]);
			m_bAttack[2] = TRUE;
			m_pSword->Set_bAttack(TRUE);
		}
		return;
	}

	if (m_bAttack[0] && m_bAttack[1] && m_bAttack[2] && !m_bAttack[3])
	{
		if (m_iCurAniState == _uint(m_iAniSet[2] - 1) && m_pMesh->Is_AnimationSetEnd())
		{
			for (int i = 0; i < iMax; ++i)
				m_bAttack[i] = FALSE;
			Animate_FSM(_IDLE);
		}
		else if (m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_uint(m_iAniSet[2] - 1));
		else if (ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON) && m_bAttack[2])
		{
			Animate_FSM(m_iAniSet[3]);
			m_bAttack[3] = TRUE;
			m_pSword->Set_bAttack(TRUE);
		}
		return;
	}

	if (m_bAttack[0] && m_bAttack[1] && m_bAttack[2] && m_bAttack[3] && !m_bAttack[4])
	{
		if (m_iCurAniState == _uint(m_iAniSet[3] - 1) && m_pMesh->Is_AnimationSetEnd())
		{
			for (int i = 0; i < iMax; ++i)
				m_bAttack[i] = FALSE;
			Animate_FSM(_IDLE);
		}
		else if (m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_uint(m_iAniSet[3] - 1));
		else if (ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON) && m_bAttack[3])
		{
			Animate_FSM(m_iAniSet[4]);
			m_bAttack[4] = TRUE;
			m_pSword->Set_bAttack(TRUE);
		}

		return;
	}

	if (m_bAttack[0] && m_bAttack[1] && m_bAttack[2] && m_bAttack[3] && m_bAttack[4] && !m_bAttack[5])
	{
		if (m_iCurAniState == _uint(m_iAniSet[4] - 1) && m_pMesh->Is_AnimationSetEnd())
		{
			for (int i = 0; i < iMax; ++i)
				m_bAttack[i] = FALSE;
			Animate_FSM(_IDLE);
		}
		else if (m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_uint(m_iAniSet[4] - 1));
		else if (ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON) && m_bAttack[4])
		{
			Animate_FSM(m_iAniSet[5]);
			m_bAttack[5] = TRUE;
			m_pSword->Set_bAttack(TRUE);
		}

		return;
	}

	if (m_bAttack[5])
	{
		if (m_iCurAniState == _uint(m_iAniSet[5] - 1) && m_pMesh->Is_AnimationSetEnd())
		{
			for (int i = 0; i < iMax; ++i)
				m_bAttack[i] = FALSE;
			Animate_FSM(_IDLE);
		}

		if (m_pMesh->Is_AnimationSetEnd())
			Animate_FSM(_uint(m_iAniSet[5] - 1));
	}


}

CNewPlayer * CNewPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNewPlayer* pInstance = new CNewPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CNewPlayer::Free()
{
	CGameObject::Free();
}
