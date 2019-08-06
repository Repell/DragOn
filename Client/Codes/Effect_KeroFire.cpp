#include "stdafx.h"
#include "Effect_KeroFire.h"
#include "Export_Utility.h"

CEffect_Kerofire::CEffect_Kerofire(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_Frame(0.f), m_MaxFrame(32.f), m_LifeTime(0.f), m_Time(0.f), m_bMove(FALSE)
{
}

CEffect_Kerofire::~CEffect_Kerofire()
{
}

void CEffect_Kerofire::Set_Move(_bool bMove, _vec3 vDir)
{
	m_bMove = bMove;
	m_vFireDir = vDir;
}

void CEffect_Kerofire::Set_Pos(_vec3 vPos)
{
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vInfo[ENGINE::INFO_POS].y += 1.f;
}

void CEffect_Kerofire::Set_Scale(_vec3 vScale)
{
	m_pTransform->m_vScale = vScale;
}

void CEffect_Kerofire::Set_LifeTime(_double fLife, _double fDelay)
{
	m_LifeTime = fLife;
	m_Time -= fDelay;
}

HRESULT CEffect_Kerofire::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };

	return S_OK;
}

HRESULT CEffect_Kerofire::Late_Init()
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	if (pLayer->Get_MapObject(L"Player").empty())
		return E_FAIL;

	list<CGameObject*> pList = pLayer->Get_MapObject(L"Player");

	ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
		(pList.front()->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));
	NULL_CHECK_RETURN(pTrans, E_FAIL);

	ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
		(pList.front()->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));
	NULL_CHECK_RETURN(pSphere, E_FAIL);

	m_pTarget = pTrans;
	m_pTargetSphere = pSphere;

	return S_OK;
}



_int CEffect_Kerofire::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	
	m_Time += TimeDelta;
	
	if (m_Time > 0.0)
		m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_vFireDir * 300.f * TimeDelta;

	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_Time > m_LifeTime)
		return END_EVENT;

	Update_Frame(TimeDelta);
	Update_Billboard();

	Compute_ViewZ(&m_pTransform->m_vInfo[ENGINE::INFO_POS]);
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_ALPHA, this);

	return NO_EVENT;
}

void CEffect_Kerofire::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CEffect_Kerofire::Render_Object()
{
	//Render_Set();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	if (m_Time >= 0.f)		//Delay Render
	{
		m_pTexture->Render_Texture(m_Frame);
		m_pBuffer->Render_Buffer();
	}

	//Render_ReSet();
}

HRESULT CEffect_Kerofire::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	//Buffer Component
	pComponent = m_pBuffer = dynamic_cast<ENGINE::CRcTex*>
		(ENGINE::Clone_Resources(RESOURCE_PRELOAD, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Buffer", pComponent);

	//Texture Componet
	pComponent = m_pTexture = dynamic_cast<ENGINE::CTexture*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Texture_Fireball"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Texture", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Transform", pComponent);

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	////////////////////////////
	return S_OK;
}

void CEffect_Kerofire::Update_Frame(const _double& TimeDelta)
{
	if (m_Time > 0.0)
		m_Frame += m_MaxFrame * TimeDelta;

	if (m_Frame > m_MaxFrame)
		m_Frame = 0.0;
}

void CEffect_Kerofire::Update_Billboard()
{
	///////////////////////////////////////
	_matrix		matView, matBill, matWorld;

	matWorld = m_pTransform->m_matWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransform->m_matWorld = matBill * matWorld;
	///////////////////////////////////////
}

void CEffect_Kerofire::Render_Set()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Alpha Test Begin
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Alpha Blend
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CEffect_Kerofire::Render_ReSet()
{
	//Alpha Test End
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//Alpha Blend End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CEffect_Kerofire * CEffect_Kerofire::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect_Kerofire* pInstance = new CEffect_Kerofire(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

CEffect_Kerofire * CEffect_Kerofire::Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vPos, _vec3 vScale, _double fLife, _double fDelay)
{
	CEffect_Kerofire* pInstance = new CEffect_Kerofire(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	pInstance->Set_Pos(vPos);
	pInstance->Set_LifeTime(fLife, fDelay);
	pInstance->Set_Scale(vScale);

	return pInstance;
}

void CEffect_Kerofire::Free()
{
	ENGINE::CGameObject::Free();
}
