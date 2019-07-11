#include "stdafx.h"
#include "Terrain.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_pBufferCom(nullptr), m_pTexture(nullptr), m_pRenderCom(nullptr),
	bReset(false)
{
	m_iTex = 0;
}

CTerrain::~CTerrain()
{
}

void CTerrain::Set_Transform(const _vec3 vPos, const _vec3 vRot)
{
	m_pTransCom->m_vInfo[ENGINE::INFO_POS] = vPos;
	m_pTransCom->m_vAngle = vRot;
}

HRESULT CTerrain::Ready_Object()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransCom->m_vScale.x = 1.f;
	m_pTransCom->m_vScale.y = 1.f;

	return S_OK;
}

_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	m_pRenderCom->Add_RenderGroup(ENGINE::RENDER_PRIORITY, this);

	if (bReset)
		return 999;

	return 0;
}

void CTerrain::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
	CValueMgr::matTerrain = m_pTransCom->m_matWorld;
}

void CTerrain::Render_Object()
{
	Render_Set();
	////////////////////
	
	//Render
	m_pTexture->Render_Texture(m_iTex);
	if(m_pBufferCom != nullptr)
		m_pBufferCom->Render_Buffer();

	////////////////////
	Render_ReSet();
}

HRESULT CTerrain::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;

	//Buffer Compnent
	pComponent = m_pBufferCom = dynamic_cast<ENGINE::CTerrainTex*>
		(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_Terrain"));
		
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Buffer", pComponent);
	
	//Texture Component
	pComponent = m_pTexture = dynamic_cast<ENGINE::CTexture*>
		(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Texture_Terrain"));
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Texture", pComponent);

	//Transform
	pComponent = m_pTransCom = ENGINE::CTransform::Create(_vec3(0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//Renderer
	pComponent = m_pRenderCom = ENGINE::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_MapComponent[ENGINE::COMP_DYNAMIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

void CTerrain::Render_Set()
{
	//Lighting
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Alpha Test Begin
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x000000ff);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	//Render State
	if (CValueMgr::bWireFrm_Terrain)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//SetTransform
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
}

void CTerrain::Render_ReSet()
{
	//Alpha Test End
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//FillMode :: Default == D3DFILL_SOLID
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CTerrain::Render_Font(const _vec2 * pPos)
{
	RECT rc = { _long(pPos->x), _long(pPos->y) };
	//Fonts
	_tchar szStr[MAX_PATH] = L"";
	
	POINT pt = {};
	GetCursorPos(&pt);
	ClientToScreen(g_hWnd, &pt);

	swprintf_s(szStr, L"%d , %d", pt.x, pt.y);

	ENGINE::Render_Font(L"Sp", szStr, pPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CTerrain::LineMaker()
{
	_matrix matLine;
	D3DXMatrixIdentity(&matLine);

	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	matLine = matView * matProj;

	ENGINE::LINE_3D vLine(_vec3(0.f, 0.f, 0.f), 10.f);

	m_pLine->SetWidth(5.f);
	m_pLine->SetAntialias(TRUE);
	m_pLine->Begin();

	m_pLine->DrawTransform(vLine.vLine_X, 2, &matLine, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pLine->DrawTransform(vLine.vLine_Y, 2, &matLine, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	m_pLine->DrawTransform(vLine.vLine_Z, 2, &matLine, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	
	m_pLine->End();
}

void CTerrain::Release_Buffer()
{
	//auto iter = find(m_MapComponent[ENGINE::COMP_STATIC].begin(), m_MapComponent[ENGINE::COMP_STATIC].end(), L"Com_Buffer");
	auto& iter = m_MapComponent[ENGINE::COMP_STATIC][L"Com_Buffer"];

	if (iter == nullptr)
		return;
	
	ENGINE::Safe_Release(iter);
	m_MapComponent[ENGINE::COMP_STATIC].erase(L"Com_Buffer");
	m_pBufferCom = nullptr;
	
}

void CTerrain::Release_Texture()
{
	auto& iter = m_MapComponent[ENGINE::COMP_STATIC][L"Texture"];

	if (iter == nullptr)
		return;

	ENGINE::Safe_Release(iter);
	m_MapComponent[ENGINE::COMP_STATIC].erase(L"Texture");
	m_pTexture = nullptr;
}

HRESULT CTerrain::Create_Buffer()
{
	ENGINE::CComponent* pComponent = nullptr;

	//Buffer Compnent
	pComponent = m_pBufferCom = dynamic_cast<ENGINE::CTerrainTex*>
		(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_Terrain"));

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CTerrain::Set_Texture(const _tchar * cloneKey, const _int iTex)
{
	ENGINE::CComponent* pComponent = nullptr;

	//Texture Component
	pComponent = m_pTexture = dynamic_cast<ENGINE::CTexture*>
		(ENGINE::Clone_Resources(RESOURCE_TOOL, cloneKey));
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Texture", pComponent);

	m_iTex = iTex;

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		ERR_BOX("Terrain Create Failed!");
		return nullptr;
	}

	return pInstance;
}

void CTerrain::Free()
{
	ENGINE::CGameObject::Free();
}

