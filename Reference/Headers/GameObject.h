#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Define.h"
#include "Base.h"

#include "Component.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CGameObject :
	public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDev);
	virtual ~CGameObject(void);

public:
	CComponent* Get_Component(const wstring pComponentTag, COMPONENTID eID);
	
public:
	virtual HRESULT Ready_Object();
	virtual HRESULT Late_Init();
	virtual _int Update_Object(const _float& fTimeDelta);
	virtual void Late_Update_Object();
	virtual void Render_Object();
	
protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	typedef	map<const wstring, CComponent*> MAP_COMPONENT;
	MAP_COMPONENT			m_MapComponent[COMP_END];
	bool m_bIsInit;
	
public:
	virtual void Free(void);

};

NS_END
#endif // GameObject_h__
