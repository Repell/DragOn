#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

#define _SPEED 4.f
#define _ANGLE 60.f
#define  _RADIUS 75.f
#define _GRAVITY 2.8f
#define _JUMPPOWER 0.75f


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
	bJump = FALSE;
	m_bAnimate = FALSE;
	m_bDash = FALSE;
	JumpTime = 0.0;
	m_DashDelay = 0.0;
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pMesh->Set_AnimationSet(109);

	m_pNaviMesh->Set_CurrentIdx(3);

	m_pTransform->m_vAngle.y = 0.f;
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 50.f, 0.1f, 2.f };
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_pSphereColl->Set_Scale(0.01f);
	m_pSphereColl->Get_CollPos() = { 50.f, 0.1f, 2.f };

	return S_OK;
}

HRESULT CPlayer::Late_Init()
{
	m_pSword = dynamic_cast<CSword*>
		(ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT)->Get_MapObject(L"Sword").front());

	if (nullptr == m_pSword)
		return E_FAIL;

	return S_OK;
}

_int CPlayer::Update_Object(const _double& TimeDelta)
{
	if (!m_bDash)
		m_DashDelay += TimeDelta;

	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_pTransform->bCamTarget)
	{
		if (Key_check(TimeDelta))
			Animate_FSM(109);
	}
	if (bJump)
		Jump_Func(TimeDelta);

	m_pMesh->Play_AnimationSet(TimeDelta);
	
	if(m_bAnimate = m_pMesh->Is_AnimationSetEnd());
	{
		m_bDash = m_bAnimate;
	}

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CPlayer::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	if (ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
	{
		CGameObject* pObject = CEffect_Tex::Create(m_pGraphicDev, m_pTransform->m_vInfo[ENGINE::INFO_POS], 1.f, 0.5f);
		ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"kaboom", pObject);
		m_pMesh->Set_AnimationSet(90);

		m_pSword->Set_AttackState(TRUE, m_iCurAniState);
	}

}

void CPlayer::Render_Object()
{
	//Render_Set();

	//Check_EnemyColl();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();

	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);

	_tchar szStr[MAX_PATH] = L"";
	swprintf_s(szStr, L"Player HP: %d", m_pSphereColl->Get_iHp(0));
	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CPlayer::Render_Set()
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

void CPlayer::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

_bool CPlayer::Key_check(const _double& TimeDelta)
{
	_vec3 vNewDir = m_pTransform->m_vInfo[ENGINE::INFO_LOOK];
	_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	vNewDir.y = 0.f;

	POINT pt = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	_long dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
		m_pTransform->m_vAngle.y += dwMouseMove * _ANGLE *TimeDelta;

	//if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	//	m_pTransform->m_vAngle.x += dwMouseMove * ANGLE *TimeDelta;

	if (m_pTransform->bCamTarget && ENGINE::Key_Down(ENGINE::dwKEY_F4))
	{
		if (m_pTransform->bCamTarget)
			m_pTransform->bCamTarget = false;
	}


	if (ENGINE::Key_Press(ENGINE::dwKEY_W))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash && m_DashDelay > 1.5)
		{
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vNewDir * -500.f));
			Animate_FSM(105);
			m_bAnimate = TRUE;
			m_bDash = TRUE;
		}
		else
		{
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vNewDir * TimeDelta * -_SPEED));
			Animate_FSM(106);
		}

	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_S))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash  && m_DashDelay > 1.5)
		{
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vNewDir * 500.f));
			Animate_FSM(104);
			m_bAnimate = TRUE;
			m_bDash = TRUE;
		}
		else
		{
			D3DXVec3Normalize(&vNewDir, &vNewDir);
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vNewDir * TimeDelta * _SPEED));
			Animate_FSM(106);
		}
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash  && m_DashDelay > 1.5)
		{
			_vec3 vLeft = vNewDir.NewDir(&m_pTransform->m_vDir, &_vec3(0.f, 1.f, 0.f));
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vLeft * -5.f));
			Animate_FSM(102);
			m_bAnimate = TRUE;
			m_bDash = TRUE;
		}
		else
		{
			_vec3 vLeft = vNewDir.NewDir(&m_pTransform->m_vDir, &_vec3(0.f, 1.f, 0.f));
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(-vLeft * TimeDelta * _SPEED));
		}
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_Shift) && !m_bDash && m_DashDelay > 1.5)
		{
			_vec3 vRight = vNewDir.NewDir(&m_pTransform->m_vDir, &_vec3(0.f, 1.f, 0.f));
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vRight * 5.f));
			Animate_FSM(103);
			m_bAnimate = TRUE;
			m_bDash = TRUE;
		}
		else
		{
			_vec3 vRight = vNewDir.NewDir(&m_pTransform->m_vDir, &_vec3(0.f, 1.f, 0.f));
			m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pNaviMesh->MoveOn_NaviMesh_Dir(&vPos, &(vRight * TimeDelta * _SPEED));
		}
	}


	if (ENGINE::Key_Down(ENGINE::dwKEY_SPACE) && !m_bAnimate)
	{
		bJump = true;
		m_bAnimate = TRUE;
		Animate_FSM(101);
	}

	//if (m_pMesh->Is_AnimationSetEnd())
	//	m_pMesh->Set_AnimationSet(57);

	return m_pMesh->Is_AnimationSetEnd();
}

