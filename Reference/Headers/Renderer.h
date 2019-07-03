#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Define.h"
#include "GameObject.h"

NS_BEGIN(ENGINE)

class ENGINE_DLL CRenderer :
	public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void Add_RenderGroup(RENDERID eGroup, CGameObject* pObj);
	void Render_GameObject();
	void Clear_RenderGroup();

private:
	list<CGameObject*>	m_pRenderlist[RENDER_END];
	LPDIRECT3DDEVICE9	m_pGraphicDev;

private:
	virtual void Free();

};

NS_END
#endif // Renderer_h__
