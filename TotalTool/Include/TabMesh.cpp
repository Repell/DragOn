// TabMesh.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "TabMesh.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "TotalToolView.h"
#include "ToolRender.h"
#include "Terrain.h"
#include "TestObj.h"

// CTabMesh 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabMesh, CDialogEx)

CTabMesh::CTabMesh(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TABTWO, pParent)
	, m_strCurrentMesh(L"")
	, m_strPosX(_T(""))
	, m_strPosY(_T(""))
	, m_strPosZ(_T(""))
	, m_strRotX(_T(""))
	, m_strRotY(_T(""))
	, m_strRotZ(_T(""))
	, m_strScaleX(_T(""))
	, m_strScaleY(_T(""))
	, m_strScaleZ(_T(""))
{

}

CTabMesh::~CTabMesh()
{
	//for_each(m_MeshList.begin(), m_MeshList.end(), ENGINE::Safe_Delete<ENGINE::MESH_INFO*>);

	for (int i = 0; i < 3; ++i)
	{
		for_each(m_mapMesh[i].begin(), m_mapMesh[i].end(), [](auto& myPair)
		{
			ENGINE::Safe_Delete(myPair.second);
		});
		m_mapMesh[i].clear();
	}
}

void CTabMesh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeList);
	DDX_Control(pDX, IDC_RADIO1, m_RenderRadio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_RenderRadio[1]);
	DDX_Control(pDX, IDC_LIST1, m_staticBox);
	DDX_Control(pDX, IDC_LIST2, m_DynamicBox);
	DDX_Text(pDX, IDC_EDIT_SPIN5, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_SPIN6, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_SPIN7, m_strPosZ);
	DDX_Text(pDX, IDC_EDIT_SPIN8, m_strRotX);
	DDX_Text(pDX, IDC_EDIT_SPIN9, m_strRotY);
	DDX_Text(pDX, IDC_EDIT_SPIN10, m_strRotZ);
	DDX_Text(pDX, IDC_EDIT_SPIN11, m_strScaleX);
	DDX_Text(pDX, IDC_EDIT_SPIN12, m_strScaleY);
	DDX_Text(pDX, IDC_EDIT_SPIN13, m_strScaleZ);
	DDX_Control(pDX, IDC_EDIT1, m_MeshType);
	DDX_Control(pDX, IDC_EDIT2, m_MeshName);
}


