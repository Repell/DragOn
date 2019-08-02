#include "Weapon.h"

#include "Export_Utility.h"

USING(ENGINE)

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pDevice)
	:m_pGraphicDev(pDevice)
{
	m_pGraphicDev->AddRef();

	iDamage = 2;
	m_iCurAni = 0;
	m_iOldAni = 0;
	m_fAngle = 20.f;

}

void CWeapon::Set_AttackState(_bool bState, _uint iCurAni, _uint iCombo, _uint iPower)
{
	m_bAttack = bState;
	iDamage = iPower;
	m_iComboCnt = iCombo;
	m_iCurAni = iCurAni;
}

_bool CWeapon::Check_ComponentColl(CSphereColl* pSphere)
{
	return m_pCollider->Check_ComponentColl(pSphere);
}


HRESULT CWeapon::Ready_Component(ENGINE::CTransform * pTarget, UNITINFO vInfo, const _tchar* szWeapon)
{
	Add_Component(vInfo.m_vPos, szWeapon, vInfo.m_fRadius);
	m_pTarget = pTarget;
	m_bPlayer = vInfo.m_bPlayer;

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vAngle = vInfo.m_vAngle;

	m_pCollider->Set_Scale(vInfo.m_vScale.x);


	return S_OK;
}

HRESULT CWeapon::Add_Component(_vec3 vPos, const _tchar* szWeapon, _float fRadius)
{
	m_pMesh = dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(1, szWeapon));

	m_pTransform = CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);

	m_pCollider = CCollider::Create(m_pGraphicDev, fRadius, vPos);
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);

	m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(m_pShader, E_FAIL);

	return S_OK;
}

_int CWeapon::Update_Component(const _double & TimeDelta)
{
	m_pTransform->Update_Component(TimeDelta);

	if (m_bAttack && m_bPlayer)
	{
		//객체 리스트별 따로 체크
		Check_EnemyColl(L"Troll");
		Check_EnemyColl(L"Enemy_Swordman");
		Check_EnemyColl(L"Enemy_Spearman");
		Check_EnemyColl(L"Enemy_Shieldman");
		Check_EnemyColl(L"Enemy_Bowman");
	}
	else if (m_bAttack && !m_bPlayer)
		Check_PlayerColl(L"Player");



	return NO_EVENT;
}

void CWeapon::Late_Update_Component()
{
	m_pTransform->Late_Update_Component();

	if (!m_bAttack && m_bPlayer)
	{
		Reset_EnemyColl(L"Troll");
		Reset_EnemyColl(L"Enemy_Swordman");
		Reset_EnemyColl(L"Enemy_Spearman");
		Reset_EnemyColl(L"Enemy_Shieldman");
		Reset_EnemyColl(L"Enemy_Bowman");
	}
}

void CWeapon::Render_Weapon(const _matrix* pParentMat)
{
	m_pTransform->Set_ParentMatrix(pParentMat);

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	////////////////////////////////////////

	m_pMesh->Render_Meshes();

	////////////////////////////////////////
	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	////////////////////////////////////////
	m_pCollider->Set_Collider(&m_pTransform->m_matWorld);

	if (m_bAttack)
		m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pTransform->m_matWorld);

}


HRESULT CWeapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();
	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pTransform->m_matWorld);

	pEffect->SetMatrix("g_matView", &matView);

	pEffect->SetMatrix("g_matProj", &matProj);

	ENGINE::Safe_Release(pEffect);

	return S_OK;
}

void CWeapon::Check_PlayerColl(const _tchar * pObjTag)
{

	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(pObjTag))
	{
		if (pLayer->Get_MapObject(pObjTag).empty())
			return;

		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pTrans->Get_Dead())
			return;

		ENGINE::CSphereColl* pTarget = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (pTarget == nullptr)
			return;

		_bool bColl = m_pCollider->Check_ComponentColl(pTarget);

		if (bColl && m_iOldAni != m_iCurAni && pTarget->Get_iHitStack() == 1)
		{
			pTarget->Get_iHp(iDamage);
			pTarget->Get_iHitStack(TRUE);
			m_iOldAni = m_iCurAni;
		}

	}
}

void CWeapon::Check_EnemyColl(const _tchar * pObjTag)
{

	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(pObjTag))
	{
		if (pLayer->Get_MapObject(pObjTag).empty())
			return;

		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pTrans->Get_Dead())
			continue;

		ENGINE::CSphereColl* pTarget = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (pTarget == nullptr)
			continue;

		_bool bColl = m_pCollider->Check_ComponentColl(pTarget);
		_vec3 vKnockDir = { 0.f, 0.f, 0.f };

		if (m_iCurAni != m_iOldAni)
			pTarget->Get_iHitStack(TRUE);

		if (bColl && pTarget->Get_iHitStack(FALSE) == 1)
		{
			pTarget->Get_iHp(iDamage);
			m_iOldAni = m_iCurAni;

			switch (m_iComboCnt)
			{
			case 6:
			case 7:
				pTarget->m_bAirbone = TRUE;
				break;
			case 4:
			case 8:
				pTarget->m_bKnockBack = TRUE;
				vKnockDir = pTarget->Get_CollPos() - m_pTarget->Get_vInfoPos(ENGINE::INFO_POS);
				vKnockDir.y = 0.f;
				D3DXVec3Normalize(&vKnockDir, &vKnockDir);
				pTarget->Set_KnockBackDist(TRUE, vKnockDir);
				break;
			default:
				pTarget->m_bHit = TRUE;
				break;
			}

		}

	}

}

void CWeapon::Reset_EnemyColl(const _tchar * pObjTag)
{
	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(pObjTag))
	{
		if (pLayer->Get_MapObject(pObjTag).empty())
			return;

		ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
			(pList->Get_Component(L"Com_Transform", ENGINE::COMP_DYNAMIC));

		if (pTrans->Get_Dead())
			continue;

		ENGINE::CSphereColl* pTarget = dynamic_cast<ENGINE::CSphereColl*>
			(pList->Get_Component(L"Com_SphereColl", ENGINE::COMP_STATIC));

		if (pTarget == nullptr)
			continue;

		pTarget->Get_iHitStack(TRUE);
		m_iOldAni = 0;
	}
}

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 pDevice, CTransform * pTarget, UNITINFO vInfo, const _tchar* szWeapon)
{
	CWeapon* pInstance = new CWeapon(pDevice);

	if (FAILED(pInstance->Ready_Component(pTarget, vInfo, szWeapon)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWeapon::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pShader);
	Safe_Release(m_pMesh);
	Safe_Release(m_pTransform);
	Safe_Release(m_pGraphicDev);
}
