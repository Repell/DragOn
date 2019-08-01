// TapTerrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "TapTerrain.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "TotalToolView.h"
#include "ToolRender.h"
#include "Terrain.h"
#include "MouseCtn.h"
#include "NaviMaker.h"

// CTapTerrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTapTerrain, CDialogEx)

CTapTerrain::CTapTerrain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TABONE, pParent)
	, m_strSize_X(_T(""))
	, m_strSize_Z(_T(""))
	, m_strInterval(_T(""))
	, m_strDetail(_T(""))
	, m_strPos_X(_T(""))
	, m_strPos_Y(_T(""))
	, m_strPos_Z(_T(""))
	, m_strRot_X(_T(""))
	, m_strRot_Y(_T(""))
	, m_strRot_Z(_T(""))
{

}

CTapTerrain::~CTapTerrain()
{
}

void CTapTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPIN1, m_strSize_X);
	DDX_Text(pDX, IDC_EDIT_SPIN2, m_strSize_Z);
	DDX_Text(pDX, IDC_EDIT_SPIN3, m_strInterval);
	DDX_Text(pDX, IDC_EDIT_SPIN4, m_strDetail);
	DDX_Text(pDX, IDC_EDIT_SPIN5, m_strPos_X);
	DDX_Text(pDX, IDC_EDIT_SPIN6, m_strPos_Y);
	DDX_Text(pDX, IDC_EDIT_SPIN7, m_strPos_Z);
	DDX_Text(pDX, IDC_EDIT_SPIN8, m_strRot_X);
	DDX_Text(pDX, IDC_EDIT_SPIN9, m_strRot_Y);
	DDX_Text(pDX, IDC_EDIT_SPIN10, m_strRot_Z);
	DDX_Control(pDX, IDC_LIST3, m_TextureListBox);
	DDX_Control(pDX, IDC_RADIO1, m_renderRadio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_renderRadio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_renderRadio[2]);
	DDX_Control(pDX, IDC_RADIO4, m_renderRadio[3]);

}


BEGIN_MESSAGE_MAP(CTapTerrain, CDialogEx)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTapTerrain::OnDeltapos_Spin_SizeX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTapTerrain::OnDeltapos_Spin_SizeZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTapTerrain::OnDeltapos_Spin_Interval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CTapTerrain::OnDeltapos_Spin_Detail)
	ON_BN_CLICKED(IDC_BUTTON1, &CTapTerrain::OnBnClicked_SetTerrain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTapTerrain::OnDeltapos_Spin_PosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTapTerrain::OnDeltapos_Spin_PosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTapTerrain::OnDeltapos_Spin_PosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTapTerrain::OnDeltapos_Spin_RotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CTapTerrain::OnDeltapos_Spin_RotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CTapTerrain::OnDeltapos_Spin_RotZ)
	ON_EN_CHANGE(IDC_EDIT_SPIN5, &CTapTerrain::OnEnChange_SetPosX)
	ON_EN_CHANGE(IDC_EDIT_SPIN6, &CTapTerrain::OnEnChange_SetPosY)
	ON_EN_CHANGE(IDC_EDIT_SPIN7, &CTapTerrain::OnEnChangeEdit_SetPosZ)
	ON_EN_CHANGE(IDC_EDIT_SPIN8, &CTapTerrain::OnEnChangeEdit_SetRotX)
	ON_EN_CHANGE(IDC_EDIT_SPIN9, &CTapTerrain::OnEnChangeEdit_SetRotY)
	ON_EN_CHANGE(IDC_EDIT_SPIN10, &CTapTerrain::OnEnChangeEdit_SetRotZ)
	ON_LBN_SELCHANGE(IDC_LIST3, &CTapTerrain::OnLbnSelchange_TextureBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CTapTerrain::OnBnClicked_ResetTerrain)
	ON_BN_CLICKED(IDC_BUTTON3, &CTapTerrain::OnBnClicked_SaveTerrain)
	ON_BN_CLICKED(IDC_BUTTON4, &CTapTerrain::OnBnClicked_LoadTerrain)
	ON_BN_CLICKED(IDC_RADIO1, &CTapTerrain::OnBnClicked_RenderSolid)
	ON_BN_CLICKED(IDC_RADIO2, &CTapTerrain::OnBnClicked_RenderWireframe)
	ON_BN_CLICKED(IDC_RADIO3, &CTapTerrain::OnBnClicked_RenderAxis)
	ON_BN_CLICKED(IDC_RADIO4, &CTapTerrain::OnBnClicked_NoRender)
