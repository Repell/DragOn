#include "stdafx.h"
#include "Dragon_GroundFire.h"

#include "Export_Function.h"

#define	_ATTACK_UP_END						3
#define	_ATTACK_DOWN_END				5
#define	_ATTACK_MID_END					7
#define _QUAKEEND								10


#define _SPEED 100.f
#define  _RADIUS 30.f

CDragon_GroundFire::CDragon_GroundFire(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), 
	m_LifeTime(0.0)
{
	ZeroMemory(&m_tInfo, sizeof(ENGINE::UNITINFO));
}

HRESULT CDragon_GroundFire::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh = dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Fireball"));
	NULL_CHECK_RETURN(m_pMesh, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform Component
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, -1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransform->m_vScale = {0.1f, 0.1f, 0.1f};

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

	//m_pTarget = dynamic_cast<ENGINE::CTransform*>
	//	(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Boss_Blue", L"Com_Transform", ENGINE::COMP_DYNAMIC));
	//NULL_CHECK_RETURN(m_pTarget, E_FAIL);

	return S_OK;
}

HRESULT CDragon_GroundFire::Ready_Object(ENGINE::UNITINFO tInfo)
{
	m_tInfo = tInfo;
	Add_Component();
	m_pSphereColl->Set_Scale(0.1f);
	//m_pTransform->Fix_TargetLook(m_pTarget, 30.f);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = tInfo.m_vPos;
	m_pTransform->m_vScale = tInfo.m_vScale;
	m_pTransform->m_vAngle = tInfo.m_vAngle;
	m_pTransform->m_vLook = tInfo.m_vDir;
		
	return S_OK;
}

_int CDragon_GroundFire::Update_Object(const _double & TimeDelta)
{
	m_LifeTime += TimeDelta;
	
	CGameObject::Update_Object(TimeDelta);

	//Fly Arrow
	m_pTransform->m_vInfo[ENGINE::INFO_POS] += m_tInfo.m_vDir * _SPEED * -TimeDelta;

	//Target Check
	//if (Check_FireballColl() || m_LifeTime > 10.f)
	//	return END_EVENT;

	if (m_pTransform->m_vInfo[ENGINE::INFO_POS].y < 1.5f)
	{
		CGameObject* pObject = CEffect_Tex::Create(m_pGraphicDev, m_pTransform->m_vInfo[ENGINE::INFO_POS], 1.f);
		ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"kaboom", pObject);
		return END_EVENT;
	}

	if (m_LifeTime > 10.f)
		return END_EVENT;

	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CDragon_GroundFire::Render_Object()
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

_bool CDragon_GroundFire::Check_FireballColl()
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(L"Boss_Blue"))
	{
		
		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pTrans->Get_Dead())
			continue;

		ENGINE::CSphereColl* pTargetHead = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereHead", ENGINE::COMP_STATIC));

		ENGINE::CSphereColl* pTargetBody = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereBody", ENGINE::COMP_STATIC));

		_uint attIdx = pTrans->m_iCurAniIndex;

		if (m_pSphereColl->Check_ComponentColl(pTargetBody) && !pTrans->m_bWeak)
		{
			switch (attIdx)
			{
			case _ATTACK_UP_END:
				//ÀÌÆåÆ® ÆãÆã
				//Ã¼·Â ¶³¾îÁü
				pTargetBody->m_bHit = TRUE;
				pTargetBody->Get_iHp(2);
				break;

			case _ATTACK_DOWN_END:
				//ÀÌÆåÆ® ÆãÆã
				//Ã¼·Â ¶³¾îÁü
				pTargetBody->m_bHit = TRUE;
				pTargetBody->Get_iHp(2);
				break;

			case _ATTACK_MID_END:
				//ÀÌÆåÆ® ÆãÆã
				//Ã¼·Â ¶³¾îÁü
				pTargetBody->m_bHit = TRUE;
				pTargetBody->Get_iHp(2);
				break;
				
			case _QUAKEEND:
				//ÀÌÆåÆ® ÆãÆã
				//Ã¼·Â ¶³¾îÁü
				pTargetBody->m_bHit = TRUE;
				pTargetBody->Get_iHp(2);
				break;
			}

			if (pTargetBody->Get_iHp() <= 0)
				pTrans->m_bWeak = TRUE;

			CGameObject* pObject = CEffect_Tex::Create(m_pGraphicDev, m_pTransform->m_vInfo[ENGINE::INFO_POS], 1.f);
			ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"kaboom", pObject);
		
			return TRUE;
		}

		if (m_pSphereColl->Check_ComponentColl(pTargetHead) && pTrans->m_bWeak)
		{
			if (pTargetHead->Get_iHp() != 0)
				pTargetHead->Get_iHp(2);
		
			return TRUE;
		}

	}

	return FALSE;
}

HRESULT CDragon_GroundFire::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();
	_matrix matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixTranslation(&matWorld, 0.f, 0.f, 80.f);
	matWorld *= m_pTransform->m_matWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matView", &matView);

	pEffect->SetMatrix("g_matProj", &matProj);

	ENGINE::Safe_Release(pEffect);

	return S_OK;
}

CDragon_GroundFire * CDragon_GroundFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, ENGINE::UNITINFO tInfo)
{
	CDragon_GroundFire* pInstance = new CDragon_GroundFire(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tInfo)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CDragon_GroundFire::Free()
{
	CGameObject::Free();
}
