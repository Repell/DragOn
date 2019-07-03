#ifndef Management_h__
#define Management_h__

#include "Scene.h"
#include "Renderer.h"

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

public:
	HRESULT		SetUp_CurrentScene(CScene* pCurrentScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		Late_Update_Scene();
	void		Render_Scene(void);

private:
	CScene*		m_pCurrentScene;

public:
	virtual void	Free(void);

};

NS_END
#endif // Management_h__
