#ifndef ProtoTypeMgr_h__
#define ProtoTypeMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr();
	virtual ~CProtoMgr();

private:
	CComponent* Find_ProtoType(const wstring pProtoTag);

private:
	map<const wstring, CComponent*>	m_MapProto;

public:
	HRESULT Ready_ProtoMgr(const wstring pProtoTag, CComponent* pComp);
	CComponent* Clone(const wstring pPrototag);

public:
	virtual void Free();


};

NS_END
#endif // ProtoTypeMgr_h__
