#include "stdafx.h"
#include "StaticObj.h"

#include "Export_Function.h"

#define _SPEED 5.f
#define _ANGLE 90.f

CStaticObj::CStaticObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStaticObj::~CStaticObj()
{
}

void CStaticObj::Set_Transform(const _vec3 vPos, const _vec3 vRot, const _vec3 vSize)
{
	m_pTransform->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransform->m_vAngle = vRot;
	m_pTransform->m_vScale = vSize;
}

_vec3 CStaticObj::Get_vPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pTransform->m_vInfo[ENGINE::INFO_POS];
}

_vec3 CStaticObj::Get_vRot()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pTransform->m_vAngle;
}

_vec3 CStaticObj::Get_vScale()
{
	return m_pTransform->m_vScale;
}

HRESULT CStaticObj::Ready_MeshObject(wstring strMesh)
{
	FAILED_CHECK_RETURN(Add_Component_MeshObject(strMesh), E_FAIL);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

HRESULT CStaticObj::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[ENGINE::INFO_POS] = { 2.f, 1.f, 2.f };
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

HRESULT CStaticObj::Late_Init()
{

	return S_OK;
}

_int CStaticObj::Update_Object(const _double& TimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(TimeDelta);

	m_pRender->Add_RenderGroup(ENGINE::RENDER_NONALPHA, this);
	return 0;
}

void CStaticObj::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	m_matWorld = m_pTransform->m_matWorld;
}

void CStaticObj::Render_Object()
{
	//Render_Set();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->m_matWorld);
	////////////////////

	m_pMesh->Render_Meshes();	//Object

	////////////////////
	//Render_ReSet();

	////////
}

HRESULT CStaticObj::Add_Component()
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

	return S_OK;
}

HRESULT CStaticObj::Add_Component_MeshObject(wstring strMesh)
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

	return S_OK;
}


void CStaticObj::Render_Set()
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

void CStaticObj::Render_ReSet()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CStaticObj::Render_Font(const _vec2 * pPos)
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

CStaticObj * CStaticObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticObj* pInstnace = new CStaticObj(pGraphicDev);

	if (FAILED(pInstnace->Ready_Object()))
	{
		ERR_BOX("Test Obj Create Failed!");
		return nullptr;
	}

	return pInstnace;
}

CStaticObj * CStaticObj::Create_MeshObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring strMesh, _vec3 vTransform[])
{
	CStaticObj* pInstnace = new CStaticObj(pGraphicDev);

	if (FAILED(pInstnace->Ready_MeshObject(strMesh)))
	{
		ERR_BOX("Test Obj Create Failed!");
		return nullptr;
	}

	if (vTransform != nullptr)
		pInstnace->Set_Transform(vTransform[0], vTransform[1], vTransform[2]);

	return pInstnace;
}

void CStaticObj::Free()
{
	ENGINE::CGameObject::Free();
}
