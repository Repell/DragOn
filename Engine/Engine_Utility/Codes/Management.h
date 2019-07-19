#ifndef Management_h__
#define Management_h__

#include "Scene.h"
#include "Renderer.h"
#include "Shader.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement(void);
	virtual		~CManagement(void);

public:
	CComponent*	Get_Component(const CLayer::LAYER_ID eLay,
								const wstring pObjTag,
								const wstring pComponentTag,
								COMPONENTID eID);
	CLayer* Get_Layer(CLayer::LAYER_ID eID)
	{		return m_pCurrentScene->Get_MapLayer(eID);	}
	
public:
	HRESULT		SetUp_CurrentScene(CScene* pCurrentScene);
	_int		Update_Scene(const _double& TimeDelta);
	void		Late_Update_Scene();
	void		Render_Scene(LPDIRECT3DDEVICE9 pDevice);

public:
	void Add_GameObject(ENGINE::CLayer::LAYER_ID eID, const wstring pObjTag, CGameObject* pObj);
	HRESULT		Ready_ShaderFile(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*		m_pCurrentScene;

public:
	virtual void	Free(void);

};

NS_END
#endif // Management_h__
