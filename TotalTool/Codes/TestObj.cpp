#include "stdafx.h"
#include "TestObj.h"

#define SPEED 5.f
#define ANGLE 90.f

CTestObj::CTestObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), bDelete(FALSE), bMeshState(FALSE) 
{
}

CTestObj::~CTestObj()
{
}

void CTestObj::Set_DeleteObj()
{
	bDelete = TRUE;
}

void CTestObj::Set_Transform(const _vec3 vPos, const _vec3 vRot, const _vec3 vSize)
{
	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransCom->m_vAngle = vRot;
	m_pTransCom->m_vScale = vSize;
}

_vec3 CTestObj::Get_vPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pTransCom->m_vInfo[ENGINE::INFO_POS];
}

_vec3 CTestObj::Get_vRot()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pTransCom->m_vAngle;
}

_vec3 CTestObj::Get_vScale()
{
	return m_pTransCom->m_vScale;
}

_ulong CTestObj::Get_NumVtx(void)
{
	if (!bMeshState)
		return m_pMeshCom->Get_NumVtx();

	return 0;
}

_ulong CTestObj::Get_Stride(void)
{
	if (!bMeshState)
		return m_pMeshCom->Get_Stride();

	return 0;
}

_vec3 * CTestObj::Get_VtxMeshPos(void)
{
	return nullptr;
}

HRESULT CTestObj::Ready_MeshObject(wstring strMesh)
{
	FAILED_CHECK_RETURN(Add_Component_MeshObject(strMesh), E_FAIL);

	if(m_pDynamicCom != nullptr)
		m_pDynamicCom->Set_AnimationSet(0);

	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = { 0.f, 0.f, 0.f };
	m_pTransCom->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

HRESULT CTestObj::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = { 2.f, 1.f, 2.f };
	m_pTransCom->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

HRESULT CTestObj::Late_Init()
{

	return S_OK;
}

_int CTestObj::Update_Object(const _float & fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	if(m_pDynamicCom != nullptr)
		m_pDynamicCom->Play_AnimationSet(fTimeDelta);

	if (bDelete)
		return 999;

	m_pRenderCom->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);
	return 0;
}

void CTestObj::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	m_matWorld = m_pTransCom->m_matWorld;
}

void CTestObj::Render_Object()
{
	Render_Set();
	////////////////////

	if(m_pMeshCom != nullptr)
		m_pMeshCom->Render_Meshes();	//Object
	if (m_pDynamicCom != nullptr)
		m_pDynamicCom->Render_Meshes();	//Object

	////////////////////
	Render_ReSet();

	////////
	//Render_Font(&_vec2(10.f, 10.f));
}

HRESULT CTestObj::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<ENGINE::CStaticMesh*>
		(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Mesh_TombStone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//Transform
	pComponent = m_pTransCom = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer
	pComponent = m_pRenderCom = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

HRESULT CTestObj::Add_Component_MeshObject(wstring strMesh)
{
	ENGINE::CComponent* pComponent = nullptr;

	// Mesh

	if (bMeshState)
	{
		pComponent = m_pDynamicCom = dynamic_cast<ENGINE::CDynamicMesh*>
			(ENGINE::Clone_Resources(RESOURCE_TOOL, strMesh.c_str()));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

		m_pMeshCom = nullptr;
	}
	else if (!bMeshState)
	{
		pComponent = m_pMeshCom = dynamic_cast<ENGINE::CStaticMesh*>
			(ENGINE::Clone_Resources(RESOURCE_TOOL, strMesh.c_str()));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Mesh", pComponent);

		m_pDynamicCom = nullptr;
	}

	//Transform
	pComponent = m_pTransCom = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer
	pComponent = m_pRenderCom = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}


void CTestObj::Render_Set()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
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

void CTestObj::Render_ReSet()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CTestObj::Render_Font(const _vec2 * pPos)
{
	RECT rc = { _long(pPos->x), _long(pPos->y) };
	//Fonts
	_tchar szStr[MAX_PATH] = L"";
	_vec3 vPos = m_pTransCom->m_vInfo[ENGINE::INFO_POS];
	_float fAngleX = m_pTransCom->m_vAngle.x;
	_float fAngleY = m_pTransCom->m_vAngle.y;

	//Render Text
	swprintf_s(szStr, L"%5.2f , %5.2f , %5.2f", vPos.x, vPos.y, vPos.z);
	//swprintf_s(szStr, L"%5.2f , %5.2f", fAngleX, fAngleY);

	ENGINE::Render_Font(L"Sp", szStr, pPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

CTestObj * CTestObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestObj* pInstnace = new CTestObj(pGraphicDev);

	if (FAILED(pInstnace->Ready_Object()))
	{
		ERR_BOX("Test Obj Create Failed!");
		return nullptr;
	}

	return pInstnace;
}

CTestObj * CTestObj::Create_MeshObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring strMesh, _vec3 vTransform[], _bool bMesh)
{
	CTestObj* pInstnace = new CTestObj(pGraphicDev);

	pInstnace->Set_MeshState(bMesh);

	if (FAILED(pInstnace->Ready_MeshObject(strMesh)))
	{
		ERR_BOX("Test Obj Create Failed!");
		return nullptr;
	}
	
	if(vTransform != nullptr)
		pInstnace->Set_Transform(vTransform[0], vTransform[1], vTransform[2]);

	return pInstnace;
}

void CTestObj::Free()
{
	ENGINE::CGameObject::Free();
}