END_MESSAGE_MAP()


// CTapTerrain 메시지 처리기입니다.

//
//void CTapTerrain::OnBnClicked_TestBut()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CColorDialog ClrDlg;
//
//	//Color Palete
//	if (ClrDlg.DoModal() == IDOK)
//	{
//		COLORREF rgb = ClrDlg.GetColor();
//	}
//	else
//		return;
//
//}

void CTapTerrain::Add_TextureString(const _tchar * pStr, _int pNum)
{
	UpdateData(TRUE);

	CString strTemp = pStr;
	CString strNumb = L"";
	strNumb.Format(L"| [%d]", pNum);
	m_TextureListBox.AddString(strTemp + strNumb);

	UpdateData(FALSE);
}

BOOL CTapTerrain::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(TRUE);

	m_iTex = 0;

	//Terrain Size init
	m_uSizeX = 1;
	m_uSizeZ = 1;
	m_uInterval = 1;
	m_uDetail = 1;

	m_strSize_X.Format(_T("%d"), m_uSizeX);
	m_strSize_Z.Format(_T("%d"), m_uSizeZ);
	m_strInterval.Format(_T("%d"), m_uInterval);
	m_strDetail.Format(_T("%d"), m_uDetail);

	//Terrain Pos / Rot
	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;
	m_fRotX = 0.f;
	m_fRotY = 0.f;
	m_fRotZ = 0.f;

	m_strPos_X.Format(_T("%5.2f"), m_fPosX);
	m_strPos_Y.Format(_T("%5.2f"), m_fPosY);
	m_strPos_Z.Format(_T("%5.2f"), m_fPosZ);
	m_strRot_X.Format(_T("%5.2f"), m_fRotX);
	m_strRot_Y.Format(_T("%5.2f"), m_fRotY);
	m_strRot_Z.Format(_T("%5.2f"), m_fRotZ);

	m_renderRadio[0].SetCheck(1);
	m_renderRadio[2].SetCheck(1);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CTapTerrain::OnDeltapos_Spin_SizeX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_uSizeX;
		m_strSize_X.Format(_T("%d"), m_uSizeX);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_uSizeX;
		if (m_uSizeX < 1)
			m_uSizeX = 1;

		m_strSize_X.Format(_T("%d"), m_uSizeX);
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_SizeZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_uSizeZ;
		m_strSize_Z.Format(_T("%d"), m_uSizeZ);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_uSizeZ;
		if (m_uSizeZ < 1)
			m_uSizeZ = 1;

		m_strSize_Z.Format(_T("%d"), m_uSizeZ);
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_Interval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_uInterval;
		m_strInterval.Format(_T("%d"), m_uInterval);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_uInterval;
		if (m_uInterval < 1)
			m_uInterval = 1;

		m_strInterval.Format(_T("%d"), m_uInterval);
	}

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_Detail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_uDetail;
		m_strDetail.Format(_T("%d"), m_uDetail);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_uDetail;
		if (m_uDetail < 1)
			m_uDetail = 1;

		m_strDetail.Format(_T("%d"), m_uDetail);
	}

	UpdateData(FALSE);
	*pResult = 0;
}



void CTapTerrain::OnBnClicked_SetTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_uSizeX = _wtoi(m_strSize_X);
	m_uSizeZ = _wtoi(m_strSize_Z);
	m_uInterval = _wtoi(m_strInterval);
	m_uDetail = _wtoi(m_strDetail);

	CValueMgr::wSizeX = m_uSizeX;
	CValueMgr::wSizeZ = m_uSizeZ;

	//Release Terrain
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());
	pTerrain->Release_Buffer();
	ENGINE::Release_Terrain(RESOURCE_TOOL, L"Buffer_Terrain");

	//Re Create Terrain
	CToolRender* pRender = pView->m_pToolRender;
	pRender->Create_NewTerrain(m_uSizeX, m_uSizeZ, m_uInterval, m_uDetail);

	pTerrain->Create_Buffer();

	list<ENGINE::CGameObject*>& pList2 = pView->m_pToolRender->Get_RenderList(L"MouseCtn");
	CMouseCtn* pMouse = dynamic_cast<CMouseCtn*>(pList2.front());
	pMouse->Make_TerrainVertex(m_uSizeX, m_uSizeZ);

	UpdateData(FALSE);
}


