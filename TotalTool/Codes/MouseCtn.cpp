#include "stdafx.h"
#include "MouseCtn.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "TabNavi.h"
#include "TabAnimation.h"

#define RADIUS 1.f

CMouseCtn::CMouseCtn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pSprite(ENGINE::Get_GraphicDev()->Get_Sprite()),
	m_pLine(ENGINE::Get_GraphicDev()->Get_Line()),
	m_pCollMouse(nullptr), m_pVtxOrigin(nullptr), m_pVtxCopy(nullptr),
	bMakeLine(FALSE), wCount(0)
{
	ZeroMemory(&m_tNavi, sizeof(ENGINE::NAVI));
	m_pSprite->AddRef();
	m_pLine->AddRef();
}

CMouseCtn::~CMouseCtn()
{

}

HRESULT CMouseCtn::Ready_Object()
{
	Add_Component();

	return S_OK;
}

HRESULT CMouseCtn::Late_Init()
{
	Make_TerrainVertex(TERRAIN_VTX_X, TERRAIN_VTX_Z);

	return S_OK;
}

_int CMouseCtn::Update_Object(const _float& fTimeDelta)
{
	ENGINE::CGameObject::Late_Init();
	ENGINE::CGameObject::Update_Object(fTimeDelta);

	if (CValueMgr::bMakeLine && CValueMgr::eTab == 2)
		Make_NaviMesh();
	//Make_NaviMesh();


	return 0;
}

void CMouseCtn::Late_Update_Object()
{
	ENGINE::CGameObject::Late_Update_Object();
}

void CMouseCtn::Render_Object()
{
	Render_NaviMesh();

	if (CValueMgr::eTab == 2)
		Render_Font(&MousePos(&CValueMgr::vRatio));

	Render_Test();
}

HRESULT CMouseCtn::Add_Component()
{
	ENGINE::CComponent* pComponent = nullptr;

	pComponent = m_pCollMouse = CColl_Mouse::Create();
	NULL_CHECK_RETURN(m_pCollMouse, E_FAIL);
	m_MapComponent[ENGINE::COMP_STATIC].emplace(L"Com_Coll_Terrain", pComponent);

	return S_OK;
}

void CMouseCtn::Render_Font(const _vec2* pPos)
{
	_bool bRender;
	CValueMgr::vMouse = *(pPos);
	RECT rc = { _long(pPos->x), _long(pPos->y) + 25 };
	_tchar szStr[MAX_PATH] = L"";

	bRender = m_pCollMouse->PickTerrain(&m_vPickPos, m_pVtxOrigin);
	//bRender = Pick_Mesh(&m_vPickPos);

	switch (wCount)
	{
	case 0:
		swprintf_s(szStr, L" 1 :: %5.2f , %5.2f, %5.2f", m_vPickPos.x, m_vPickPos.y, m_vPickPos.z);
		break;
	case 1:
		swprintf_s(szStr, L" 2 :: %5.2f , %5.2f, %5.2f", m_vPickPos.x, m_vPickPos.y, m_vPickPos.z);
		break;
	case 2:
		swprintf_s(szStr, L" 3 :: %5.2f , %5.2f, %5.2f", m_vPickPos.x, m_vPickPos.y, m_vPickPos.z);
		break;
	}

	if (bRender)
		ENGINE::Render_Font(L"Sp", szStr, pPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	else
		m_vPickPos = { 0.f, 0.f, 0.f };

}

_vec2 CMouseCtn::MousePos(const _vec2* pMice)
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_tchar szStr[MAX_PATH] = L"";

	m_vMousePos = { _float(pt.x * pMice->x), _float(pt.y * pMice->y) };

	swprintf_s(szStr, L" Mice Pos :: %5.2f , %5.2f", m_vMousePos.x, m_vMousePos.y);

	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	return  m_vMousePos;
}

