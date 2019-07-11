#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

#define SPEED 5.f
#define ANGLE 60.f

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	:	CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMesh->Set_AnimationSet(57);

	m_pNaviMesh->Set_CurrentIdx(0);

	m_pTransform->m_vAngle.y = 0.f;

	return S_OK;
}

HRESULT CPlayer::Late_Init()
{
	m_pTransform->m_vScale = { 0.01f, 0.01f, 0.01f };
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.1f, 0.1f, 0.1f };

	return S_OK;
}

_int CPlayer::Update_Object(const _float & fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	if (m_pTransform->bCamTarget)
	{
		if (Key_check(fTimeDelta))
			m_pMesh->Set_AnimationSet(0);
	}
		//Key_Old(fTimeDelta);

	m_pMesh->Play_AnimationSet(fTimeDelta);

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CPlayer::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CPlayer::Render_Object()
{
	//Render_Set();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pNaviMesh->Render_NaviMesh();

	m_pMesh->Render_Meshes();

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

bool CPlayer::Key_check(_float fTimeDelta)
{
	_vec3 vNewDir = m_pTransform->m_vInfo[ENGINE::INFO_LOOK];
	_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	vNewDir.y = 0.f;

	POINT pt = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	_long dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
		m_pTransform->m_vAngle.y += dwMouseMove * ANGLE *fTimeDelta;

	//if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	//	m_pTransform->m_vAngle.x += dwMouseMove * ANGLE *fTimeDelta;

	if (m_pTransform->bCamTarget && ENGINE::Key_Down(ENGINE::dwKEY_F4))
	{
		if(m_pTransform->bCamTarget)
			m_pTransform->bCamTarget = false;
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_W))
	{
		D3DXVec3Normalize(&vNewDir, &vNewDir);
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vNewDir * fTimeDelta * -SPEED));
			
		m_pMesh->Set_AnimationSet(54);
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_S))
	{
		D3DXVec3Normalize(&vNewDir, &vNewDir);
		m_pTransform->m_vInfo[ENGINE::INFO_POS] = m_pNaviMesh->MoveOn_NaviMesh(&vPos, &(vNewDir * fTimeDelta * SPEED));

		m_pMesh->Set_AnimationSet(54);
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_A))
		m_pTransform->m_vAngle.y -= ANGLE *fTimeDelta;

	if (ENGINE::Key_Press(ENGINE::dwKEY_D))
		m_pTransform->m_vAngle.y += ANGLE *fTimeDelta;

	if (ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
	{
		CGameObject* pObject = CEffect_Tex::Create(m_pGraphicDev, vPos, 1.f, 0.5f);
		ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"kaboom", pObject);
		m_pMesh->Set_AnimationSet(30);
	}

	//if (m_pMesh->Is_AnimationSetEnd())
	//	m_pMesh->Set_AnimationSet(57);

	return m_pMesh->Is_AnimationSetEnd();

}

void CPlayer::Key_Old(_float fTimeDelta)
{
	_vec3 vNewDir = {};
	//Fix Mouse
	POINT pt = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	_long dwMouseMove = 0;
	if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
		m_pTransform->m_vAngle.y += dwMouseMove * ANGLE * fTimeDelta;

	//if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	//	m_pTransform->m_vAngle.x += dwMouseMove * ANGLE * fTimeDelta;

	if (m_pTransform->bCamTarget && ENGINE::Key_Down(ENGINE::dwKEY_F4))
	{
		if (m_pTransform->bCamTarget)
			m_pTransform->bCamTarget = false;
	}

	//Key Input
	if (ENGINE::Key_Press(ENGINE::dwKEY_W))
	{
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_pTransform->m_vDir * SPEED * fTimeDelta;
		m_pMesh->Set_AnimationSet(54);
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_S))
	{
		m_pTransform->m_vInfo[ENGINE::INFO_POS] -= m_pTransform->m_vDir * SPEED * fTimeDelta;
		m_pMesh->Set_AnimationSet(54);
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		m_pTransform->m_vInfo[ENGINE::INFO_POS] +=
			vNewDir.NewDir(&m_pTransform->m_vDir, &_vec3(0.f, 1.f, 0.f)) * SPEED * fTimeDelta;
		m_pMesh->Set_AnimationSet(54);
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		m_pTransform->m_vInfo[ENGINE::INFO_POS] -=
			vNewDir.NewDir(&m_pTransform->m_vDir, &_vec3(0.f, 1.f, 0.f)) * SPEED * fTimeDelta;
		m_pMesh->Set_AnimationSet(54);
	}

	if (m_pMesh->Is_AnimationSetEnd())
		m_pMesh->Set_AnimationSet(57);

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

	pComponent = m_pMesh =dynamic_cast<ENGINE::CDynamicMesh*>
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