BEGIN_MESSAGE_MAP(CTabMesh, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CTabMesh::OnBnClicked_RenderSolid)
	ON_BN_CLICKED(IDC_RADIO2, &CTabMesh::OnBnClicked_RenderWireFrm)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTabMesh::OnTvnSelchanged_Item)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabMesh::OnBnClicked_AddMesh)
	ON_BN_CLICKED(IDC_BUTTON2, &CTabMesh::OnBnClicked_DeleteStatic)
	ON_BN_CLICKED(IDC_BUTTON5, &CTabMesh::OnBnClicked_AddDynamic)
	ON_BN_CLICKED(IDC_BUTTON3, &CTabMesh::OnBnClicked_DeleteDynamic)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTabMesh::OnDeltaposSpin_PosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTabMesh::OnDeltaposSpin_PosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTabMesh::OnDeltaposSpin_PosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTabMesh::OnDeltaposSpin_RotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CTabMesh::OnDeltaposSpin_RotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CTabMesh::OnDeltaposSpin_RotZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CTabMesh::OnDeltaposSpin_ScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CTabMesh::OnDeltaposSpin_ScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN13, &CTabMesh::OnDeltaposSpin_ScaleZ)
	ON_EN_CHANGE(IDC_EDIT_SPIN5, &CTabMesh::OnEnChangeEdit_PosX)
	ON_EN_CHANGE(IDC_EDIT_SPIN6, &CTabMesh::OnEnChangeEdit_PosY)
	ON_EN_CHANGE(IDC_EDIT_SPIN7, &CTabMesh::OnEnChangeEdit_PosZ)
	ON_EN_CHANGE(IDC_EDIT_SPIN8, &CTabMesh::OnEnChangeEdit_RotX)
	ON_EN_CHANGE(IDC_EDIT_SPIN9, &CTabMesh::OnEnChangeEdit_RotY)
	ON_EN_CHANGE(IDC_EDIT_SPIN10, &CTabMesh::OnEnChangeEdit_RotZ)
	ON_EN_CHANGE(IDC_EDIT_SPIN11, &CTabMesh::OnEnChangeEdit_ScaleX)
	ON_EN_CHANGE(IDC_EDIT_SPIN12, &CTabMesh::OnEnChangeEdit_ScaleY)
	ON_EN_CHANGE(IDC_EDIT_SPIN13, &CTabMesh::OnEnChangeEdit_ScaleZ)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTabMesh::OnLbnSelchange_PickStaticObj)
	ON_LBN_SELCHANGE(IDC_LIST2, &CTabMesh::OnLbnSelchange_PickDynamicObj)
	ON_BN_CLICKED(IDC_BUTTON6, &CTabMesh::OnBnClicked_SaveMesh)
	ON_BN_CLICKED(IDC_BUTTON7, &CTabMesh::OnBnClicked_LoadMesh)
	ON_BN_CLICKED(IDC_BUTTON8, &CTabMesh::OnBnClicked_SaveMesh_Dynamic)
	ON_BN_CLICKED(IDC_BUTTON9, &CTabMesh::OnBnClicked_LoadMesh_Dynamic)
END_MESSAGE_MAP()


// CTabMesh 메시지 처리기입니다.


BOOL CTabMesh::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_iIndex = 0;
	bMeshSwitch = false;
	m_RenderRadio[0].SetCheck(TRUE);
	CValueMgr::bWireFrm_Obj = FALSE;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(TRUE);

	//Import Mesh Folder
	ImportMeshFolder();

	//Mesh Transform Init
	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;
	m_fRotX = 0.f;
	m_fRotY = 0.f;
	m_fRotZ = 0.f;
	m_fScaleX = 1.f;
	m_fScaleY = 1.f;
	m_fScaleZ = 1.f;

	m_strPosX.Format(_T("%5.3f", m_fPosX));
	m_strPosY.Format(_T("%5.3f", m_fPosY));
	m_strPosZ.Format(_T("%5.3f", m_fPosZ));
	m_strRotX.Format(_T("%5.3f", m_fRotX));
	m_strRotY.Format(_T("%5.3f", m_fRotY));
	m_strRotZ.Format(_T("%5.3f", m_fRotZ));
	m_strScaleX.Format(_T("%5.3f", m_fScaleX));
	m_strScaleY.Format(_T("%5.3f", m_fScaleY));
	m_strScaleZ.Format(_T("%5.3f", m_fScaleZ));


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTabMesh::ImportMeshFolder()
{
	CString OriginPath = L"../../Client/Bin/Resources/Mesh/StaticMesh";
	CString OriginTemp = OriginPath;

	//경로 내 파일 탐색
	BOOL bWorking = Finder.FindFile(OriginTemp);

	//Level 1 :: Mesh
	while (bWorking)
	{
		bWorking = Finder.FindNextFileW();
		CString strTemp = Finder.GetFileName();

		if (Finder.IsSystem())
			continue;

		if (strTemp == _T(".") || strTemp == _T("..") || strTemp == _T("Thumbs.db"))
			continue;

		if (Finder.IsDirectory())
		{
			hLevel1 = m_TreeList.InsertItem(strTemp, 0, 0);
			m_TreeList.Expand(hLevel1, TVE_EXPAND);
			CFileInfo::DirInfoExtract(OriginTemp.GetString(), m_mapMesh, &hLevel1, &m_TreeList);
		}
	}

}

