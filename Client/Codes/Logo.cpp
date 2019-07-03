#include "stdafx.h"
#include "Logo.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
	:	ENGINE::CScene(pDevice)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	RenderPipeLine_SetUp();
	
	FAILED_CHECK_RETURN(ENGINE::CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Add_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI_Layer(), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_LOGO);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(_float fTimeDelta)
{
	ENGINE::CScene::Update_Scene(fTimeDelta);

	return NO_EVENT;
}

void CLogo::Late_Update_Scene()
{
	Title_Change();
	ENGINE::CScene::Late_Update_Scene();

	if (m_pLoading->Get_Finished())
	{
		if (ENGINE::Key_Down(ENGINE::dwKEY_RETURN))
		{
			ENGINE::CScene* pScene = nullptr;

			pScene = CTestStage::Create(m_pGraphicDev);
			NULL_CHECK(pScene);

			ENGINE::SetUp_CurrentScene(pScene);
		}
	}
}

void CLogo::Render_Scene()
{
	//Only Debug Mode
//#ifdef _DEBUG
//	ENGINE::CScene::Render_Scene(); 
//#endif // _DEBUG
}

HRESULT CLogo::Add_Environment_Layer()
{
	return S_OK;
}

HRESULT CLogo::Add_GameObject_Layer()
{
	ENGINE::CLayer* pObject_Layer = ENGINE::CLayer::Create();
	NULL_CHECK_RETURN(pObject_Layer, E_FAIL);

	ENGINE::CGameObject* pObject = nullptr;
	////////////INSERT GAME OBJECT////////////

	//Logo_loading
	pObject = CLogo_Loading::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pObject, E_FAIL);
	pObject_Layer->Add_GameObject(L"Logo_Loading", pObject);

	//////////////INSERT LAYER//////////////
	m_MapLayer.emplace(ENGINE::CLayer::OBJECT, pObject_Layer);

	return S_OK;
}

HRESULT CLogo::Add_UI_Layer()
{
	return S_OK;
}

void CLogo::RenderPipeLine_SetUp()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CLogo::Title_Change()
{
	CLogo_Loading* pLoading =dynamic_cast<CLogo_Loading*>(m_MapLayer[ENGINE::CLayer::OBJECT]->Get_MapObject(L"Logo_Loading").front());
	pLoading->Set_Loading(m_pLoading->Get_Finished());
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pInstance = new CLogo(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		ENGINE::Safe_Release(pInstance);

		return pInstance;
}

void CLogo::Free()
{
	ENGINE::Safe_Release(m_pLoading);
	ENGINE::CScene::Free();
}
