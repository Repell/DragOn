#include "stdafx.h"
#include "NaviMaker.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "TabNavi.h"

#define RADIUS 1.5f

CNaviMaker::CNaviMaker(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pSprite(ENGINE::Get_GraphicDev()->Get_Sprite()),
	m_pLine(ENGINE::Get_GraphicDev()->Get_Line()),
	m_pCollMouse(nullptr), m_pVtxOrigin(nullptr)
{
	ZeroMemory(&m_tNavi, sizeof(ENGINE::NAVI));
	m_pSprite->AddRef();
	m_pLine->AddRef();
}

CNaviMaker::~CNaviMaker()
{

}

void CNaviMaker::Transform_NaviMesh(_int iNavi, _int iTri, _vec3 vPos)
{
	auto iter_find = m_MapNavi.find(iNavi);

	if (iter_find == m_MapNavi.end())
		return;

	switch (iTri)
	{
	case 0:
		iter_find->second.vPointA = vPos;
		break;
	case 1:
		iter_find->second.vPointB = vPos;
		break;
	case 2:
		iter_find->second.vPointC = vPos;
		break;
	}

}

ENGINE::NAVI CNaviMaker::Get_Navi(_int iNavi)
{
	auto iter = m_MapNavi.find(iNavi);

	if (iter == m_MapNavi.end())
		return ENGINE::NAVI();



	return iter->second;
}

map<_int, ENGINE::NAVI>& CNaviMaker::Get_MapNavi()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_MapNavi;
}

void CNaviMaker::Insert_NaviTree()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pForm = dynamic_cast<CMyForm*>(pMain->Get_MainWnd().GetPane(0, 1));

	if (m_MapNavi.empty())
		return;

	pForm->m_pTabNavi.Insert_Navi(CValueMgr::iNaviCount);
}

void CNaviMaker::Delete_MapNavi(_int iNavi, _int iTri)
{
	auto& iter_find = m_MapNavi.find(iNavi);

	if (iter_find == m_MapNavi.end())
		return;

	m_MapNavi.erase(iter_find);

}

void CNaviMaker::Clear_MapNavi()
{

	if (m_MapNavi.empty())
		return;

	m_MapNavi.clear();
}

HRESULT CNaviMaker::Ready_Object()
{
	Add_Component();

	return S_OK;
}

HRESULT CNaviMaker::Late_Init()
{
	Make_TerrainVertex(TERRAIN_VTX_X, TERRAIN_VTX_Z);

	return S_OK;
}

_int CNaviMaker::Update_Object(const _float& fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	if (CValueMgr::bMakeLine && CValueMgr::eTab == 2)
		Make_NaviMesh();



	return 0;
}

void CNaviMaker::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();

	if (!CValueMgr::bMakeLine)
	{
		wCount = 0;
		ZeroMemory(&m_tNavi, sizeof(ENGINE::NAVI));
	}

	if (m_MapNavi.empty())
		CValueMgr::iNaviCount = 0;

}

void CNaviMaker::Render_Object()
{
	Render_Navi();

	if (CValueMgr::eTab == 2)
		Render_Font(&MousePos(&CValueMgr::vRatio));
}

HRESULT CNaviMaker::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;

	pComponent = m_pCollMouse = CColl_Mouse::Create();
	NULL_CHECK_RETURN(m_pCollMouse, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Coll_Terrain", pComponent);

	return S_OK;
}

void CNaviMaker::Render_Font(const _vec2 * pPos)
{
	_bool bRender = FALSE;
	CValueMgr::vMouse = *(pPos);
	RECT rc = { _long(pPos->x), _long(pPos->y + 40) };
	_tchar szStr[MAX_PATH] = L"";

	bRender = m_pCollMouse->PickTerrain(&m_vPickPos, m_pVtxOrigin);

	swprintf_s(szStr, L"%5.2f , %5.2f , %5.2f", m_vPickPos.x, m_vPickPos.y, m_vPickPos.z);

	if (bRender)
		ENGINE::Render_Font(L"Sp", szStr, pPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	else
		m_vPickPos = { 0.f, 0.f, 0.f };

}

_vec2 CNaviMaker::MousePos(const _vec2 * pMice)
{
	POINT pt = {};
	//_tchar szStr[MAX_PATH] = L"";

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_vMousePos = { _float(pt.x * pMice->x), _float(pt.y * pMice->y) };

	//swprintf_s(szStr, L" Mice Pos :: %5.2f , %5.2f", m_vMousePos.x, m_vMousePos.y);
	//ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	return m_vMousePos;
}

void CNaviMaker::Make_TerrainVertex(WORD iX, WORD iY)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());
	ENGINE::CViBuffer* pBuffer = dynamic_cast<ENGINE::CViBuffer*>(pTerrain->Get_TerrainTex());

	if (m_pVtxOrigin == nullptr)
	{
		m_pVtxOrigin = new ENGINE::VTX_TEX[iX * iY];
		pBuffer->Get_VtxInfo(m_pVtxOrigin);
	}
	else if (m_pVtxOrigin != nullptr)
	{
		ENGINE::Safe_Delete_Array(m_pVtxOrigin);

		m_pVtxOrigin = new ENGINE::VTX_TEX[iX * iY];
		pBuffer->Get_VtxInfo(m_pVtxOrigin);
	}
}

