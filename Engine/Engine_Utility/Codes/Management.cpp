#include "Management.h"
#include "ProtoTypeMgr.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement(void)
	: m_pCurrentScene(nullptr)
{

}

CManagement::~CManagement(void)
{
	Free();
}

HRESULT CManagement::SetUp_CurrentScene(CScene* pCurrentScene)
{
	Safe_Release(m_pCurrentScene);
	
	m_pCurrentScene = pCurrentScene;

	return S_OK;
}

_int CManagement::Update_Scene(const _float& fTimeDelta)
{

	if (nullptr == m_pCurrentScene)
		return -1;

	m_pCurrentScene->Update_Scene(fTimeDelta);

	return 0;
}

void CManagement::Late_Update_Scene()
{
	if (nullptr == m_pCurrentScene)
		return;

	m_pCurrentScene->Late_Update_Scene();

}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer::Get_Instance()->Render_GameObject(pDevice);

	if (nullptr == m_pCurrentScene)
		return;

#ifdef _DEBUG
	m_pCurrentScene->Render_Scene();
#endif

}

void CManagement::Add_GameObject(ENGINE::CLayer::LAYER_ID eID, const wstring pObjTag, CGameObject * pObj)
{
	if (m_pCurrentScene == nullptr)
		return;

	m_pCurrentScene->Get_MapLayer(eID)->Add_GameObject(pObjTag, pObj);
}

HRESULT CManagement::Ready_ShaderFile(LPDIRECT3DDEVICE9 pGraphicDev)
{
	// Shader
	CShader*			pShader = nullptr;

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(CProtoMgr::Get_Instance()->Ready_ProtoMgr(L"Shader_Sample", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Shade.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(CProtoMgr::Get_Instance()->Ready_ProtoMgr(L"Shader_Shade", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(CProtoMgr::Get_Instance()->Ready_ProtoMgr(L"Shader_Terrain", pShader), E_FAIL);

	return S_OK;
}

void CManagement::Free(void)
{
	Safe_Release(m_pCurrentScene);
}

CComponent* CManagement::Get_Component(const CLayer::LAYER_ID eLay, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pCurrentScene)
		return nullptr;

	return m_pCurrentScene->Get_Component(eLay, pObjTag, pComponentTag, eID);
}

