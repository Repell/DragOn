// TabNavi.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "TabNavi.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "TotalToolView.h"
#include "ToolRender.h"
//#include "MouseCtn.h"
#include "NaviMaker.h"


// CTabNavi 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabNavi, CDialogEx)

CTabNavi::CTabNavi(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TABTHREE, pParent)
	, m_strPosX(_T(""))
	, m_strPosY(_T(""))
	, m_strPosZ(_T(""))
	, m_bClick(FALSE)
	, strCurType(_T(""))
	, strCurMesh(_T(""))
{
	iNaviNumb = 0;
}

CTabNavi::~CTabNavi()
{
}

void CTabNavi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeList);
	DDX_Text(pDX, IDC_EDIT_SPIN5, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_SPIN6, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_SPIN7, m_strPosZ);
	DDX_Check(pDX, IDC_CHECK1, m_bClick);
	DDX_Control(pDX, IDC_CHECK1, Check_MakeLine);
	DDX_Text(pDX, IDC_EDIT1, strCurType);
	DDX_Text(pDX, IDC_EDIT2, strCurMesh);
}


BEGIN_MESSAGE_MAP(CTabNavi, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTabNavi::OnTvnSelchanged_NaviTree)
	ON_EN_CHANGE(IDC_EDIT_SPIN5, &CTabNavi::OnEnChangeEdit_PosX)
	ON_EN_CHANGE(IDC_EDIT_SPIN6, &CTabNavi::OnEnChangeEdit_PosY)
	ON_EN_CHANGE(IDC_EDIT_SPIN7, &CTabNavi::OnEnChangeEdit_PosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTabNavi::OnDeltaposSpin_PosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTabNavi::OnDeltaposSpin_PosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTabNavi::OnDeltaposSpin_PosZ)
	ON_BN_CLICKED(IDC_CHECK1, &CTabNavi::OnBnClicked_CreateCheck)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabNavi::OnBnClicked_SaveNavi)
	ON_BN_CLICKED(IDC_BUTTON10, &CTabNavi::OnBnClicked_LoadNavi)
	ON_BN_CLICKED(IDC_BUTTON11, &CTabNavi::OnBnClicked_DeleteNavi)
END_MESSAGE_MAP()


// CTabNavi 메시지 처리기입니다.


void CTabNavi::Insert_Navi(_int idx)
{
	CString strTemp;

	strTemp.Format(_T("%d"), idx);
	
	hLevel1 = m_TreeList.InsertItem(strTemp, 0, 0);
	m_TreeList.Expand(hLevel1, TVE_EXPAND);

	strTemp.Format(_T("0"));
	hLevel2 = m_TreeList.InsertItem(strTemp, 0, 0, hLevel1);

	strTemp.Format(_T("1"));
	hLevel2 = m_TreeList.InsertItem(strTemp, 0, 0, hLevel1);

	strTemp.Format(_T("2"));
	hLevel2 = m_TreeList.InsertItem(strTemp, 0, 0, hLevel1);

}

void CTabNavi::Update_Transform()
{
	UpdateData(TRUE);

	m_strPosX.Format(_T("%5.3f"), m_fPosX);
	m_strPosY.Format(_T("%5.3f"), m_fPosY);
	m_strPosZ.Format(_T("%5.3f"), m_fPosZ);
	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());

	pNavi->Transform_NaviMesh(iNaviNum, iTriNum, vPos);
	

	UpdateData(FALSE);
}

