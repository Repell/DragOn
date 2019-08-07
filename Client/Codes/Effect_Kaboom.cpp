#include "stdafx.h"
#include "Effect_Kaboom.h"
#include "Export_Utility.h"

CEffect_Kaboom::CEffect_Kaboom(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice),
	m_Frame(0.f), m_MaxFrame(90.f), m_LifeTime(0.f), m_Time(0.f), m_bTwice(FALSE)
{
}

CEffect_Kaboom::~CEffect_Kaboom()
{
}

void CEffect_Kaboom::Set_Pos(_vec3 vPos)
{
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vInfo[ENGINE::INFO_POS].y += 1.f;
}

void CEffect_Kaboom::Set_LifeTime(_double fLife, _double fDelay)
{
	m_LifeTime = fLife;
	m_Time -= fDelay;
}

HRESULT CEffect_Kaboom::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 5.f, 5.f, 5.f };
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };
	m_pCollider->Set_Scale(0.1f);
	
	return S_OK;
}

_int CEffect_Kaboom::Update_Object(const _double& TimeDelta)
{
	//ENGINE::CGameObject::Late_Init();
	m_Time += TimeDelta;
	ENGINE::CGameObject::Update_Object(TimeDelta);
	m_pCollider->Set_Collider(&m_pTransform->m_matWorld);

	if (m_Time > m_LifeTime)
		return END_EVENT;

	Update_Frame(TimeDelta);
	Update_Billboard();

	if(!m_bTwice)
		Check_EnemyGroup();

	Compute_ViewZ(&m_pTransform->m_vInfo[ENGINE::INFO_POS]);
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_ALPHA, this);

	return NO_EVENT;
}

void CEffect_Kaboom::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CEffect_Kaboom::Render_Object()
{
	//Render_Set();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	if (m_Time >= 0.f)		//Delay Render
	{
		m_pTexture->Render_Texture(m_Frame);
		m_pBuffer->Render_Buffer();
	}

	//m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pTransform->m_matWorld);

	//Render_ReSet();
}

HRESULT CEffect_Kaboom::Add_Component()
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
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Texture_Explosion"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Texture", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Transform", pComponent);

	//Collider Component
	pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev, 30.f, m_pTransform->Get_vInfoPos(ENGINE::INFO_POS));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	////////////////////////////
	return S_OK;
}

void CEffect_Kaboom::Update_Frame(const _double & TimeDelta)
{
	if (m_Time > 0.f)
		m_Frame += m_MaxFrame * TimeDelta;

	if (m_Frame > m_MaxFrame)
		m_Frame = 0.f;
}

void CEffect_Kaboom::Update_Billboard()
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

void CEffect_Kaboom::Check_EnemyGroup()
{
	Check_KaboomColl(L"Player");
	Check_KaboomColl(L"Troll");
	Check_KaboomColl(L"Enemy_Swordman");
	Check_KaboomColl(L"Enemy_Spearman");
	Check_KaboomColl(L"Enemy_Shieldman");
	Check_KaboomColl(L"Enemy_Bowman");
	Check_KaboomColl(L"Boss_Keroberos");
	
	m_bTwice = TRUE;
}

void CEffect_Kaboom::Check_KaboomColl(const _tchar * pObjTag)
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	if (pLayer->Get_MapObject(pObjTag).empty())
		return;
	for (auto pList : pLayer->Get_MapObject(pObjTag))
	{
		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pTrans->Get_Dead())
			continue;

		ENGINE::CSphereColl* pTarget = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (m_pCollider->Check_ComponentColl(pTarget))
		{
			pTarget->m_bHit = TRUE;
			pTarget->m_bKnockBack = TRUE;

			_vec3 vKnockDir = pTarget->Get_CollPos() - m_pTransform->Get_vInfoPos(ENGINE::INFO_POS);
			vKnockDir.y = 0.f;
			D3DXVec3Normalize(&vKnockDir, &vKnockDir);
			pTarget->Set_KnockBackDist(TRUE, vKnockDir);
			pTarget->Get_iHp(5);
		}
	}

}


void CEffect_Kaboom::Render_Set()
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

void CEffect_Kaboom::Render_ReSet()
{
	//Alpha Test End
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	//Alpha Blend End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CEffect_Kaboom * CEffect_Kaboom::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect_Kaboom* pInstance = new CEffect_Kaboom(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

CEffect_Kaboom * CEffect_Kaboom::Create(LPDIRECT3DDEVICE9 pDevice, _vec3 vPos, _double fLife, _double fDelay)
{
	CEffect_Kaboom* pInstance = new CEffect_Kaboom(pDevice);

	if (FAILED(pInstance->Ready_Object()))
		ENGINE::Safe_Release(pInstance);

	pInstance->Set_Pos(vPos);
	pInstance->Set_LifeTime(fLife, fDelay);

	return pInstance;
}

void CEffect_Kaboom::Free()
{
	ENGINE::CGameObject::Free();
}
