#include "ProtoTypeMgr.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}

CProtoMgr::~CProtoMgr()
{
	Free();
}


HRESULT CProtoMgr::Ready_ProtoMgr(const wstring pProtoTag, CComponent * pComp)
{
	CComponent* pProto = Find_ProtoType(pProtoTag);

	if (nullptr != pProto)
		return E_FAIL;

	m_MapProto.emplace(pProtoTag, pComp);

	return S_OK;
}

CComponent * CProtoMgr::Clone(const wstring pPrototag)
{
	CComponent*	pPrototype = Find_ProtoType(pPrototag);

	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone();
}


CComponent * CProtoMgr::Find_ProtoType(const wstring pProtoTag)
{
	
	auto iter = find_if(m_MapProto.begin(), m_MapProto.end(), [pProtoTag](auto& MyPair)
	{
		if (MyPair.first == pProtoTag)
			return TRUE;
		else
			return FALSE;

	});

	if(iter == m_MapProto.end())
		return nullptr;
	
	return iter->second;
}

void CProtoMgr::Free()
{
	for_each(m_MapProto.begin(), m_MapProto.end(), [](auto& MyPair)
	{
		Safe_Release(MyPair.second);
	});
	m_MapProto.clear();
}
