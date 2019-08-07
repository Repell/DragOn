#include "stdafx.h"
#include "Player_Rider.h"

#include "Export_Function.h"

#define _SPEED 4.f
#define _ANGLE 60.f
#define  _RADIUS 75.f

CPlayer_Rider::CPlayer_Rider(LPDIRECT3DDEVICE9 pDevice)
	:	CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr), m_pNaviMesh(nullptr),
	m_pParentBoneMatrix(nullptr), m_pParentWorldMatrix(nullptr)
{
	m_bAi = FALSE;
}

CPlayer_Rider::~CPlayer_Rider()
{
}

HRESULT CPlayer_Rider::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pMesh->Set_AnimationSet(1);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.1f, 0.f };
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vAngle.x = 90.f;
	m_pSphereColl->Set_Scale(0.01f);

	return S_OK;
}

HRESULT CPlayer_Rider::Late_Init()
{
	//m_pSpear = dynamic_cast<CSpear*>
	//	(ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT)->Get_MapObject(L"Spear").front());

	//if (nullptr == m_pSpear)
	//	return E_FAIL;

	return S_OK;
}

_int CPlayer_Rider::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	Get_ParentMatrix();
	ENGINE::CGameObject::Update_Object(TimeDelta);
		
	m_pMesh->Is_AnimationSetEnd();
	m_pMesh->Play_AnimationSet(TimeDelta);
		
	m_pTransform->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));	//공전 * 부모
	m_pTransform->m_matWorld.m[3][1] += 0.5f;
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CPlayer_Rider::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();


}

void CPlayer_Rider::Render_Object()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	if(!m_bAi)
		m_pMesh->Render_Meshes();

	m_pTransform->m_matWorld.m[3][1] -= 0.5f;
	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);

	//_tchar szStr[MAX_PATH] = L"";
	//swprintf_s(szStr, L"Player HP: %d", m_pSphereColl->Get_iHp(0));
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 70.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//Render_ReSet();
}

void CPlayer_Rider::UI_Sample()
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

void CPlayer_Rider::Get_ParentMatrix()
{
	ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player_Dragon", L"Com_Transform", ENGINE::COMP_DYNAMIC));

	m_bAi = pTrans->Check_Ai();

	if (nullptr == m_pParentBoneMatrix)
	{
		ENGINE::CDynamicMesh* pPlayerMesh = dynamic_cast<ENGINE::CDynamicMesh*>
			(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player_Dragon", L"Com_Mesh", ENGINE::COMP_DYNAMIC));

		if (nullptr == pPlayerMesh)
			return;

		const ENGINE::D3DXFRAME_DERIVED* pFrame = pPlayerMesh->Get_FrameByName("BODY2");

		m_pParentBoneMatrix = &pFrame->combinedTransformMatrix;


		if (nullptr == pTrans)
			return;

		m_pParentWorldMatrix = &(pTrans->m_matWorld);
	}
}

HRESULT CPlayer_Rider::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh =dynamic_cast<ENGINE::CDynamicMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Rider"));
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
	//pComponent = m_pNaviMesh = dynamic_cast<ENGINE::CNaviMesh*>
	//	(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Navi"));
	//m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_NaviMesh", pComponent);
	
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

CPlayer_Rider * CPlayer_Rider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Rider* pInstance = new CPlayer_Rider(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Rider::Free()
{
	ENGINE::CGameObject::Free();
}
