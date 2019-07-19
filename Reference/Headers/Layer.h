#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CLayer :
	public CBase
{
public:
	enum LAYER_ID
	{
		ENVIRONMENT,
		OBJECT,
		UI
	};

public:	//Typedef
	typedef list<CGameObject*> OBJLIST;
	typedef map<const wstring, OBJLIST> MAP_GAMEOBJ;

private:
	explicit CLayer(void);
	virtual ~CLayer();

public:
	CComponent* Get_Component(const wstring pMapObjTag, const wstring pComponentTag, COMPONENTID eID);
	OBJLIST& Get_MapObject(const wstring pKey)
	{		return m_mapObject[pKey];	}

public:
	virtual HRESULT Ready_Layer(void);
	virtual _int Update_Layer(const _double& TimeDelta);
	virtual void Late_Update_Layer(void);
	virtual void Render_Layer(void);
	
public:
	HRESULT Add_GameObject(const wstring pObjTag, CGameObject* pObj);

private:
	MAP_GAMEOBJ m_mapObject;

public:
	static CLayer* Create(void);
	void Free(void);

};

NS_END
#endif // Layer_h__