void CMouseCtn::Render_Test()
{
	_tchar szStr[MAX_PATH] = L"";

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pForm = dynamic_cast<CMyForm*>(pMain->Get_MainWnd().GetPane(0, 1));
	//CTabAnimation* pAni = dynamic_cast<CTabAnimation*>(pForm->m_pTabAnimation);
	map<wstring, _matrix*>& pMap = pForm->m_pTabAnimation.Get_mapBones();

	if (pMap.empty())
		return;

	for (auto iter : pMap)
	{
		_vec3 vTemp = {};
		memcpy(vTemp, &iter.second->m[3][0], sizeof(_vec3));
		swprintf_s(szStr, L" Matrix :: %5.2f , %5.2f, %5.2f", vTemp.x, vTemp.y, vTemp.z);
		ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
		
}

void CMouseCtn::Insert_TabNavi()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm* pForm = dynamic_cast<CMyForm*>(pMain->Get_MainWnd().GetPane(0, 1));
	//CTabNavi*pNavi = dynamic_cast<CTabNavi*>(pForm->m_pTabNavi);

	if (m_MapNavi.empty())
		return;

	//m_iCurNavi = pForm->m_pTabNavi.Insert_Navi();

}

void CMouseCtn::Delete_TabNavi(_int iNavi, _int iTri)
{
	auto& iter_find = m_MapLine.find(iNavi);

	if (iter_find == m_MapLine.end())
		return;

	m_MapLine.erase(iter_find);

	auto& iter_find2 = m_MapNavi.find(iNavi);

	if (iter_find2 == m_MapNavi.end())
		return;

	ENGINE::Safe_Release(iter_find2->second);
	m_MapNavi.erase(iter_find2);



}

void CMouseCtn::Load_NaviList(ENGINE::LINE_3D pLine, _int iCount)
{
	ENGINE::CTriTex* pBuffer = dynamic_cast<ENGINE::CTriTex*>(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_TriTex"));

	pBuffer->Set_Line(pLine);

	m_MapNavi.emplace(iCount, pBuffer);
	//m_vecLine.emplace_back(m_tLine);

	Insert_TabNavi();
}

ENGINE::LINE_3D CMouseCtn::Get_vecLine3D(_int iNavi)
{
	//return m_MapNavi[iNavi]->Get_3DLine();
	auto iter_find = m_MapNavi.find(iNavi);

	if (iter_find == m_MapNavi.end())
		return ENGINE::LINE_3D();

	return iter_find->second->Get_3DLine();

}

void CMouseCtn::Transform_NaviMesh(_int iNavi, _int iTri, _vec3 vPos)
{
	auto iter_find = m_MapNavi.find(iNavi);

	if (iter_find == m_MapNavi.end())
		return;

	ENGINE::LINE_3D pLine = iter_find->second->Get_3DLine();

	switch (iTri)
	{
	case 0:
		pLine.vLine_X[0] = vPos;
		break;
	case 1:
		pLine.vLine_Y[0] = vPos;
		break;
	case 2:
		pLine.vLine_Z[0] = vPos;
		break;
	}

	iter_find->second->Set_Line(pLine);


}

void CMouseCtn::Make_TerrainVertex(WORD iX, WORD iY)
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
	//memcpy(m_pVtxCopy, m_pVtxOrigin, sizeof(ENGINE::VTX_TEX) * TERRAIN_VTX_X * TERRAIN_VTX_Z);
}

