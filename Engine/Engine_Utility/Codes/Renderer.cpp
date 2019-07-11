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

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pDevice)
{
	//for (_uint i = 0; i < RENDER_END; ++i)
	//{
	//	for (auto iter : m_pRenderlist[i])
	//	{
	//		iter->Render_Object();
	//		Safe_Release(iter);
	//	}
	//	m_pRenderlist[i].clear();
	//	//렌더링 소팅을 위해 렌더 후 릴리즈 앤 클리어
	//}

	m_pGraphicDev = pDevice;
	m_pGraphicDev->AddRef();

	Render_Priority();
	Render_NonAlpha();	//서순 중요 nonAlpha -> Alpha
	Render_Alpha();
	Render_UI();
	//Render_ReSet();

	Clear_RenderGroup();
	Safe_Release(m_pGraphicDev);
}

void CRenderer::Clear_RenderGroup()
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_pRenderlist[i].begin(), m_pRenderlist[i].end(), CDeleteObj());
		m_pRenderlist[i].clear();
	}
}

void CRenderer::Render_Priority()
{
	//SkyBox, Effect, Logo
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_pRenderlist[RENDER_PRIORITY])
		iter->Render_Object();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_NonAlpha()
{
	for (auto& iter : m_pRenderlist[RENDER_NONALPHA])
		iter->Render_Object();
}

_bool Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

void CRenderer::Render_Alpha()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pRenderlist[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_pRenderlist[RENDER_ALPHA])
		iter->Render_Object();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI()
{
	//UI
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_pRenderlist[RENDER_UI])
		iter->Render_Object();

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_ReSet()
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRenderer::Free()
{
	//Safe_Release(m_pGraphicDev);

	Clear_RenderGroup();
}

