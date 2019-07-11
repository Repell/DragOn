#include "stdafx.h"
#include "DynamicCamera.h"

#define SPEED 20.f
#define ANGLE 90.f

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDev)
	:ENGINE::CCamera(pDev), m_pTransCom(nullptr),
	m_fTargetDist(0.f), m_fCamAngle(0.f), m_fCamSpeed(0.f), m_fTargetDistMax(10.f), bSpectre(TRUE)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_Object()
{
	Add_Component();

	

	m_fTargetDist = 10.f;
	m_fCamAngle = 45.f;
	m_fCamSpeed = 1.f;

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

HRESULT CDynamicCamera::Late_Init()
{
	//ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);
	//m_pCamTarget = dynamic_cast<ENGINE::CTransform*>(pLayer->Get_Component(L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));

	//if (m_pCamTarget == nullptr)
	//	return E_FAIL;

	m_pSprite = ENGINE::Get_GraphicDev()->Get_Sprite();
	m_pSprite->AddRef();
	m_pLine = ENGINE::Get_GraphicDev()->Get_Line();
	m_pLine->AddRef();
	m_pLine->SetAntialias(TRUE);

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _float & fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	Key_Input(fTimeDelta);
	Key_Spectre(fTimeDelta);

	Render_Line();

	return NO_EVENT;
}

void CDynamicCamera::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	//if (bSpectre)
	Target_Spectre();
	//else
	//	Target_Renewal();
}

void CDynamicCamera::Render_Object()
{
	Render_Set();



	Render_ReSet();
	//Render_Font(&_vec2(10.f, 10.f));

	if(CValueMgr::bAxisLine)
		Render_Line();
}

HRESULT CDynamicCamera::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	
	//Transform
	pComponent = m_pTransCom = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CDynamicCamera::Reset_Camera()
{
	m_fTargetDist = 10.f;
	m_fCamAngle = 45.f;
	m_fCamSpeed = 1.f;

	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };
	m_pTransCom->m_vAngle = { 0.f, 0.f, 0.f };

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	if (ENGINE::Key_Down(ENGINE::dwKEY_F1))
		Reset_Camera();

	_long dwMouseMove = 0;
	if (dwMouseMove < ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Z))
		m_fTargetDist -= m_fCamSpeed;
	if (dwMouseMove > ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Z))
		m_fTargetDist += m_fCamSpeed;

	if (m_fTargetDist < 2.5f)
		m_fTargetDist = 3.f;

	if (ENGINE::Key_Press(ENGINE::dwKEY_Q))
		m_fCamAngle += ANGLE * fTimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_E))
		m_fCamAngle -= ANGLE * fTimeDelta;

	if (m_fCamAngle > 90.f)
		m_fCamAngle = 90.f;
	else if (m_fCamAngle < -90.f)
		m_fCamAngle = -90.f;

}

void CDynamicCamera::Key_Spectre(const _float & fTimeDelta)
{
	//_vec3 vNewDir = {};
	////Fix Mouse
	//POINT pt = { WINCX >> 1, WINCY >> 1 };
	//ClientToScreen(g_hWnd, &pt);
	//SetCursorPos(pt.x, pt.y);

	//_long dwMouseMove = 0;
	//if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_X))
	//	m_pTransCom->m_vAngle.y += dwMouseMove * ANGLE * fTimeDelta;

	//if (dwMouseMove = ENGINE::Get_DIMouseMove(ENGINE::CInputDev::DIMS_Y))
	//	m_pTransCom->m_vAngle.x += dwMouseMove * ANGLE * fTimeDelta;

	_vec3 vNewDir;
	POINT ptMouse = {};
	//POINT pt = { LONG(WINCX * 0.5f), LONG(WINCY * 0.5f) };
	GetCursorPos(&ptMouse);
	ClientToScreen(g_hWnd, &ptMouse);

	if (ptMouse.x > 1850)
		m_pTransCom->m_vAngle.y += 90.f * fTimeDelta;
	else if (ptMouse.x < 70)
		m_pTransCom->m_vAngle.y -= 90.f * fTimeDelta;
	if (ptMouse.y > 1080)
		m_pTransCom->m_vAngle.x += 90.f * fTimeDelta;
	else if (ptMouse.y < 80)
		m_pTransCom->m_vAngle.x -= 90.f * fTimeDelta;


	//Key Input
	if (ENGINE::Key_Press(ENGINE::dwKEY_W))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] += m_pTransCom->m_vDir * SPEED * fTimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_S))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] -= m_pTransCom->m_vDir * SPEED * fTimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_A))
	{
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] +=
			vNewDir.NewDir(&m_pTransCom->m_vDir, &_vec3(0.f, 1.f, 0.f)) * SPEED * fTimeDelta;
	}
	if (ENGINE::Key_Press(ENGINE::dwKEY_D))
	{
		m_pTransCom->m_vInfo[ENGINE::INFO_POS] -=
			vNewDir.NewDir(&m_pTransCom->m_vDir, &_vec3(0.f, 1.f, 0.f)) * SPEED * fTimeDelta;
	}

	if (ENGINE::Key_Press(ENGINE::dwKEY_SPACE))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS].y += SPEED * fTimeDelta;
	if (ENGINE::Key_Press(ENGINE::dwKEY_Shift))
		m_pTransCom->m_vInfo[ENGINE::INFO_POS].y -= SPEED * fTimeDelta;


}