void CMouseCtn::Make_Line()
{
	if (m_vMousePos.x > 0 && m_vMousePos.x < 1400)
	{

		if (ENGINE::Key_Down(ENGINE::dwKEY_RBUTTON))
		{
			wCount = 0;
			bRenderLine = false;
			for (int i = 0; i < 3; ++i)
				m_LinePos[i] = { 0.f, 0.f, 0.f };
		}

		if (wCount == 0 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_Navi())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurNavi_Tri)
				{
				case 0:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_X[0];
					break;
				case 1:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Y[0];
					break;
				case 2:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Z[0];
					break;
				}

				++wCount;
			}
			else if (Pick_Mesh(&m_LinePos[wCount]))
			{

				++wCount;
			}
		}

		if (wCount == 1 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_Navi())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurNavi_Tri)
				{
				case 0:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_X[0];
					break;
				case 1:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Y[0];
					break;
				case 2:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Z[0];
					break;
				}

				++wCount;
			}
			else if (Pick_Mesh(&m_LinePos[wCount]))
				++wCount;
		}

		if (wCount == 2 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_Navi())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurNavi_Tri)
				{
				case 0:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_X[0];
					break;
				case 1:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Y[0];
					break;
				case 2:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Z[0];
					break;
				}

				m_tLine.Set_LinePos_X(m_LinePos[0], m_LinePos[1]);
				m_tLine.Set_LinePos_Y(m_LinePos[1], m_LinePos[2]);
				m_tLine.Set_LinePos_Z(m_LinePos[2], m_LinePos[0]);

				ENGINE::CTriTex* pBuffer = dynamic_cast<ENGINE::CTriTex*>(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_TriTex"));
				//pBuffer->Create_Sphere();
				pBuffer->Set_Line(m_tLine);

				Insert_TabNavi();

				m_MapNavi.emplace(m_iCurNavi, pBuffer);
				m_MapLine.emplace(m_iCurNavi, m_tLine);
				ZeroMemory(&m_tLine, sizeof(ENGINE::LINE_3D));
				ZeroMemory(m_LinePos, sizeof(_vec3));


				wCount = 0;
			}
			else if (Pick_Mesh(&m_LinePos[wCount]))
			{
				m_tLine.Set_LinePos_X(m_LinePos[0], m_LinePos[1]);
				m_tLine.Set_LinePos_Y(m_LinePos[1], m_LinePos[2]);
				m_tLine.Set_LinePos_Z(m_LinePos[2], m_LinePos[0]);

				ENGINE::CTriTex* pBuffer = dynamic_cast<ENGINE::CTriTex*>(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_TriTex"));
				//pBuffer->Create_Sphere();
				pBuffer->Set_Line(m_tLine);

				m_MapNavi.emplace(m_iCurNavi, pBuffer);
				m_MapLine.emplace(m_iCurNavi, m_tLine);

				Insert_TabNavi();

				ZeroMemory(&m_tLine, sizeof(ENGINE::LINE_3D));
				ZeroMemory(m_LinePos, sizeof(_vec3));

				wCount = 0;
			}

		}

	}

}

void CMouseCtn::Make_NaviMesh()
{
	if (m_vMousePos.x > 0 && m_vMousePos.x < 1400)
	{

		if (ENGINE::Key_Down(ENGINE::dwKEY_RBUTTON))
		{
			wCount = 0;
			bRenderLine = false;
			for (int i = 0; i < 3; ++i)
				m_LinePos[i] = { 0.f, 0.f, 0.f };
		}

		if (wCount == 0 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_Navi())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurNavi_Tri)
				{
				case 0:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_X[0];
					break;
				case 1:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Y[0];
					break;
				case 2:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Z[0];
					break;
				}

				++wCount;
			}
			else if (m_pCollMouse->PickTerrain(&m_LinePos[wCount], m_pVtxOrigin))
				++wCount;
		}

		if (wCount == 1 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_Navi())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurNavi_Tri)
				{
				case 0:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_X[0];
					break;
				case 1:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Y[0];
					break;
				case 2:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Z[0];
					break;
				}

				++wCount;
			}
			else if (m_pCollMouse->PickTerrain(&m_LinePos[wCount], m_pVtxOrigin))
				++wCount;
		}

		if (wCount == 2 && ENGINE::Key_Down(ENGINE::dwKEY_LBUTTON))
		{
			if (Coll_Navi())
			{
				auto& iter_find = m_MapNavi.find(m_iCurNavi);

				if (iter_find == m_MapNavi.end())
					return;

				switch (m_iCurNavi_Tri)
				{
				case 0:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_X[0];
					break;
				case 1:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Y[0];
					break;
				case 2:
					m_LinePos[wCount] = iter_find->second->Get_3DLine().vLine_Z[0];
					break;
				}

				m_tLine.Set_LinePos_X(m_LinePos[0], m_LinePos[1]);
				m_tLine.Set_LinePos_Y(m_LinePos[1], m_LinePos[2]);
				m_tLine.Set_LinePos_Z(m_LinePos[2], m_LinePos[0]);

				ENGINE::CTriTex* pBuffer = dynamic_cast<ENGINE::CTriTex*>(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_TriTex"));
				//pBuffer->Create_Sphere();
				pBuffer->Set_Line(m_tLine);

				Insert_TabNavi();

				m_MapNavi.emplace(m_iCurNavi, pBuffer);
				m_MapLine.emplace(m_iCurNavi, m_tLine);
				ZeroMemory(&m_tLine, sizeof(ENGINE::LINE_3D));
				ZeroMemory(m_LinePos, sizeof(_vec3));

				wCount = 0;
			}
			else if (m_pCollMouse->PickTerrain(&m_LinePos[wCount], m_pVtxOrigin))
			{
				m_tLine.Set_LinePos_X(m_LinePos[0], m_LinePos[1]);
				m_tLine.Set_LinePos_Y(m_LinePos[1], m_LinePos[2]);
				m_tLine.Set_LinePos_Z(m_LinePos[2], m_LinePos[0]);

				ENGINE::CTriTex* pBuffer = dynamic_cast<ENGINE::CTriTex*>(ENGINE::Clone_Resources(RESOURCE_TOOL, L"Buffer_TriTex"));
				//pBuffer->Create_Sphere();
				pBuffer->Set_Line(m_tLine);

				m_MapNavi.emplace(m_iCurNavi, pBuffer);
				m_MapLine.emplace(m_iCurNavi, m_tLine);

				Insert_TabNavi();

				ZeroMemory(&m_tLine, sizeof(ENGINE::LINE_3D));
				ZeroMemory(m_LinePos, sizeof(_vec3));

				wCount = 0;
			}

		}


	}
}