void CTapTerrain::OnDeltapos_Spin_PosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_fPosX;
		m_strPos_X.Format(_T("%5.2f"), m_fPosX);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_fPosX;
		m_strPos_X.Format(_T("%5.2f"), m_fPosX);
	}

	OnEnChange_SetPosX();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_fPosY;
		m_strPos_Y.Format(_T("%5.2f"), m_fPosY);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_fPosY;
		m_strPos_Y.Format(_T("%5.2f"), m_fPosY);
	}

	OnEnChange_SetPosY();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_fPosZ;
		m_strPos_Z.Format(_T("%5.2f"), m_fPosZ);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_fPosZ;
		m_strPos_Z.Format(_T("%5.2f"), m_fPosZ);
	}

	OnEnChangeEdit_SetPosZ();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_RotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_fRotX;
		m_strRot_X.Format(_T("%5.2f"), m_fRotX);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_fRotX;
		m_strRot_X.Format(_T("%5.2f"), m_fRotX);
	}

	OnEnChangeEdit_SetRotX();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_RotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_fRotY;
		m_strRot_Y.Format(_T("%5.2f"), m_fRotY);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_fRotY;
		m_strRot_Y.Format(_T("%5.2f"), m_fRotY);
	}

	OnEnChangeEdit_SetRotY();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnDeltapos_Spin_RotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		++m_fRotZ;
		m_strRot_Z.Format(_T("%5.2f"), m_fRotZ);
	}
	else if (pNMUpDown->iDelta > 0)
	{
		--m_fRotZ;
		m_strRot_Z.Format(_T("%5.2f"), m_fRotZ);
	}

	OnEnChangeEdit_SetRotZ();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTapTerrain::OnEnChange_SetPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	m_strPos_X.Format(_T("%5.2f"), m_fPosX);
	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTapTerrain::OnEnChange_SetPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	m_strPos_Y.Format(_T("%5.2f"), m_fPosY);
	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTapTerrain::OnEnChangeEdit_SetPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	m_strPos_Z.Format(_T("%5.2f"), m_fPosZ);
	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTapTerrain::OnEnChangeEdit_SetRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	m_strRot_X.Format(_T("%5.2f"), m_fRotX);
	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTapTerrain::OnEnChangeEdit_SetRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	m_strRot_Y.Format(_T("%5.2f"), m_fRotY);
	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTapTerrain::OnEnChangeEdit_SetRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	m_strRot_Z.Format(_T("%5.2f"), m_fRotZ);
	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTapTerrain::OnLbnSelchange_TextureBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	_ulong	iIndex = m_TextureListBox.GetCurSel();
	strKey = L"";
	m_TextureListBox.GetText(iIndex, strKey);

	CString strTex = strKey.Right(5);
	m_iTex = 0;
	for (; m_iTex < strTex.GetLength(); ++m_iTex)
	{
		if (isdigit(strTex[m_iTex]))	//문자열 하나하나 검색, 숙자가 나오면 true
			break;
	}
	strTex.Delete(0, m_iTex);
	m_iTex = _wtoi(strTex);

	AfxExtractSubString(strKey, strKey, 0, '|');

	pTerrain->Release_Texture();
	HRESULT hr = pTerrain->Set_Texture(strKey, m_iTex);

}


void CTapTerrain::OnBnClicked_ResetTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());

	//Terrain Size init
	m_uSizeX = TERRAIN_VTX_X;
	m_uSizeZ = TERRAIN_VTX_Z;
	m_uInterval = TERRAIN_INTRV;
	m_uDetail = 1;

	CValueMgr::wSizeX = m_uSizeX;
	CValueMgr::wSizeZ = m_uSizeZ;

	m_strSize_X.Format(_T("%d"), m_uSizeX);
	m_strSize_Z.Format(_T("%d"), m_uSizeZ);
	m_strInterval.Format(_T("%d"), m_uInterval);
	m_strDetail.Format(_T("%d"), m_uDetail);

	//Terrain Pos / Rot
	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;
	m_fRotX = 0.f;
	m_fRotY = 0.f;
	m_fRotZ = 0.f;

	m_strPos_X.Format(_T("%5.2f"), m_fPosX);
	m_strPos_Y.Format(_T("%5.2f"), m_fPosY);
	m_strPos_Z.Format(_T("%5.2f"), m_fPosZ);
	m_strRot_X.Format(_T("%5.2f"), m_fRotX);
	m_strRot_Y.Format(_T("%5.2f"), m_fRotY);
	m_strRot_Z.Format(_T("%5.2f"), m_fRotZ);

	pTerrain->Release_Texture();
	HRESULT hr = pTerrain->Set_Texture(L"Texture_Terrain", 0);

	pTerrain->Set_Transform(_vec3(m_fPosX, m_fPosY, m_fPosZ), _vec3(m_fRotX, m_fRotY, m_fRotZ));

	UpdateData(FALSE);
}


