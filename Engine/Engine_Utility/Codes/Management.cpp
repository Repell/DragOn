#include "Management.h"

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

void CManagement::Render_Scene(void)
{
	CRenderer::Get_Instance()->Render_GameObject();

	if (nullptr == m_pCurrentScene)
		return;

#ifdef _DEBUG
	m_pCurrentScene->Render_Scene();
#endif

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