//void CDynamicCamera::Target_Renewal()
//{
//	_vec3 vEye = vEye.Reverse(&m_pCamTarget->m_vDir);
//
//	//카메라는 플레이어의 역방향
//	vEye *= m_fTargetDist;
//
//	//높이는 카메라의 각도의 따라 조절될 것
//	_vec3 vRight = {};
//	memcpy(&vRight, m_pCamTarget->m_matWorld.m[0], sizeof(_vec3));
//
//	//right 벡터 기준으로 축 회전
//	D3DXMATRIX matRotAxis;
//
//	//D3DXMatrixRotationAxis: 임의의 축회전
//	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_fCamAngle));
//	D3DXVec3TransformNormal(&vEye, &vEye, &matRotAxis);
//
//	//카메라의 위치
//	vEye += m_pCamTarget->m_vInfo[ENGINE::INFO_POS];
//	m_vEye = vEye;
//
//	_vec3 vAt = m_pCamTarget->m_vInfo[ENGINE::INFO_POS];
//	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = vAt;		//Spectre Cam StartPos
//	vAt.y += 1.25f;
//
//	if (m_pCamTarget->m_vAngle.x > 44.f || m_pCamTarget->m_vAngle.x < -135.f)
//		ENGINE::CCamera::Make_ViewMatrix(&vEye, &vAt, &_vec3(0.f, -1.f, 0.f));
//	else
//		ENGINE::CCamera::Make_ViewMatrix(&vEye, &vAt, &_vec3(0.f, 1.f, 0.f));
//}

void CDynamicCamera::Target_Spectre()
{
	_vec3 vEye = vEye.Reverse(&m_pTransCom->m_vDir);
	D3DXVec3Normalize(&vEye, &vEye);

	//카메라는 플레이어의 역방향
	vEye *= m_fTargetDist;

	//높이는 카메라의 각도의 따라 조절될 것
	_vec3 vRight = {};
	memcpy(&vRight, m_pTransCom->m_matWorld.m[0], sizeof(_vec3));

	//right 벡터 기준으로 축 회전
	D3DXMATRIX matRotAxis;

	//D3DXMatrixRotationAxis: 임의의 축회전
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_fCamAngle));
	D3DXVec3TransformNormal(&vEye, &vEye, &matRotAxis);

	//카메라의 위치
	vEye += m_pTransCom->m_vInfo[ENGINE::INFO_POS];
	m_vEye = vEye;

	_vec3 vAt = m_pTransCom->m_vInfo[ENGINE::INFO_POS];
	vAt.y += 1.25f;

	ENGINE::CCamera::Make_ViewMatrix(&vEye, &vAt, &_vec3(0.f, 1.f, 0.f));
	CValueMgr::matView = m_matView;
}

void CDynamicCamera::Render_Set()
{
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	if (CValueMgr::bWireFrm_Obj)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CDynamicCamera::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CDynamicCamera::Render_Font(const _vec2* pPos)
{
	//RECT rc = { _long(pPos->x), _long(pPos->y) };
	////Fonts
	_tchar szStr[MAX_PATH] = L"";
	POINT pt = {};
	GetCursorPos(&pt);
	ClientToScreen(g_hWnd, &pt);
	////_vec3 vPos = m_pTransCom->m_vInfo[ENGINE::INFO_POS];
	////_float fAngleX = m_pTransCom->m_vAngle.x;
	////_float fAngleY = m_pTransCom->m_vAngle.y;
	//m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	////Render Text
	////swprintf_s(szIndex, L"%5.2f , %5.2f , %5.2f", vPos.x, vPos.y, vPos.z);
	////swprintf_s(szStr, L"%5.2f , %5.2f", fAngleX, fAngleY);
	swprintf_s(szStr, L"%d , %d", pt.x, pt.y);

	//m_pFont->DrawTextW(m_pSprite,
	//	szStr, lstrlen(szStr), &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

	//m_pSprite->End();
	ENGINE::Render_Font(L"Sp", szStr, pPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

void CDynamicCamera::Render_Line()
{
	_matrix matLine;
	D3DXMatrixIdentity(&matLine);
	matLine = m_matView * m_matProj;

	//ENGINE::LINE_3D vLine(m_pTransCom->m_vInfo[ENGINE::INFO_POS], 10.f);
	ENGINE::LINE_3D vLine(_vec3(0.f, 0.f,0.f), 10.f);

	m_pLine->SetWidth(5.f);
	m_pLine->Begin();

	m_pLine->DrawTransform(vLine.vLine_X, 2, &matLine, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_pLine->DrawTransform(vLine.vLine_Y, 2, &matLine, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	m_pLine->DrawTransform(vLine.vLine_Z, 2, &matLine, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	m_pLine->End();
}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		ENGINE::Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Make_ProjectionMatrix(D3DXToRadian(60.f), WINCX / float(WINCY), 0.1f, 1000.f);
	CValueMgr::matProj = pInstance->m_matProj;

	return pInstance;
}

void CDynamicCamera::Free()
{
	ENGINE::Safe_Release(m_pSprite);
	ENGINE::Safe_Release(m_pLine);

	ENGINE::CGameObject::Free();
}
