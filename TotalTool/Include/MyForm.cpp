// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "MyForm.h"

#include "MainFrm.h"
#include "TotalToolView.h"



// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{
	m_pWndShow = nullptr;
	m_WndCpy = nullptr;
	//m_pTapTerrain = nullptr;
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchange_Tab)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TabONE = L"  Terrain Tool  ";
	m_TabTWO = L" Mesh Tool ";
	m_TabTHREE = L"  Navi Tool  ";
	m_TabFOUR = L"  Effect Tool  ";
	m_TabFIVE = L"  NOPE  ";

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Tab.InsertItem(0, m_TabONE);
	m_Tab.InsertItem(1, m_TabTWO);
	m_Tab.InsertItem(2, m_TabTHREE);
	m_Tab.InsertItem(3, m_TabFOUR);
	m_Tab.InsertItem(4, m_TabFIVE);

	CRect Rc = {};
	m_Tab.GetClientRect(&Rc);

	m_pTapTerrain.Create(IDD_TABONE, &m_Tab);
	m_pTapTerrain.SetWindowPos(NULL, 5, 25, Rc.Width() - 5, Rc.Height() - 25, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pWndShow = &m_pTapTerrain;

	m_pTabMesh.Create(IDD_TABTWO, &m_Tab);
	m_pTabMesh.SetWindowPos(NULL, 5, 25, Rc.Width() - 5, Rc.Height() - 25, SWP_NOZORDER);

	m_pTabNavi.Create(IDD_TABTHREE, &m_Tab);
	m_pTabNavi.SetWindowPos(NULL, 5, 25, Rc.Width() - 5, Rc.Height() - 25, SWP_NOZORDER);

	m_pTabEffect.Create(IDD_TABFOUR, &m_Tab);
	m_pTabEffect.SetWindowPos(NULL, 5, 25, Rc.Width() - 5, Rc.Height() - 25, SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CMyForm::OnTcnSelchange_Tab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));

	if (m_pWndShow != nullptr)
	{
		m_pWndShow->ShowWindow(SW_HIDE);
		m_pWndShow = nullptr;
	}

	int iIndex = m_Tab.GetCurSel();

	switch (iIndex)
	{
	case 0:
		m_pTapTerrain.ShowWindow(SW_SHOW);
		m_pWndShow = &m_pTapTerrain;
		m_eTab = TABTERRAIN;
		break;
	case 1:
		m_pTabMesh.ShowWindow(SW_SHOW);
		m_pWndShow = &m_pTabMesh;
		m_eTab = TABMESH;
		break;
	case 2:
		m_pTabNavi.ShowWindow(SW_SHOW);
		m_pWndShow = &m_pTabNavi;
		m_eTab = TABNAVI;
		break;
	case 3:
		m_pTabEffect.ShowWindow(SW_SHOW);
		m_pWndShow = &m_pTabEffect;
		m_eTab = TABEFFECT;
		break;
	}

	pView->m_eTabState = m_eTab;
	CValueMgr::eTab = m_eTab;

	*pResult = 0;
}
