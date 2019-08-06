#include "stdafx.h"
#include "Static_Relic.h"

#include "Export_Function.h"

#define _SPEED 5.f
#define _ANGLE 90.f
#define _RADIUS 200.f

CStatic_Relic::CStatic_Relic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_bCharge = FALSE;
	m_PurificationTime = 0.0;
	for (int i = 0; i < 5; ++i)
		m_bPurification[i] = FALSE;
}

CStatic_Relic::~CStatic_Relic()
{
}

void CStatic_Relic::Set_Transform(const _vec3 vPos, const _vec3 vRot, const _vec3 vSize)
{
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vAngle = vRot;
	m_pTransform->m_vScale = vSize;
	m_fScale = 1.f;
}

_vec3 CStatic_Relic::Get_vPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pTransform->m_vInfo[ENGINE::INFO_POS];
}

_vec3 CStatic_Relic::Get_vRot()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pTransform->m_vAngle;
}

_vec3 CStatic_Relic::Get_vScale()
{
	return m_pTransform->m_vScale;
}

HRESULT CStatic_Relic::Ready_MeshObject(wstring strMesh)
{
	FAILED_CHECK_RETURN(Add_Component_MeshObject(strMesh), E_FAIL);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	m_pSphereColl->Set_Scale(0.2f);

	return S_OK;
}


HRESULT CStatic_Relic::Add_Component_MeshObject(wstring strMesh)
{
	ENGINE::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMesh = dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_LOGO, strMesh.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer
	pComponent = m_pRender = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 100);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	//Shadow Component
	pComponent = m_pAura = ENGINE::CAura::Create(m_pGraphicDev, L"Texture_HolyCircle");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Aura", pComponent);

	return S_OK;
}


HRESULT CStatic_Relic::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 2.f, 1.f, 2.f };
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

HRESULT CStatic_Relic::Late_Init()
{
	_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	vPos.y += 24.f;
	CGameObject* pObject = CEffect_Torch::Create(m_pGraphicDev, vPos, _vec3(4.f, 8.f, 4.f), 1000.f);
	m_pEffect = dynamic_cast<CEffect_Torch*>(pObject);
	ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"Torch", pObject);

	return S_OK;
}

_int CStatic_Relic::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	if (m_bCharge)
		Purification(TimeDelta);

	m_fScale = _float(m_pSphereColl->Get_iHp() * 0.01f);
	m_pEffect->Set_TotalScale(m_fScale);

	if (m_pSphereColl->Get_iHp() >= 300)
	{
		m_pSphereColl->Set_Invisible(TRUE);
		m_bCharge = TRUE;
	}

	if (m_pSphereColl->Get_iHp() < 0)
		return END_EVENT;

	m_pRender->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return NO_EVENT;
}

void CStatic_Relic::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	m_matWorld = m_pTransform->m_matWorld;
}

void CStatic_Relic::Render_Object()
{
	if (nullptr == m_pShader)
		return;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();

	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	///////////////////////////////////

	m_pMesh->Render_Meshes();

	///////////////////////////////////
	pEffect->EndPass();
	pEffect->End();

	ENGINE::Safe_Release(pEffect);

	///////////////////////////////////
	m_pSphereColl->Render_SphereColl(&m_pTransform->m_matWorld, (6.f * m_fScale));

	if(m_bCharge)
		m_pAura->Render_Shadow(&m_pTransform->m_matWorld, 30.f, 0.2f);

	_tchar szStr[MAX_PATH] = L"";


	swprintf_s(szStr, L"HP : %d ", m_pSphereColl->Get_iHp());
	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


}

HRESULT CStatic_Relic::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMesh = dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Mesh_TombStone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform
	pComponent = m_pTransform = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer
	pComponent = m_pRender = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);


	//Sphere Collider
	pComponent = m_pSphereColl = ENGINE::CSphereColl::Create(m_pGraphicDev, _RADIUS, 10);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_SphereColl", pComponent);

	//Shader 
	pComponent = m_pShader = dynamic_cast<ENGINE::CShader*>(ENGINE::Clone(L"Shader_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

void CStatic_Relic::Render_Set()
{
	//Lighting
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

void CStatic_Relic::Render_ReSet()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CStatic_Relic::Render_Font(const _vec2 * pPos)
{
	RECT rc = { _long(pPos->x), _long(pPos->y) };
	//Fonts
	_tchar szStr[MAX_PATH] = L"";
	_vec3 vPos = m_pTransform->m_vInfo[ENGINE::INFO_POS];
	_float fAngleX = m_pTransform->m_vAngle.x;
	_float fAngleY = m_pTransform->m_vAngle.y;

	//Render Text
	swprintf_s(szStr, L"%5.2f , %5.2f , %5.2f", vPos.x, vPos.y, vPos.z);
	//swprintf_s(szStr, L"%5.2f , %5.2f", fAngleX, fAngleY);

	ENGINE::Render_Font(L"Sp", szStr, pPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

void CStatic_Relic::Purification(const _double& TimeDelta)
{
	m_PurificationTime += TimeDelta;

	if (m_PurificationTime > 1.f && !m_bPurification[0])
	{
		//CGameObject* pObject = CShade::Create(m_pGraphicDev, L"Texture_HolyCircle");
		//ENGINE::Get_Management()->Add_GameObject(ENGINE::CLayer::OBJECT, L"HolyCircle", pObject);
		m_bPurification[0] = TRUE;
		m_bPurification[1] = TRUE;
	}

	ENGINE::CLayer* pLayer = ENGINE::Get_Management()->Get_Layer(ENGINE::CLayer::OBJECT);

	if (m_PurificationTime > 5.f && m_bPurification[1])
	{

		for (auto pList : pLayer->Get_MapObject(L"Boss_Blue"))
		{
			ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
				(pList->Get_Component(L"Com_SphereBody", ENGINE::COMP_STATIC));

			pSphere->m_bHit = TRUE;
			pSphere->Get_iHp(10);

		}
		m_bPurification[2] = TRUE;
	}

	if (m_PurificationTime > 10.f && m_bPurification[2])
	{
		for (auto& pList : pLayer->Get_MapObject(L"Boss_Blue"))
		{
			ENGINE::CSphereColl* pSphere = dynamic_cast<ENGINE::CSphereColl*>
				(pList->Get_Component(L"Com_SphereHead", ENGINE::COMP_STATIC));

			pSphere->Set_Invisible(FALSE);
			pSphere->Get_iHp(100);
		}

		if (m_pSphereColl->Get_iHp() > 100)
			m_pSphereColl->m_iHp -= 1;

	}

}

HRESULT CStatic_Relic::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CStatic_Relic * CStatic_Relic::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStatic_Relic* pInstnace = new CStatic_Relic(pGraphicDev);

	if (FAILED(pInstnace->Ready_Object()))
	{
		ERR_BOX("Test Obj Create Failed!");
		return nullptr;
	}

	return pInstnace;
}

CStatic_Relic * CStatic_Relic::Create_MeshObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring strMesh, _vec3 vTransform[])
{
	CStatic_Relic* pInstnace = new CStatic_Relic(pGraphicDev);

	if (FAILED(pInstnace->Ready_MeshObject(strMesh)))
	{
		ERR_BOX("Test Obj Create Failed!");
		return nullptr;
	}

	if (vTransform != nullptr)
		pInstnace->Set_Transform(vTransform[0], vTransform[1], vTransform[2]);

	return pInstnace;
}

void CStatic_Relic::Free()

{
	ENGINE::CGameObject::Free();
}