BOOL CTabNavi::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(TRUE);

	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;
	m_strPosX.Format(_T("%5.3f"), m_fPosX);
	m_strPosY.Format(_T("%5.3f"), m_fPosY);
	m_strPosZ.Format(_T("%5.3f"), m_fPosZ);

	Check_MakeLine.SetCheck(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CTabNavi::PreTranslateMessage(MSG* pMsg)
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


void CTabNavi::OnTvnSelchanged_NaviTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Check_MakeLine.SetCheck(false);
	CValueMgr::bMakeLine = false;
	m_bClick = false;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());
	//pNavi->Transform_NaviMesh(iNaviNum, iTriNum, _vec3(0.f, 10.f, 0.f));

	hLevel2 = m_TreeList.GetNextItem(NULL, TVGN_CARET);
	hLevel1 = m_TreeList.GetNextItem(hLevel2, TVGN_PARENT);

	CString strTmp = m_TreeList.GetItemText(hLevel1);
	CString strTmp1 = m_TreeList.GetItemText(hLevel2);

	iNaviNum = _wtoi(strTmp);
	iTriNum = _wtoi(strTmp1);

	//ENGINE::LINE_3D pLine = pNavi->Get_vecLine3D(iNaviNum);
	ENGINE::NAVI tNavi = pNavi->Get_Navi(iNaviNum);
	_vec3 vTemp;
	switch (iTriNum)
	{
	case 0:
		vTemp = tNavi.vPointA;
		break;
	case 1:
		vTemp = tNavi.vPointB;
		break;
	case 2:
		vTemp = tNavi.vPointC;
		break;
	}


	m_fPosX = vTemp.x;
	m_fPosY = vTemp.y;
	m_fPosZ = vTemp.z;
	m_strPosX.Format(_T("%5.2f"), m_fPosX);
	m_strPosY.Format(_T("%5.2f"), m_fPosY);
	m_strPosZ.Format(_T("%5.2f"), m_fPosZ);

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabNavi::OnEnChangeEdit_PosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fPosX = (float)_wtof(m_strPosX);
	m_fPosY = (float)_wtof(m_strPosY);
	m_fPosZ = (float)_wtof(m_strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());

	pNavi->Transform_NaviMesh(iNaviNum, iTriNum, vPos);

	UpdateData(FALSE);
}


void CTabNavi::OnEnChangeEdit_PosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fPosX = (float)_wtof(m_strPosX);
	m_fPosY = (float)_wtof(m_strPosY);
	m_fPosZ = (float)_wtof(m_strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());

	pNavi->Transform_NaviMesh(iNaviNum, iTriNum, vPos);

	UpdateData(FALSE);
}


void CTabNavi::OnEnChangeEdit_PosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fPosX = (float)_wtof(m_strPosX);
	m_fPosY = (float)_wtof(m_strPosY);
	m_fPosZ = (float)_wtof(m_strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());

	pNavi->Transform_NaviMesh(iNaviNum, iTriNum, vPos);

	UpdateData(FALSE);
}


void CTabNavi::OnDeltaposSpin_PosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosX += 0.1f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosX -= 0.1f;

	Update_Transform();

	UpdateData(FALSE);

	*pResult = 0;
}


void CTabNavi::OnDeltaposSpin_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosY += 0.1f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosY -= 0.1f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabNavi::OnDeltaposSpin_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosZ += 0.1f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosZ -= 0.1f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabNavi::OnBnClicked_CreateCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bClick = Check_MakeLine.GetCheck();

	if (m_bClick)
		CValueMgr::bMakeLine = true;
	else
		CValueMgr::bMakeLine = false;

	UpdateData(TRUE);

	m_iMeshIndex = CValueMgr::iCurIndex;
	strCurMesh = CValueMgr::strCurObj;

	if (CValueMgr::bMeshState)
	{
		strCurType = L"Dynamic_MeshObject";
		//strCurType = L"DynamicMesh";
	}
	else if (!CValueMgr::bMeshState)
	{
		strCurType = L"Static_MeshObject";
		//strCurType = L"StaticMesh";
	}

	UpdateData(FALSE);
}


