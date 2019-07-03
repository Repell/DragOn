#include "Renderer.h"

USING(ENGINE)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject * pObj)
{
	if (RENDER_END <= eGroup)
		return;

	m_pRenderlist[eGroup].emplace_back(pObj);

	pObj->AddRef();
}

void CRenderer::Render_GameObject()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto iter : m_pRenderlist[i])
		{
			iter->Render_Object();
			Safe_Release(iter);
		}
		m_pRenderlist[i].clear();
		//렌더링 소팅을 위해 렌더 후 릴리즈 앤 클리어
	}

}

void CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_pRenderlist[i].begin(), m_pRenderlist[i].end(), CDeleteObj());
		m_pRenderlist[i].clear();
	}
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}

