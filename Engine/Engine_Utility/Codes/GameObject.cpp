#include "GameObject.h"

USING(ENGINE)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDev)
	: m_pGraphicDev(pDev), m_bIsInit(false)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{

}

CComponent * CGameObject::Get_Component(const wstring pComponentTag, COMPONENTID eID)
{
	auto iter = find_if(m_MapComponent[eID].begin(), m_MapComponent[eID].end(), [pComponentTag](auto& MyPair) 
	{
		if (MyPair.first == pComponentTag)
			return TRUE;
		else
			return FALSE;
	});

	if (iter == m_MapComponent[eID].end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Ready_Object()
{
	return S_OK;
}

_int CGameObject::Update_Object(const _float& fTimeDelta)
{
	for (_uint i = 0; i < COMP_END; ++i)
	{
		MAP_COMPONENT::iterator iter_Begin = m_MapComponent[i].begin();
		MAP_COMPONENT::iterator iter_End = m_MapComponent[i].end();

		for (; iter_Begin != iter_End; ++iter_Begin)
			iter_Begin->second->Update_Component(fTimeDelta);
	}

	return 0;
}

void CGameObject::Late_Update_Object()
{
	for (_uint i = 0; i < COMP_END; ++i)
	{
		MAP_COMPONENT::iterator iter_Begin = m_MapComponent[i].begin();
		MAP_COMPONENT::iterator iter_End = m_MapComponent[i].end();

		for (; iter_Begin != iter_End; ++iter_Begin)
			iter_Begin->second->Late_Update_Component();
	}
}

void CGameObject::Render_Object()
{

}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}


HRESULT CGameObject::Late_Init()
{
	if (!m_bIsInit)
	{
		this->Late_Init();
		m_bIsInit = true;
	}

	return S_OK;
}

void CGameObject::Free()
{
	Safe_Release(m_pGraphicDev);

	for (_ulong i = 0; i < COMP_END; ++i)
	{
		for_each(m_MapComponent[i].begin(), m_MapComponent[i].end(), [](auto& MyPair)
		{
			Safe_Release(MyPair.second);
		});
		//for_each(m_MapComponent[i].begin(), m_MapComponent[i].end(), CDeleteMap());
		m_MapComponent[i].clear();
	}

}