//void CPlayer::Jump_Check(const _double& TimeDelta)
//{
//	if (iJumpCount < 25 && bUpDown && bJump)
//	{
//		++iJumpCount;
//		m_pTransform->m_fJump = (iJumpCount * 10) * TimeDelta;
//		if (iJumpCount >= 25)
//			bUpDown = FALSE;
//	}
//	else if (iJumpCount > 0 && !bUpDown && bJump)
//	{
//		m_pTransform->m_fJump = (iJumpCount * 10) * TimeDelta;
//		--iJumpCount;
//		if (iJumpCount == 0)
//		{
//			bJump = FALSE;
//			m_pTransform->m_fJump = 0.f;
//			//_float fY = m_pNaviMesh->MoveOn_Terrain(&m_pTransform->m_vInfo[ENGINE::INFO_POS], &m_pTransform->m_vInfo[ENGINE::INFO_LOOK]);
//			//m_pTransform->m_vInfo[ENGINE::INFO_POS].y = fY;
//		}
//	}
//}

void CPlayer::Jump_Func(const _double& TimeDelta)
{
	_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	//y=-a*x+b에서 (a: 중력가속도, b: 초기 점프속도)
	//적분하여 y = (-a/2)*x*x + (b*x) 공식을 얻는다.(x: 점프시간, y: 오브젝트의 높이)
	//변화된 높이 height를 기존 높이 _posY에 더한다.
	float height = (JumpTime * JumpTime * (-_GRAVITY) / 2) + (JumpTime * _JUMPPOWER);
	//_transform.position = new Vector3(_transform.position.x, _posY + height, _transform.position.z);
	m_pTransform->m_fJump = height;
	//점프시간을 증가시킨다.
	JumpTime += TimeDelta;

	//처음의 높이 보다 더 내려 갔을때 => 점프전 상태로 복귀한다.
	if (m_pTransform->m_vInfo[ENGINE::INFO_POS].y < 0.1f)
	{
		bJump = FALSE;
		JumpTime = 0.0f;
		m_pTransform->m_fJump = 0.f;
		m_pTransform->m_vInfo[ENGINE::INFO_POS].y = 0.1f;
		m_pMesh->Set_AnimationSet(99);
	}

}

_bool CPlayer::Check_EnemyColl()
{
	//const _tchar* pObjTag, const _tchar* pCompTag
	//_tchar szStr[MAX_PATH] = L"충돌했음!";

	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(L"Troll"))
	{
		ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (pSphere == nullptr)
			return FALSE;

		return m_pSphereColl->Check_ComponentColl(pSphere);
	}

}

VOID CPlayer::Animate_FSM(_uint iAniState)
{
	m_iCurAniState = iAniState;

	if (!m_bAnimate && m_iCurAniState != m_iPreAniState)
	{
		m_pMesh->Set_AnimationSet(m_iCurAniState);
		m_iPreAniState = m_iCurAniState;
	}
}

void CPlayer::UI_Sample()
{
	//직교 투영
	_matrix matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);	 // 항등
	D3DXMatrixIdentity(&matView);	// 스케일, 위치
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);		//Near 0.f, Far 1.f;

	float fScale[4];
	fScale[0] = 450.f;	// 텍스처의 X 크기 값
	fScale[1] = 250.f;	// 텍스처의 Y 크기 값
	fScale[2] = 1.f;        // 텍스처의 Z 크기 값(Z는 1.f 고정)
	_vec3 vScale = { 450.f, 250.f, 1.f };

	//크기 조절(Scale)
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
			matView(i, j) *= fScale[i];
	}

	//위치 조절(Pos)
	matView.m[3][1] = -225.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);	//항등
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);		//항등 *= 스케일값, 위치값 적용
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);	//직교행렬 변환

	//이후 기존행렬로 Reset(d3dts)하여 일반 객체가 직교행렬에 영향을 받지않도록 주의	

}

HRESULT CPlayer::Add_Component()
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

	//Collider
	//pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev,
	//	m_pMesh->Get_VtxMeshPos(), m_pMesh->Get_NumVtx(), m_pMesh->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	////////////////////////////
	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	ENGINE::CGameObject::Free();
}
