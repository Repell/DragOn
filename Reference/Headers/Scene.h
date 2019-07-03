#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CScene :
	public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDev);
	virtual ~CScene();

public:
	CLayer* Get_MapLayer(CLayer::LAYER_ID eID);
	CComponent*	Get_Component(const CLayer::LAYER_ID eLay,
		const wstring pObjTag,
		const wstring pComponentTag,
		COMPONENTID eID);

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Late_Update_Scene();
	virtual void Render_Scene();

protected:
	virtual void Free();

	virtual HRESULT Add_Environment_Layer() PURE;
	virtual HRESULT Add_GameObject_Layer() PURE;
	virtual HRESULT Add_UI_Layer() PURE;

public:
	typedef map<WORD, CLayer*> MAP_LAYER;

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	MAP_LAYER m_MapLayer;

};

NS_END

#endif // Scene_h__