void CTabNavi::OnBnClicked_SaveNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	Check_MakeLine.SetCheck(false);
	CValueMgr::bMakeLine = false;
	m_bClick = false;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());

	map<_int, ENGINE::NAVI> pMapNavi = pNavi->Get_MapNavi();


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
			AfxMessageBox(L"Save Navi Failed");
			return;
		}

		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		_tchar szBuff[MAX_STR] = L"";

		_int iCnt = pMapNavi.size();

		WriteFile(hFile, &iCnt, sizeof(_int), &dwByte, nullptr);

		auto iter = pMapNavi.begin();

		for (; iter != pMapNavi.end() ; ++iter)
		{
			WriteFile(hFile, &iter->second.Index, sizeof(_ulong), &dwByte, nullptr);
			WriteFile(hFile, iter->second.vPointA, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, iter->second.vPointB, sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, iter->second.vPointC, sizeof(_vec3), &dwByte, nullptr);
			
		}

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTabNavi::OnBnClicked_LoadNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	Check_MakeLine.SetCheck(false);
	CValueMgr::bMakeLine = false;	
	m_bClick = false;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());


	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_READONLY,
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
			AfxMessageBox(L"Save Navi Failed");
			return;
		}

		if (!pNavi->Get_MapNavi().empty())
		{
			pNavi->Clear_MapNavi();
			m_TreeList.DeleteAllItems();
		}
		
		map<_int, ENGINE::NAVI> pMapNavi;

		iNaviNumb = 0;
		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		_tchar szBuff[MAX_STR] = L"";

		ENGINE::NAVI tNavi;

		_int iCnt;

		ReadFile(hFile, &iCnt, sizeof(_int), &dwByte, nullptr);

		while (true)
		{
			ReadFile(hFile, &tNavi.Index, sizeof(_ulong), &dwByte, nullptr);
			ReadFile(hFile, &tNavi.vPointA, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &tNavi.vPointB, sizeof(_vec3), &dwByte, nullptr);
			ReadFile(hFile, &tNavi.vPointC, sizeof(_vec3), &dwByte, nullptr);

			if (dwByte == 0)
				break;

			pNavi->Get_MapNavi().emplace(tNavi.Index, tNavi);
			Insert_Navi(tNavi.Index);
			CValueMgr::iNaviCount = tNavi.Index + 1;
		}

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTabNavi::OnBnClicked_DeleteNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);
	CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pRender->Get_RenderList(L"NaviMaker").front());

	UpdateData(TRUE);

	hLevel2 = m_TreeList.GetNextItem(NULL, TVGN_CARET);
	hLevel1 = m_TreeList.GetNextItem(hLevel2, TVGN_PARENT);

	if (hLevel2 == nullptr)
		return;

	if (hLevel1 != nullptr)
	{
		CString strTmp = m_TreeList.GetItemText(hLevel1);
		CString strTmp1 = m_TreeList.GetItemText(hLevel2);

		iNaviNum = _wtoi(strTmp.GetString());
		iTriNum = _wtoi(strTmp1.GetString());
		pNavi->Delete_MapNavi(iNaviNum, iTriNum);
		m_TreeList.DeleteItem(hLevel1);
	}
	else
	{
		CString strTmp = m_TreeList.GetItemText(hLevel2);
		iNaviNum = _wtoi(strTmp.GetString());
		iTriNum = 0;
		pNavi->Delete_MapNavi(iNaviNum, iTriNum);
		m_TreeList.DeleteItem(hLevel2);
	}
	


	UpdateData(FALSE);
}

void CTabNavi::Pick_ObjectVtx()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(strCurType.GetString());

	if (pList.empty())
		return;

	auto& iter = pList.begin();

	for (_int i = 0; i < m_iMeshIndex; ++i)
		++iter;

	CTestObj* pObj = dynamic_cast<CTestObj*>(*iter);

	_ulong dwVtx = pObj->Get_NumVtx();

	_vec3* pVtxPos = new _vec3[dwVtx];

	pVtxPos = pObj->Get_VtxMeshPos();

	UpdateData(FALSE);
}
