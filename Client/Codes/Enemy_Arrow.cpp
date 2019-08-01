#include "stdafx.h"
#include "Enemy_Arrow.h"

#include "Export_Function.h"

#define _SPEED 600.f
#define  _RADIUS 35.f

CEnemy_Arrow::CEnemy_Arrow(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), 
	m_LifeTime(0.0)
{
	ZeroMemory(&m_tInfo, sizeof(ENGINE::UNITINFO));
}

HRESULT CEnemy_Arrow::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh = dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Enemy_Arrow"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, -1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, m_tInfo.m_fRadius);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Renderer Component
	pComponent = m_pRenderer = ENGINE::Get_Renderer();
	pComponent->AddRef();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTarget = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	NULL_CHECK_RETURN(m_pTarget, E_FAIL);

	return S_OK;
}

HRESULT CEnemy_Arrow::Ready_Object(ENGINE::UNITINFO tInfo)
{
	m_tInfo = tInfo;
	Add_Component();
	m_pSphereColl->Set_Scale(0.006f);
	m_pTransform->Fix_TargetLook(m_pTarget, 30.f);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = tInfo.m_vPos;
	m_pTransform->m_vInfo[ENGINE::INFO_POS].y = 0.75f;
	m_pTransform->m_vScale = tInfo.m_vScale;
	m_pTransform->m_vAngle = tInfo.m_vAngle;
	m_pTransform->m_vLook = tInfo.m_vDir;
		
	return S_OK;
}

_int CEnemy_Arrow::Update_Object(const _double & TimeDelta)
{
	m_LifeTime += TimeDelta;
	
	CGameObject::Update_Object(TimeDelta);

	//Fly Arrow
	m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_tInfo.m_vDir * _SPEED * -TimeDelta;

	//Target Check
	if (Fly_Arrow() || m_LifeTime > 10.f)
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CEnemy_Arrow::Render_Object()
{
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

	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld);

	//_tchar szStr[MAX_PATH] = L"";
	//_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	//swprintf_s(szStr, L"X : %3.2f, Y : %3.2f, Z : %3.2f ", vPos.x, vPos.y, vPos.z);
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

_bool CEnemy_Arrow::Fly_Arrow()
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(L"Player"))
	{
		
		if (m_pTarget->Get_Dead())
			return FALSE;

		ENGINE::CSphereColl* pTarget = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (pTarget == nullptr)
			return FALSE;

		if (m_pSphereColl->Check_ComponentColl(pTarget))
		{
			pTarget->Get_iHp(2);
			pTarget->m_bHit = TRUE;

			if (pTarget->Get_HitState())
				pTarget->m_bKnockBack = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

HRESULT CEnemy_Arrow::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();
	_matrix matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matWorld, 0.f, 0.f, 80.f);
	matWorld *= m_pTransform->m_matWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matView", &matView);

	pEffect->SetMatrix("g_matProj", &matProj);

	ENGINE::Safe_Release(pEffect);

	return S_OK;
}

CEnemy_Arrow * CEnemy_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::UNITINFO tInfo)
{
	CEnemy_Arrow* pInstance = new CEnemy_Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tInfo)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CEnemy_Arrow::Free()
{
	CGameObject::Free();
}