void CNaviMaker::Make_NaviMesh()
{
	//Reset NaviList

	if (ENGINE::Key_Down(ENGINE::dwKEY_RBUTTON))
	{
		wCount = 0;
		ZeroMemory(&m_tNavi, sizeof(ENGINE::NAVI));
	}


	if (m_vMousePos.x > 10 && m_vMousePos.x < 1270)
	{

		//Make Navi -> Point A
		if (wCount == 0 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_NaviPoint())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurPoint)
				{
				case 0:
					m_tNavi.vPointA = iter_find->second.vPointA;
					break;
				case 1:
					m_tNavi.vPointA = iter_find->second.vPointB;
					break;
				case 2:
					m_tNavi.vPointA = iter_find->second.vPointC;
					break;
				}

				wCount++;
			}
			else if (m_pCollMouse->PickTerrain(&m_tNavi.vPointA, m_pVtxOrigin))
				wCount++;
		}

		//Make Navi -> Point B
		if (wCount == 1 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_NaviPoint())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurPoint)
				{
				case 0:
					m_tNavi.vPointB = iter_find->second.vPointA;
					break;
				case 1:
					m_tNavi.vPointB = iter_find->second.vPointB;
					break;
				case 2:
					m_tNavi.vPointB = iter_find->second.vPointC;
					break;
				}

				wCount++;
			}
			else if (m_pCollMouse->PickTerrain(&m_tNavi.vPointB, m_pVtxOrigin))
				wCount++;
		}

		//Make Navi -> Point C & Insert Map
		if (wCount == 2 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_NaviPoint())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurPoint)
				{
				case 0:
					m_tNavi.vPointC = iter_find->second.vPointA;
					break;
				case 1:
					m_tNavi.vPointC = iter_find->second.vPointB;
					break;
				case 2:
					m_tNavi.vPointC = iter_find->second.vPointC;
					break;
				}

				m_tNavi.Index = CValueMgr::iNaviCount;
				m_MapNavi.emplace(CValueMgr::iNaviCount, m_tNavi);
				Insert_NaviTree();
				++CValueMgr::iNaviCount;

				ZeroMemory(&m_tNavi, sizeof(ENGINE::NAVI));
			}
			else if (m_pCollMouse->PickTerrain(&m_tNavi.vPointC, m_pVtxOrigin))
			{
				m_tNavi.Index = CValueMgr::iNaviCount;
				m_MapNavi.emplace(CValueMgr::iNaviCount, m_tNavi);
				Insert_NaviTree();
				++CValueMgr::iNaviCount;

				ZeroMemory(&m_tNavi, sizeof(ENGINE::NAVI));
			}

			wCount = 0;
		}

	}


}

void CNaviMaker::Render_Navi()
{
	_matrix matLine;
	D3DXMatrixIdentity(&matLine);
	matLine = CValueMgr::matView * CValueMgr::matProj;

	auto& iter = m_MapNavi.begin();

	if (iter == m_MapNavi.end())
		return;

	for (; iter != m_MapNavi.end(); ++iter)
	{
		ENGINE::LINE_3D vLine;
		vLine.Set_LinePos_X(iter->second.vPointA, iter->second.vPointB);
		vLine.Set_LinePos_Y(iter->second.vPointB, iter->second.vPointC);
		vLine.Set_LinePos_Z(iter->second.vPointC, iter->second.vPointA);

		m_pLine->SetWidth(5.f);
		m_pLine->Begin();

		m_pLine->DrawTransform(vLine.vLine_X, 2, &matLine, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		m_pLine->DrawTransform(vLine.vLine_Y, 2, &matLine, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		m_pLine->DrawTransform(vLine.vLine_Z, 2, &matLine, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

		m_pLine->End();
	}

}

_bool CNaviMaker::Coll_NaviPoint()
{
	auto& iter = m_MapNavi.begin();

	if (iter == m_MapNavi.end())
		return FALSE;

	for (; iter != m_MapNavi.end(); ++iter)
	{
		float fDist = 0.f;
		D3DXVECTOR3 vDiff;

		vDiff = m_vPickPos - iter->second.vPointA;
		fDist = D3DXVec3Length(&vDiff);

		if (fDist <= (RADIUS + RADIUS))
		{
			m_iCurPoint = 0;
			m_iCurNavi = iter->first;
			return TRUE;
		}

		vDiff = m_vPickPos - iter->second.vPointB;
		fDist = D3DXVec3Length(&vDiff);

		if (fDist <= (RADIUS + RADIUS))
		{
			m_iCurPoint = 1;
			m_iCurNavi = iter->first;
			return TRUE;
		}

		vDiff = m_vPickPos - iter->second.vPointC;
		fDist = D3DXVec3Length(&vDiff);

		if (fDist <= (RADIUS + RADIUS))
		{
			m_iCurPoint = 2;
			m_iCurNavi = iter->first;
			return TRUE;
		}

	}

	return FALSE;
}

_bool CNaviMaker::Coll_Terrain(ENGINE::_vec3 * pPos)
{


	return FALSE;
}

CNaviMaker * CNaviMaker::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMaker*	pInstance = new CNaviMaker(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		ENGINE::Safe_Delete(pInstance);
		return nullptr;
	}


	return	pInstance;
}

void CNaviMaker::Free()
{
	ENGINE::Safe_Release(m_pSprite);
	ENGINE::Safe_Release(m_pLine);
	ENGINE::Safe_Delete_Array(m_pVtxOrigin);

	m_MapNavi.clear();

	ENGINE::CGameObject::Free();
}
