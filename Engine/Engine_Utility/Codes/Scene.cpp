#include "Scene.h"

USING(ENGINE)

CScene::CScene(LPDIRECT3DDEVICE9 pDev)
	: m_pGraphicDev(pDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{

}

CLayer * CScene::Get_MapLayer(CLayer::LAYER_ID eID)
{
	return m_MapLayer[eID];
}

CComponent * CScene::Get_Component(const CLayer::LAYER_ID eLay, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	return m_MapLayer[eLay]->Get_Component(pObjTag, pComponentTag, eID);
}

_int CScene::Update_Scene(const _double& TimeDelta)
{
	MAP_LAYER::iterator iter_Begin = m_MapLayer.begin();
	MAP_LAYER::iterator iter_End = m_MapLayer.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
		iter_Begin->second->Update_Layer(TimeDelta);

	return 0;
}

void CScene::Late_Update_Scene()
{
	MAP_LAYER::iterator iter_Begin = m_MapLayer.begin();
	MAP_LAYER::iterator iter_End = m_MapLayer.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
		iter_Begin->second->Late_Update_Layer();
}

void CScene::Render_Scene()
{
	MAP_LAYER::iterator iter_Begin = m_MapLayer.begin();
	MAP_LAYER::iterator iter_End = m_MapLayer.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
		iter_Begin->second->Render_Layer();
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

void CScene::Free()
{
	Safe_Release(m_pGraphicDev);

	for_each(m_MapLayer.begin(), m_MapLayer.end(), [](auto& MyPair) {
		ENGINE::Safe_Release(MyPair.second);
	});

	m_MapLayer.clear();
}