void CTapTerrain::OnBnClicked_SaveTerrain()
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
		TCHAR szKey[MAX_STR] = L"";
		lstrcpy(szKey, strKey);
		dwBuff = strKey.GetLength() + 1;

		WriteFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);

		WriteFile(hFile, &m_uSizeX, sizeof(WORD), &dwByte, nullptr);
		WriteFile(hFile, &m_uSizeZ, sizeof(WORD), &dwByte, nullptr);
		WriteFile(hFile, &m_uInterval, sizeof(WORD), &dwByte, nullptr);
		WriteFile(hFile, &m_uDetail, sizeof(WORD), &dwByte, nullptr);
		WriteFile(hFile, &_vec3(m_fPosX, m_fPosY, m_fPosZ), sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &_vec3(m_fRotX, m_fRotY, m_fRotZ), sizeof(_vec3), &dwByte, nullptr);
		WriteFile(hFile, &m_iTex, sizeof(_uint), &dwByte, nullptr);

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTapTerrain::OnBnClicked_LoadTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_READONLY,
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

		OnBnClicked_ResetTerrain();

		DWORD dwByte = 0;
		DWORD dwBuff = 0;
		TCHAR* szKey = nullptr;
		_vec3 vPos, vRot;

		ReadFile(hFile, &dwBuff, sizeof(DWORD), &dwByte, nullptr);
		szKey = new TCHAR[dwBuff];
		ReadFile(hFile, szKey, sizeof(TCHAR) * dwBuff, &dwByte, nullptr);
		strKey = szKey;

		ReadFile(hFile, &m_uSizeX, sizeof(WORD), &dwByte, nullptr);
		ReadFile(hFile, &m_uSizeZ, sizeof(WORD), &dwByte, nullptr);
		ReadFile(hFile, &m_uInterval, sizeof(WORD), &dwByte, nullptr);
		ReadFile(hFile, &m_uDetail, sizeof(WORD), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &vRot, sizeof(_vec3), &dwByte, nullptr);
		ReadFile(hFile, &m_iTex, sizeof(_uint), &dwByte, nullptr);

		//Release Terrain
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
		list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Terrain");
		list<ENGINE::CGameObject*>& pList2 = pView->m_pToolRender->Get_RenderList(L"NaviMaker");

		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pList.front());
		pTerrain->Release_Buffer();
		ENGINE::Release_Terrain(RESOURCE_TOOL, L"Buffer_Terrain");

		//Re Create Terrain
		CToolRender* pRender = pView->m_pToolRender;
		pRender->Create_NewTerrain(m_uSizeX, m_uSizeZ, m_uInterval, m_uDetail);

		CValueMgr::wSizeX = m_uSizeX;
		CValueMgr::wSizeZ = m_uSizeZ;

		pTerrain->Create_Buffer();

		pTerrain->Release_Texture();
		HRESULT hr = pTerrain->Set_Texture(strKey, m_iTex);
		pTerrain->Set_Transform(vPos, vRot);
		
		CNaviMaker* pNavi = dynamic_cast<CNaviMaker*>(pList2.front());
		pNavi->Make_TerrainVertex(m_uSizeX, m_uSizeZ);


		ENGINE::Safe_Delete_Array(szKey);

		CloseHandle(hFile);
	}
	else
		return;

	UpdateData(FALSE);
}


void CTapTerrain::OnBnClicked_RenderSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CValueMgr::bWireFrm_Terrain = FALSE;
}


void CTapTerrain::OnBnClicked_RenderWireframe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CValueMgr::bWireFrm_Terrain = TRUE;
}


void CTapTerrain::OnBnClicked_RenderAxis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CValueMgr::bAxisLine = TRUE;
}


void CTapTerrain::OnBnClicked_NoRender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CValueMgr::bAxisLine = FALSE;
}

// ESC 무시 
BOOL CTapTerrain::PreTranslateMessage(MSG* pMsg)
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
