#include "Layer.h"

USING(ENGINE)

CLayer::CLayer(void)
{

}

CLayer::~CLayer()
{
	
}

CComponent * CLayer::Get_Component(const wstring pMapObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), [pMapObjTag](auto& MyPair) 
	{
		if (MyPair.first == pMapObjTag)
			return TRUE;
		
		return FALSE;
	});

	if (iter == m_mapObject.end())
		return nullptr;

	auto iter_List = iter->second.begin();

	for (; iter_List != iter->second.end(); ++iter_List)
	{
		CComponent* pComponent = (*iter_List)->Get_Component(pComponentTag, eID);

		if (nullptr != pComponent)
			return pComponent;
	}

	return nullptr;
}




HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _double& TimeDelta)
{
	MAP_GAMEOBJ::iterator iter_Begin = m_mapObject.begin();
	MAP_GAMEOBJ::iterator iter_End = m_mapObject.end();
	OBJLIST::iterator iter = {};

	int iEvnet = 0;

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		iter = iter_Begin->second.begin();
		for (; iter != iter_Begin->second.end();)
		{
			iEvnet = (*iter)->Update_Object(TimeDelta);

			if (-1 == iEvnet)
			{
				Safe_Release(*iter);
				iter = iter_Begin->second.erase(iter);
			}
			else if (iEvnet & 0x80000000)
				return iEvnet;
			else
				++iter;
		}
	}

	return iEvnet;
}

void CLayer::Late_Update_Layer(void)
{
	MAP_GAMEOBJ::iterator iter_Begin = m_mapObject.begin();
	MAP_GAMEOBJ::iterator iter_End = m_mapObject.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		for (auto& pObj : iter_Begin->second)
			pObj->Late_Update_Object();
	}
}

void CLayer::Render_Layer(void)
{
	MAP_GAMEOBJ::iterator iter_Begin = m_mapObject.begin();
	MAP_GAMEOBJ::iterator iter_End = m_mapObject.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		for (auto& pObj : iter_Begin->second)
			pObj->Render_Object();
	}
}

void CLayer::Free(void)
{
	//Safe_Release(m_pGraphicDev);

	MAP_GAMEOBJ::iterator iter_Begin = m_mapObject.begin();
	MAP_GAMEOBJ::iterator iter_End = m_mapObject.end();

	for (; iter_Begin != iter_End; ++iter_Begin)
	{
		for_each(iter_Begin->second.begin(), iter_Begin->second.end(), CDeleteObj());

		iter_Begin->second.clear();
	}
	m_mapObject.clear();
}

CLayer * CLayer::Create(void)
{
	CLayer* pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
	{
		ERR_BOX("CLayer Create Failed!");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CLayer::Add_GameObject(const wstring pObjTag, CGameObject * pObj)
{
	if (nullptr == pObj)
		return E_FAIL;

	m_mapObject[pObjTag].emplace_back(pObj);

	return S_OK;
}