_bool CMouseCtn::Pick_Mesh(_vec3 * pPos)
{
	_vec3 vNewMice;
	//윈도우 좌표에서 투영 공간으로 변환
	vNewMice.x = m_vMousePos.x / (WINCX * 0.5f) - 1.f;
	vNewMice.y = m_vMousePos.y / -(WINCY * 0.5f) + 1.f;
	vNewMice.z = 1.f;
	//투영에서 뷰 스페이스로 변환
	_matrix matProj;
	D3DXMatrixInverse(&matProj, NULL, &CValueMgr::matProj);
	D3DXVec3TransformCoord(&vNewMice, &vNewMice, &matProj);

	//뷰에서 월드로
	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vNewMice - vRayPos;

	_matrix matView;
	D3DXMatrixInverse(&matView, NULL, &CValueMgr::matView);

	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	///////////////
	ENGINE::LINE_3D vLine;
	vLine.Set_LinePos_X(_vec3(0.f, 0.f, 0.f), vRayDir);

	m_pLine->SetWidth(5.f);
	m_pLine->Begin();
	_matrix mat;
	D3DXMatrixIdentity(&mat);
	m_pLine->DrawTransform(vLine.vLine_X, 2, &CValueMgr::matTerrain, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pLine->End();


	/////////////////////////////////////

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Static_MeshObject");

	if (CValueMgr::bMeshState)
		return FALSE;

	auto& iter = pList.begin();

	for (int i = 0; i < CValueMgr::iCurIndex; ++i)
		++iter;

	if (iter == pList.end())
		return FALSE;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);
	LPDIRECT3DVERTEXBUFFER9		 pVB;
	LPDIRECT3DINDEXBUFFER9			pIB;

	_int iNum = pObj->Get_Static()->Get_Mesh()->GetNumFaces();
	_int iMaxVtx = pObj->Get_Static()->Get_Mesh()->GetNumVertices();
	//_matrix matWorld = pObj->Get_Matrix();

	//D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

	//D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	//D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	//D3DXVec3Normalize(&vRayDir, &vRayDir);

	pObj->Get_Static()->Get_Mesh()->GetVertexBuffer(&pVB);
	pObj->Get_Static()->Get_Mesh()->GetIndexBuffer(&pIB);

	ENGINE::VTX_TEX* pVertex = nullptr;
	pVB->Lock(0, 0, (void**)&pVertex, 0);


	WORD w[3] = {};
	WORD* wIndex;
	pIB->Lock(0, 0, (void**)&wIndex, 0);

	vector<_vec3> vecTemp;
	vector<_float> vecdist;

	_float fU, fV, fDist;
	for (int i = 0; i < iNum; ++i)
	{
		w[0] = wIndex[3 * i + 0];
		w[1] = wIndex[3 * i + 1];
		w[2] = wIndex[3 * i + 2];

		if (D3DXIntersectTri(&pVertex[w[0]].vPos, &pVertex[w[1]].vPos, &pVertex[w[2]].vPos,
			&vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			//*pPos = pVertex[w[0]].vPos +
			//	fU * (pVertex[w[1]].vPos - pVertex[w[0]].vPos) +
			//	fV * (pVertex[w[2]].vPos - pVertex[w[0]].vPos);

			//D3DXVec3TransformCoord(pPos, pPos, &matWorld);
			_vec3 vTemp = pVertex[w[0]].vPos +
				fU * (pVertex[w[1]].vPos - pVertex[w[0]].vPos) +
				fV * (pVertex[w[2]].vPos - pVertex[w[0]].vPos);

			vecTemp.emplace_back(vTemp);
			vecdist.emplace_back(fDist);

			pVB->Unlock();
			pIB->Unlock();

		}

	}
	_int i = 0;
	_int idx = 0;
	_float Dist = 0;

	if (!vecdist.empty())
		Dist = vecdist[0];

	for (auto v : vecdist)
	{
		if (Dist > v)
		{
			Dist = v;
			idx = i;
		}

		++i;
	}

	_tchar szStr[MAX_PATH] = L"";
	swprintf_s(szStr, L" %5.2f , %5.2f, %5.2f", vecTemp[idx].x, vecTemp[idx].y, vecTemp[idx].z);

	ENGINE::Render_Font(L"Sp", szStr, &_vec2(10.f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	*pPos = vecTemp[idx];

	pVB->Unlock();
	pIB->Unlock();

	return TRUE;
}

void CMouseCtn::Render_NaviMesh()
{
	_matrix matLine;
	D3DXMatrixIdentity(&matLine);
	matLine = CValueMgr::matView * CValueMgr::matProj;

	auto& iter = m_MapNavi.begin();

	if (iter == m_MapNavi.end())
		return;

	for (; iter != m_MapNavi.end(); ++iter)
	{
		iter->second->Set_World(&CValueMgr::matTerrain);
		iter->second->Render_Buffer();
	}


	//if (bRenderLine)
	//{
	//	//ENGINE::LINE_3D vLine(m_LinePos[0], m_LinePos[1]);
	//	m_tLine.Set_LinePos_X(m_LinePos[0], m_LinePos[1]);
	//	m_tLine.Set_LinePos_Y(m_LinePos[1], m_LinePos[2]);
	//	m_tLine.Set_LinePos_Z(m_LinePos[2], m_LinePos[0]);

	//	m_pLine->SetWidth(5.f);
	//	m_pLine->Begin();

	//	m_pLine->DrawTransform(m_tLine.vLine_X, 2, &matLine, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	//	m_pLine->DrawTransform(m_tLine.vLine_Y, 2, &matLine, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	//	m_pLine->DrawTransform(m_tLine.vLine_Z, 2, &matLine, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	//	m_pLine->End();
	//}
}

_bool CMouseCtn::Coll_Navi()
{

	if (m_MapNavi.empty())
		return false;

	_int i = 0;

	for (auto& line : m_MapNavi)
	{
		if (Coll_Sphere(line.second->Get_3DLine()))
		{
			m_iCurNavi = i;

			return TRUE;
		}

		++i;
	}

	return false;
}

_bool CMouseCtn::Coll_Sphere(ENGINE::LINE_3D pLine)
{
	float fDist = 0.f;
	D3DXVECTOR3 vDiff;

	vDiff = m_vPickPos - pLine.vLine_X[0];
	fDist = D3DXVec3Length(&vDiff);

	if (fDist <= (RADIUS + RADIUS))
	{
		m_iCurNavi_Tri = 0;
		return true;
	}

	vDiff = m_vPickPos - pLine.vLine_Y[0];
	fDist = D3DXVec3Length(&vDiff);

	if (fDist <= (RADIUS + RADIUS))
	{
		m_iCurNavi_Tri = 1;
		return true;
	}

	vDiff = m_vPickPos - pLine.vLine_Z[0];
	fDist = D3DXVec3Length(&vDiff);

	if (fDist <= (RADIUS + RADIUS))
	{
		m_iCurNavi_Tri = 2;
		return true;
	}

	return FALSE;
}

CMouseCtn * CMouseCtn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMouseCtn*	pInstance = new CMouseCtn(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		ENGINE::Safe_Delete(pInstance);
		return nullptr;
	}


	return	pInstance;
}

void CMouseCtn::Free()
{
	ENGINE::Safe_Release(m_pSprite);
	ENGINE::Safe_Release(m_pLine);
	ENGINE::Safe_Delete_Array(m_pVtxOrigin);

	m_MapLine.clear();
	for_each(m_MapNavi.begin(), m_MapNavi.end(), [](auto& MyPair)
	{
		ENGINE::Safe_Release(MyPair.second);
		//MyPair.second->Free();
	});
	m_MapNavi.clear();

	ENGINE::CGameObject::Free();
}