void CTabMesh::Release_MapMesh(_ushort eType)
{
	//for_each(m_mapMesh[eType].begin(), m_mapMesh[eType].end(), [=](auto& myPair)
	//{
	//	ENGINE::Safe_Delete(myPair.second);
	//});

	CString strKey = L"";
	switch (eType)
	{
	case 0 :
		strKey = L"Static_MeshObject";
		break;
	case 1:
		strKey = L"Dynamic_MeshObject";
		break;
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	pRender->Delete_GameObject(strKey.GetString());
	
}

void CTabMesh::Update_Transform()
{

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_strPosX.Format(_T("%5.3f"), m_fPosX);
	m_strPosY.Format(_T("%5.3f"), m_fPosY);
	m_strPosZ.Format(_T("%5.3f"), m_fPosZ);
	m_strRotX.Format(_T("%5.3f"), m_fRotX);
	m_strRotY.Format(_T("%5.3f"), m_fRotY);
	m_strRotZ.Format(_T("%5.3f"), m_fRotZ);
	m_strScaleX.Format(_T("%5.3f"), m_fScaleX);
	m_strScaleY.Format(_T("%5.3f"), m_fScaleY);
	m_strScaleZ.Format(_T("%5.3f"), m_fScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
}


void CTabMesh::OnBnClicked_RenderSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CValueMgr::bWireFrm_Obj = FALSE;
}


void CTabMesh::OnBnClicked_RenderWireFrm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CValueMgr::bWireFrm_Obj = TRUE;
}


void CTabMesh::OnTvnSelchanged_Item(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_strCurrentMesh = m_TreeList.GetItemText(pNMTreeView->itemNew.hItem);


	for (int i = 0; i < 3; ++i)
	{
		auto iter_find = m_mapMesh[i].find(m_strCurrentMesh.GetString());

		if (iter_find == m_mapMesh[i].end())
			continue;

	}

	//if (iter_find == m_mapMesh.end())
	//{
	//	m_strCurrentMesh = L"";
	//	return;
	//}

	*pResult = 0;
}

BOOL CTabMesh::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CTabMesh::OnBnClicked_AddMesh()		//Static
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto iter_find = m_mapMesh[0].find(m_strCurrentMesh.GetString());

	if (iter_find == m_mapMesh[0].end())
		return;

	bMeshSwitch = false;
	CValueMgr::bMeshState = bMeshSwitch;
	wstring strKey = L"";
	if (bMeshSwitch)
		strKey = L"Dynamic_MeshObject";
	else if (!bMeshSwitch)
		strKey = L"Static_MeshObject";

	//if (LB_ERR != m_staticBox.FindString(-1, iter_find->second->pFileName.c_str()))		//중복 방지
	m_staticBox.AddString(iter_find->second->pFileName.c_str());

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	pRender->Add_GameObject(iter_find->second->pMeshTag, strKey);

	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_DeleteStatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iIndex = m_staticBox.GetCurSel();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Static_MeshObject");

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);
	pObj->Set_DeleteObj();

	m_staticBox.DeleteString(m_iIndex);
	m_strCurrentMesh = L"";

	m_staticBox.SetCurSel(--m_iIndex);

	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_AddDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto iter_find = m_mapMesh[1].find(m_strCurrentMesh.GetString());

	if (iter_find == m_mapMesh[1].end())
		return;

	bMeshSwitch = true;
	CValueMgr::bMeshState = bMeshSwitch;
	wstring strKey = L"";
	if (bMeshSwitch)
		strKey = L"Dynamic_MeshObject";
	else if (!bMeshSwitch)
		strKey = L"Static_MeshObject";

	m_DynamicBox.AddString(iter_find->second->pFileName.c_str());

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	pRender->Add_GameObject(iter_find->second->pMeshTag, strKey);

	m_strCurrentMesh = L"";
	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_DeleteDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bMeshSwitch = true;
	m_iIndex = m_DynamicBox.GetCurSel();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_MeshObject");

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);
	pObj->Set_DeleteObj();

	m_DynamicBox.DeleteString(m_iIndex);

	m_strCurrentMesh = L"";

	m_DynamicBox.SetCurSel(--m_iIndex);
	UpdateData(FALSE);
}


