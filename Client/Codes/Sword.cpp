#include "stdafx.h"
#include "Sword.h"

#include "Export_Function.h"

CSword::CSword(LPDIRECT3DDEVICE9 pDevice)
	:	CGameObject(pDevice),
	m_pMesh(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr),
	m_pParentBoneMatrix(nullptr), m_pParentWorldMatrix(nullptr)
{
	iDamage = 2;
	m_iCurAni = 0;
	m_iOldAni = 0;
	m_fAngle = 20.f;
}

CSword::~CSword()
{

}

void CSword::Set_AttackState(_bool bState, _uint iCurAni, _uint iPower)
{
	bAttack = bState;
	iDamage = iPower;
	m_iCurAni = iCurAni;
}

HRESULT CSword::Ready_Object(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pMesh->Set_AnimationSet(57);
	m_iFlag = iFlag;
	bAttack = FALSE;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vAngle.x = 90.f;
	m_pTransform->m_vAngle.y = 20.f;
	m_pTransform->m_vAngle.z = 20.f;

	m_pCollider->Set_Scale(0.01f);

	return S_OK;
}

HRESULT CSword::Late_Init()
{

	return S_OK;
}

_int CSword::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	
	Get_ParentMatrix();
	ENGINE::CGameObject::Update_Object(TimeDelta);
	
	m_pTransform->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));	//공전 * 부모
	m_pRenderer->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);

	if (bAttack)
		Check_EnemyColl(L"Troll", L"Com_SphereColl");
	
	return NO_EVENT;
}

void CSword::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	//if (ENGINE::Key_Press(ENGINE::dwKEY_F1))
	//	m_fAngle += 1.f;
	//if (ENGINE::Key_Press(ENGINE::dwKEY_F2))
	//	m_fAngle -= 1.f;

	//m_pTransform->m_vAngle.y = m_fAngle;
	
}

void CSword::Render_Object()
{
	//Render_Set();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);

	m_pMesh->Render_Meshes();

	if (bAttack)
		m_pCollider->Render_Collider(ENGINE::COL_TRUE, &m_pTransform->m_matWorld, _vec3(0.f, 0.f, -110.f));

	//Render_ReSet();
}

void CSword::Render_Set()
{
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
}

void CSword::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CSword::Check_EnemyColl(const _tchar* pObjTag, const _tchar* pCompTag)
{

	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	for (auto pList : pLayer->Get_MapObject(L"Troll"))
	{
		if (pLayer == nullptr)
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

		if (bColl && m_iOldAni != m_iCurAni && pTarget->Get_iHitStack() == 1)
		{
			pTarget->Get_iHp(iDamage);
			pTarget->Get_iHitStack(TRUE);
			m_iOldAni = m_iCurAni;
		}

	}
	
}

void CSword::Get_ParentMatrix()
{
	ENGINE::CTransform* pTrans = dynamic_cast<ENGINE::CTransform*>
		(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Transform", ENGINE::COMP_DYNAMIC));

	if (nullptr == m_pParentBoneMatrix)
	{
		ENGINE::CDynamicMesh* pPlayerMesh = dynamic_cast<ENGINE::CDynamicMesh*>
			(ENGINE::Get_Component(ENGINE::CLayer::OBJECT, L"Player", L"Com_Mesh", ENGINE::COMP_DYNAMIC));

		if (nullptr == pPlayerMesh)
			return;

		const ENGINE::D3DXFRAME_DERIVED* pFrame = pPlayerMesh->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->combinedTransformMatrix;


		if (nullptr == pTrans)
			return;

		m_pParentWorldMatrix = &(pTrans->m_matWorld);
	}
}


HRESULT CSword::Add_Component()
{

	ENGINE::CComponent* pComponent = nullptr;
	/////////INSERT COMPONENT/////////

	pComponent = m_pMesh =dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, L"Mesh_Sword"));
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
	
	pComponent = m_pCollider = ENGINE::CCollider::Create(m_pGraphicDev, 40.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Collider", pComponent);

	////////////////////////////
	return S_OK;
}

CSword * CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CSword* pInstance = new CSword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iFlag)))
		ENGINE::Safe_Release(pInstance);

	return pInstance;
}

void CSword::Free()
{


	ENGINE::CGameObject::Free();
}