void CTabMesh::OnDeltaposSpin_PosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosX += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosX -= 1.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosY += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosY -= 1.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosZ += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosZ -= 1.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_RotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fRotX += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fRotX -= 1.f;

	if (m_fRotX < 0.f)
		m_fRotX = 359.f;
	else if (m_fRotX > 359.f)
		m_fRotX = 0.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_RotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fRotY += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fRotY -= 1.f;

	if (m_fRotY < 0.f)
		m_fRotY = 359.f;
	else if (m_fRotY > 359.f)
		m_fRotY = 0.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_RotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fRotZ += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fRotZ -= 1.f;

	if (m_fRotZ < 0.f)
		m_fRotZ = 359.f;
	else if (m_fRotZ > 359.f)
		m_fRotZ = 0.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_ScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fScaleX += 0.01f;
	else if (pNMUpDown->iDelta > 0)
		m_fScaleX -= 0.01f;

	if (m_fScaleX < 0.01f)
		m_fScaleX = 0.01f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_ScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fScaleY += 0.01f;
	else if (pNMUpDown->iDelta > 0)
		m_fScaleY -= 0.01f;

	if (m_fScaleY < 0.01f)
		m_fScaleY = 0.01f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnDeltaposSpin_ScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fScaleZ += 0.01f;
	else if (pNMUpDown->iDelta > 0)
		m_fScaleZ -= 0.01f;

	if (m_fScaleZ < 0.01f)
		m_fScaleZ = 0.01f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabMesh::OnEnChangeEdit_PosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	//m_strPosX.Format(_T("%5.3f"), m_fPosX);
	//m_strPosY.Format(_T("%5.3f"), m_fPosY);
	//m_strPosZ.Format(_T("%5.3f"), m_fPosZ);

	m_fPosX = _wtof(m_strPosX);
	m_fPosY = _wtof(m_strPosY);
	m_fPosZ = _wtof(m_strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_PosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fPosX = _wtof(m_strPosX);
	m_fPosY = _wtof(m_strPosY);
	m_fPosZ = _wtof(m_strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_PosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fPosX = _wtof(m_strPosX);
	m_fPosY = _wtof(m_strPosY);
	m_fPosZ = _wtof(m_strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_RotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fRotX = _wtof(m_strRotX);
	m_fRotY = _wtof(m_strRotY);
	m_fRotZ = _wtof(m_strRotZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_RotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fRotX = _wtof(m_strRotX);
	m_fRotY = _wtof(m_strRotY);
	m_fRotZ = _wtof(m_strRotZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_RotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fRotX = _wtof(m_strRotX);
	m_fRotY = _wtof(m_strRotY);
	m_fRotZ = _wtof(m_strRotZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_ScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fScaleX = _wtof(m_strScaleX);
	m_fScaleY = _wtof(m_strScaleY);
	m_fScaleZ = _wtof(m_strScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_ScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fScaleX = _wtof(m_strScaleX);
	m_fScaleY = _wtof(m_strScaleY);
	m_fScaleZ = _wtof(m_strScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnEnChangeEdit_ScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	wstring strKey = L"";
	if (bMeshSwitch)
	{
		strKey = L"Dynamic_MeshObject";
		m_iIndex = m_DynamicBox.GetCurSel();
	}
	else if (!bMeshSwitch)
	{
		strKey = L"Static_MeshObject";
		m_iIndex = m_staticBox.GetCurSel();
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strKey);

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	m_fScaleX = _wtof(m_strScaleX);
	m_fScaleY = _wtof(m_strScaleY);
	m_fScaleZ = _wtof(m_strScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { m_fRotX, m_fRotY, m_fRotZ };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	UpdateData(FALSE);
}


void CTabMesh::OnLbnSelchange_PickStaticObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bMeshSwitch = false;
	m_iIndex = m_staticBox.GetCurSel();

	m_MeshType.SetWindowTextW(L"Static Mesh");
	CString strTemp = L"";
	m_staticBox.GetText(m_iIndex, strTemp);
	m_MeshName.SetWindowTextW(strTemp);

	CValueMgr::bMeshState = bMeshSwitch;
	CValueMgr::iCurIndex = m_iIndex;
	CValueMgr::strCurObj = strTemp;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Static_MeshObject");

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);
	_vec3 vPos = pObj->Get_vPos();
	_vec3 vRot = pObj->Get_vRot();
	_vec3 vScale = pObj->Get_vScale();
	//pObj->Set_DeleteObj();

	m_fPosX = vPos.x;
	m_strPosX.Format(_T("%5.3f"), m_fPosX);
	m_fPosY = vPos.y;
	m_strPosY.Format(_T("%5.3f"), m_fPosY);
	m_fPosZ = vPos.z;
	m_strPosZ.Format(_T("%5.3f"), m_fPosZ);

	m_fRotX = vRot.x;
	m_strRotX.Format(_T("%5.3f"), m_fRotX);
	m_fRotY = vRot.y;
	m_strRotY.Format(_T("%5.3f"), m_fRotY);
	m_fRotZ = vRot.z;
	m_strRotZ.Format(_T("%5.3f"), m_fRotZ);

	m_fScaleX = vScale.x;
	m_strScaleX.Format(_T("%5.3f"), m_fScaleX);
	m_fScaleY = vScale.y;
	m_strScaleY.Format(_T("%5.3f"), m_fScaleY);
	m_fScaleZ = vScale.z;
	m_strScaleZ.Format(_T("%5.3f"), m_fScaleZ);

	UpdateData(FALSE);
}


void CTabMesh::OnLbnSelchange_PickDynamicObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bMeshSwitch = true;
	m_iIndex = m_DynamicBox.GetCurSel();

	m_MeshType.SetWindowTextW(L"Dynamic Mesh");
	CString strTemp = L"";
	m_DynamicBox.GetText(m_iIndex, strTemp);
	m_MeshName.SetWindowTextW(strTemp);

	CValueMgr::bMeshState = bMeshSwitch;
	CValueMgr::iCurIndex = m_iIndex;
	CValueMgr::strCurObj = strTemp;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dyanmic_MeshObject");

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);
	_vec3 vPos = pObj->Get_vPos();
	_vec3 vRot = pObj->Get_vRot();
	_vec3 vScale = pObj->Get_vScale();
	//pObj->Set_DeleteObj();

	m_fPosX = vPos.x;
	m_strPosX.Format(_T("%5.3f"), m_fPosX);
	m_fPosY = vPos.y;
	m_strPosY.Format(_T("%5.3f"), m_fPosY);
	m_fPosZ = vPos.z;
	m_strPosZ.Format(_T("%5.3f"), m_fPosZ);

	m_fRotX = vRot.x;
	m_strRotX.Format(_T("%5.3f"), m_fRotX);
	m_fRotY = vRot.y;
	m_strRotY.Format(_T("%5.3f"), m_fRotY);
	m_fRotZ = vRot.z;
	m_strRotZ.Format(_T("%5.3f"), m_fRotZ);

	m_fScaleX = vScale.x;
	m_strScaleX.Format(_T("%5.3f"), m_fScaleX);
	m_fScaleY = vScale.y;
	m_strScaleY.Format(_T("%5.3f"), m_fScaleY);
	m_fScaleZ = vScale.z;
	m_strScaleZ.Format(_T("%5.3f"), m_fScaleZ);

	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_SaveMesh()	//Static
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	PathCombine(szPath, szPath, L"Data");

	// 초기 경로 설정. 
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (Dlg.DoModal() == IDOK)
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Terrain Failed");
			return;
		}

		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		_tchar szBuff[MAX_STR] = L"";

		int iCnt = m_staticBox.GetCount();

		if (iCnt == 0)
			return;

		for (int i = 0; i < iCnt; ++i)
		{
			CString strTemp;
			m_staticBox.GetText(i, strTemp);

			auto iter = m_mapMesh[0].find(strTemp.GetString());

			WriteFile(hFile, &iter->second->iUnitType, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &iter->second->wMeshType, sizeof(_ushort), &dwByte, nullptr);
			//WriteFile(hFile, &iter.second->wContainerIdx, sizeof(_ushort), &dwByte, nullptr);

			lstrcpy(szBuff, iter->second->pMeshTag.c_str());
			dwBuff = iter->second->pMeshTag.length() + 1;
			WriteFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, szBuff, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);

			lstrcpy(szBuff, iter->second->pFileName.c_str());
			dwBuff = iter->second->pFileName.length() + 1;
			WriteFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, szBuff, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);

			
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
			list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Static_MeshObject");

			if (pList.empty())
				return;

			auto& iterList = pList.begin();

			for (_int j = 0; j < i; ++j)
				++iterList;

			CTestObj* pObj = dynamic_cast<CTestObj*>(*iterList);

			_vec3 vPos = pObj->Get_vPos();
			_vec3 vRot = pObj->Get_vRot();
			_vec3 vScale = pObj->Get_vScale();

			WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &vRot, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);


		}

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_LoadMesh()	//Static
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	PathCombine(szPath, szPath, L"Data");

	// 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (Dlg.DoModal() == IDOK)
	{
		CString strFileName = Dlg.GetPathName();
		strFileName = CFileInfo::ConvertRelativePath(strFileName);

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Terrain Failed");
			return;
		}

		Release_MapMesh(0);
		m_staticBox.ResetContent();
		//m_DynamicBox.ResetContent();

		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		TCHAR* szKey = nullptr;

		while (true)	//static
		{
			ENGINE::MESH_INFO pMesh;

			ReadFile(hFile, &pMesh.iUnitType, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &pMesh.wMeshType, sizeof(_ushort), &dwByte, nullptr);
			if (pMesh.wMeshType != 0)
			{
				AfxMessageBox(L"This File is No Static!");
				return;
			}

			//ReadFile(hFile, &mInfo.wContainerIdx, sizeof(_ushort), &dwByte, nullptr);

			//Mesh_Tag
			ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			szKey = new TCHAR[dwBuff];
			ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
			pMesh.pMeshTag = szKey;
			ENGINE::Safe_Delete_Array(szKey);

			//FileName
			ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			szKey = new TCHAR[dwBuff];
			ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
			pMesh.pFileName = szKey;
			ENGINE::Safe_Delete_Array(szKey);

			_vec3 vTransform[3];

			//Pos, Rot, Scale
			ReadFile(hFile, &vTransform[0], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vTransform[1], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vTransform[2], sizeof(_vec3), &dwByte, nullptr);

			if (dwByte == 0)
				break;
		
			m_staticBox.AddString(pMesh.pFileName.c_str());
			CValueMgr::bMeshState = FALSE;

			wstring strKey = L"Static_MeshObject";
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
			CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
			pRender->Add_GameObject(pMesh.pMeshTag, strKey, vTransform);
		}


		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_SaveMesh_Dynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	PathCombine(szPath, szPath, L"Data");

	// 초기 경로 설정. 
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (Dlg.DoModal() == IDOK)
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Terrain Failed");
			return;
		}

		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		_tchar szBuff[MAX_STR] = L"";

		int iCnt = m_DynamicBox.GetCount();

		if (iCnt == 0)
			return;

		for (int i = 0; i < iCnt; ++i)
		{
			CString strTemp;
			m_DynamicBox.GetText(i, strTemp);

			auto iter = m_mapMesh[1].find(strTemp.GetString());

			WriteFile(hFile, &iter->second->iUnitType, sizeof(_int), &dwByte, nullptr);
			WriteFile(hFile, &iter->second->wMeshType, sizeof(_ushort), &dwByte, nullptr);
			//WriteFile(hFile, &iter.second->wContainerIdx, sizeof(_ushort), &dwByte, nullptr);

			lstrcpy(szBuff, iter->second->pMeshTag.c_str());
			dwBuff = iter->second->pMeshTag.length() + 1;
			WriteFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, szBuff, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);

			lstrcpy(szBuff, iter->second->pFileName.c_str());
			dwBuff = iter->second->pFileName.length() + 1;
			WriteFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, szBuff, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);


			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
			list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_MeshObject");

			if (pList.empty())
				return;

			auto& iterList = pList.begin();

			for (_int j = 0; j < i; ++j)
				++iterList;

			CTestObj* pObj = dynamic_cast<CTestObj*>(*iterList);

			_vec3 vPos = pObj->Get_vPos();
			_vec3 vRot = pObj->Get_vRot();
			_vec3 vScale = pObj->Get_vScale();

			WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &vRot, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, nullptr);


		}

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTabMesh::OnBnClicked_LoadMesh_Dynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szPath[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szPath);
	PathRemoveFileSpec(szPath);
	PathRemoveFileSpec(szPath);

	PathCombine(szPath, szPath, L"Data");

	// 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (Dlg.DoModal() == IDOK)
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Terrain Failed");
			return;
		}

		Release_MapMesh(1);
		m_DynamicBox.ResetContent();

		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		TCHAR* szKey = nullptr;

		while (true)	//Dynamic
		{
			ENGINE::MESH_INFO pMesh;

			ReadFile(hFile, &pMesh.iUnitType, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &pMesh.wMeshType, sizeof(_ushort), &dwByte, nullptr);

			if (pMesh.wMeshType != 1)
			{
				AfxMessageBox(L"This File is No Dynamic!");
				return;
			}
			//ReadFile(hFile, &mInfo.wContainerIdx, sizeof(_ushort), &dwByte, nullptr);

			ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			szKey = new TCHAR[dwBuff];
			ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
			pMesh.pMeshTag = szKey;
			ENGINE::Safe_Delete_Array(szKey);

			ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
			szKey = new TCHAR[dwBuff];
			ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
			pMesh.pFileName = szKey;
			ENGINE::Safe_Delete_Array(szKey);

			_vec3 vTransform[3];

			ReadFile(hFile, &vTransform[0], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vTransform[1], sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &vTransform[2], sizeof(_vec3), &dwByte, nullptr);


			//m_strPosX.Format(_T("%5.3f"), vTransform[0].x);
			//m_strPosY.Format(_T("%5.3f"), vTransform[0].y);
			//m_strPosZ.Format(_T("%5.3f"), vTransform[0].z);
			//m_strRotX.Format(_T("%5.3f"), vTransform[1].x);
			//m_strRotY.Format(_T("%5.3f"), vTransform[1].y);
			//m_strRotZ.Format(_T("%5.3f"), vTransform[1].z);
			//m_strScaleX.Format(_T("%5.3f"), vTransform[2].x);
			//m_strScaleY.Format(_T("%5.3f"), vTransform[2].y);
			//m_strScaleZ.Format(_T("%5.3f"), vTransform[2].z);
			
			if (dwByte == 0)
				break;

			m_DynamicBox.AddString(pMesh.pFileName.c_str());
			
			wstring strKey = L"Dynamic_MeshObject";
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
			CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
			pRender->Add_GameObject(pMesh.pMeshTag, strKey, vTransform);
		}

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}
