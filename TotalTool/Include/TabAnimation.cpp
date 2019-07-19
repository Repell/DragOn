// TabAnimation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "TabAnimation.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "TotalToolView.h"
#include "ToolRender.h"
#include "TestObj.h"

// CTabAnimation 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabAnimation, CDialogEx)

CTabAnimation::CTabAnimation(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMATION, pParent)
	, strPosX(_T(""))
	, strPosY(_T(""))
	, strPosZ(_T(""))
	, strScaleX(_T(""))
	, strScaleY(_T(""))
	, strScaleZ(_T(""))
{

}

CTabAnimation::~CTabAnimation()
{
	m_mapBone.clear();
}

void CTabAnimation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_CurXFile);
	DDX_Text(pDX, IDC_EDIT_SPIN5, strPosX);
	DDX_Text(pDX, IDC_EDIT_SPIN6, strPosY);
	DDX_Text(pDX, IDC_EDIT_SPIN14, strPosZ);
	DDX_Text(pDX, IDC_EDIT_SPIN11, strScaleX);
	DDX_Text(pDX, IDC_EDIT_SPIN12, strScaleY);
	DDX_Text(pDX, IDC_EDIT_SPIN13, strScaleZ);
	DDX_Control(pDX, IDC_LIST1, m_BoneList);
	DDX_Control(pDX, IDC_LIST4, m_AniList);
	DDX_Control(pDX, IDC_CHECK1, m_VisibleBoneName);
}


BEGIN_MESSAGE_MAP(CTabAnimation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTabAnimation::OnBnClicked_LoadXFile)
	ON_BN_CLICKED(IDC_BUTTON12, &CTabAnimation::OnBnClicked_DeleteXFile)
	ON_EN_CHANGE(IDC_EDIT_SPIN5, &CTabAnimation::OnEnChangeEdit_PosX)
	ON_EN_CHANGE(IDC_EDIT_SPIN6, &CTabAnimation::OnEnChangeEdit_PosY)
	ON_EN_CHANGE(IDC_EDIT_SPIN14, &CTabAnimation::OnEnChangeEdit_PosZ)
	ON_EN_CHANGE(IDC_EDIT_SPIN11, &CTabAnimation::OnEnChangeEdit_ScaleX)
	ON_EN_CHANGE(IDC_EDIT_SPIN12, &CTabAnimation::OnEnChangeEdit_ScaleY)
	ON_EN_CHANGE(IDC_EDIT_SPIN13, &CTabAnimation::OnEnChangeEdit_ScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTabAnimation::OnDeltaposSpin_PosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN14, &CTabAnimation::OnDeltaposSpin_PosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTabAnimation::OnDeltaposSpin_PosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN17, &CTabAnimation::OnDeltaposSpin_ScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CTabAnimation::OnDeltaposSpin_ScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN13, &CTabAnimation::OnDeltaposSpin_ScaleZ)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTabAnimation::OnLbnSelchange_BoneList)
	ON_LBN_SELCHANGE(IDC_LIST4, &CTabAnimation::OnLbnSelchange_AniList)
	ON_BN_CLICKED(IDC_BUTTON3, &CTabAnimation::OnBnClicked_ExportAniList)
	ON_BN_CLICKED(IDC_BUTTON13, &CTabAnimation::OnBnClicked_BoneList)
	ON_BN_CLICKED(IDC_CHECK1, &CTabAnimation::OnBnClicked_Visible)
END_MESSAGE_MAP()


// CTabAnimation 메시지 처리기입니다.


map<wstring, _matrix*>& CTabAnimation::Get_mapBones()
{
	return m_mapBone;
}

void CTabAnimation::Update_Transform()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;
	
	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	UpdateData(TRUE);

	strPosX.Format(_T("%5.3f"), m_fPosX);
	strPosY.Format(_T("%5.3f"), m_fPosY);
	strPosZ.Format(_T("%5.3f"), m_fPosZ);

	strScaleX.Format(_T("%5.3f"), m_fScaleX);
	strScaleY.Format(_T("%5.3f"), m_fScaleY);
	strScaleZ.Format(_T("%5.3f"), m_fScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);


	UpdateData(FALSE);
}

void CTabAnimation::Add_MeshFrameList()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	ENGINE::D3DXMESHCONTAINER_DERIVED* pContainer = pObj->Get_Dynamic()->Get_MeshContainer();

	LPD3DXANIMATIONCONTROLLER pAniCon = pObj->Get_Dynamic()->Get_AniController()->Get_AniControll();

	_ulong iIndex = pAniCon->GetNumAnimationSets();
	LPD3DXANIMATIONSET pAS = nullptr;

	//AniList
	for (_ulong i = 0; i < iIndex; ++i)
	{
		pAniCon->GetAnimationSet(i, &pAS);
	
		CString strNumb = L"";
		const char* strAni = pAS->GetName();
		_ulong iLength = strlen(strAni);
		wstring strName(&strAni[0], &strAni[iLength]);

		if (i < 10)
			strNumb.Format(L"[ 00%d ] : ", i);
		else if (i < 100)
			strNumb.Format(L"[ 0%d ] : ", i);
		else
			strNumb.Format(L"[ %d ] : ", i);

		strNumb += strName.c_str();

		m_AniList.AddString(strNumb);

	}

	//Bone List
	for (_ulong i = 0; i < pContainer->dwNumBones; ++i)
	{
		const char* strBone = pContainer->pSkinInfo->GetBoneName(i);

		_ulong iLength = strlen(strBone);

		CString strNumb = L"";
		wstring strName(&strBone[0], &strBone[iLength]);
		
		if(i < 10)
			strNumb.Format(L"[ 00%d ] : ", i);
		else if(i < 100)
			strNumb.Format(L"[ 0%d ] : ", i);
		else
			strNumb.Format(L"[ %d ] : ", i);
		
		strNumb += strName.c_str();

		//m_BoneList.AddString(strNumb);
		m_mapBone.emplace(strName, pContainer->ppFrameCombinedMatrix[i]);
		//_vec3 pPos = {};
		//_matrix pMatrix = *pContainer->ppFrameCombinedMatrix[i];
		//memcpy(pPos, &pMatrix.m[3][0], sizeof(_vec3));
		
	}

	for (auto iter : m_mapBone)
		m_BoneList.AddString(iter.first.c_str());

}

BOOL CTabAnimation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	UpdateData(TRUE);

	m_VisibleBoneName.SetCheck(0);
	CValueMgr::bVisibleBone = FALSE;
	
	ZeroMemory(&m_MeshInfo, sizeof(ENGINE::MESH_INFO));

	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;

	m_fScaleX = 0.1f;
	m_fScaleY = 0.1f;
	m_fScaleZ = 0.1f;

	strPosX.Format(L"%5.3f", m_fPosX);
	strPosY.Format(L"%5.3f", m_fPosY);
	strPosZ.Format(L"%5.3f", m_fPosZ);

	strScaleX.Format(L"%5.3f", m_fScaleX);
	strScaleY.Format(L"%5.3f", m_fScaleY);
	strScaleZ.Format(L"%5.3f", m_fScaleZ);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CTabAnimation::PreTranslateMessage(MSG* pMsg)
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


void CTabAnimation::OnBnClicked_LoadXFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);

	CFileFind finder;		//검색클래스 생성
	CFileDialog Dlg(TRUE, L"X", L"*.X", OFN_HIDEREADONLY | OFN_READONLY,
		L"Data Files(*.X)|*.X||", this);

	CString strMeshPath = L"../../Client/Bin/Resources/Mesh/DynamicMesh/";

	// 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = strMeshPath;

	if (Dlg.DoModal() == IDOK)
	{
		if (!pRender->Get_RenderList(L"Dynamic_AniObject").empty())
			return;
		
		CString strFileName = Dlg.GetFileName();
		CString strPathName = L"";

		TCHAR szCurPath[MAX_STR] = L"";
		lstrcpy(szCurPath, Dlg.GetPathName());
		PathRemoveExtension(szCurPath);			///확장자 제거
		PathRemoveFileSpec(szCurPath);		///파일명 제거
		strPathName = CFileInfo::ConvertRelativePath(szCurPath);
		strPathName += L"/";
		
		m_MeshInfo.iUnitType = 0;
		m_MeshInfo.wContainerIdx = 0;
		m_MeshInfo.wMeshType = 1;
		m_MeshInfo.pFileName = strFileName;
		//wstring MeshTag = L"Mesh_" + Dlg.GetFileTitle();
		m_MeshInfo.pMeshTag = L"Mesh_" + Dlg.GetFileTitle();
		
		if (FAILED(ENGINE::Ready_Meshes(ENGINE::Get_GraphicDev()->Get_Device(), RESOURCE_TOOL,
			m_MeshInfo.pMeshTag.c_str(), ENGINE::MESH_DYNAMIC, strPathName, m_MeshInfo.pFileName.c_str())))
		{
			CString strTemp = m_MeshInfo.pFileName.c_str();
			AfxMessageBox(strTemp);

			CValueMgr::bMeshState = TRUE;
			pRender->Add_GameObject(m_MeshInfo.pMeshTag, L"Dynamic_AniObject");
			m_CurXFile.SetWindowTextW(m_MeshInfo.pFileName.c_str());

		}

		CValueMgr::bMeshState = TRUE;
		pRender->Add_GameObject(m_MeshInfo.pMeshTag, L"Dynamic_AniObject");
		m_CurXFile.SetWindowTextW(m_MeshInfo.pFileName.c_str());

		//ENGINE::Safe_Release(m_pMesh);
	}

	Add_MeshFrameList();

	UpdateData(FALSE);
}


void CTabAnimation::OnBnClicked_DeleteXFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	CToolRender* pRender = dynamic_cast<CToolRender*>(pView->m_pToolRender);

	pRender->Delete_GameObject(L"Dynamic_AniObject");

	m_AniList.ResetContent();
	m_BoneList.ResetContent();

	m_CurXFile.SetWindowTextW(L"");

	//for_each(m_mapBone.begin(), m_mapBone.end(), [](auto& MyPair) 
	//{
	//	MyPair.second
	//});
	m_mapBone.clear();

}


void CTabAnimation::OnEnChangeEdit_PosX()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	m_fPosX = (float)_wtof(strPosX);
	m_fPosY = (float)_wtof(strPosY);
	m_fPosZ = (float)_wtof(strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);
	
	UpdateData(FALSE);
}


void CTabAnimation::OnEnChangeEdit_PosY()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	m_fPosX = (float)_wtof(strPosX);
	m_fPosY = (float)_wtof(strPosY);
	m_fPosZ = (float)_wtof(strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);

	UpdateData(FALSE);
}


void CTabAnimation::OnEnChangeEdit_PosZ()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	m_fPosX = (float)_wtof(strPosX);
	m_fPosY = (float)_wtof(strPosY);
	m_fPosZ = (float)_wtof(strPosZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);

	UpdateData(FALSE);
}


void CTabAnimation::OnEnChangeEdit_ScaleX()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	m_fScaleX = (float)_wtof(strScaleX);
	m_fScaleY = (float)_wtof(strScaleY);
	m_fScaleZ = (float)_wtof(strScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);

	UpdateData(FALSE);
}


void CTabAnimation::OnEnChangeEdit_ScaleY()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	m_fScaleX = (float)_wtof(strScaleX);
	m_fScaleY = (float)_wtof(strScaleY);
	m_fScaleZ = (float)_wtof(strScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);

	UpdateData(FALSE);
}


void CTabAnimation::OnEnChangeEdit_ScaleZ()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	m_fScaleX = (float)_wtof(strScaleX);
	m_fScaleY = (float)_wtof(strScaleY);
	m_fScaleZ = (float)_wtof(strScaleZ);

	_vec3 vPos = { m_fPosX, m_fPosY, m_fPosZ };
	_vec3 vRot = { 0.f, 0.f, 0.f };
	_vec3 vScale = { m_fScaleX, m_fScaleY, m_fScaleZ };

	pObj->Set_Transform(vPos, vRot, vScale);

	UpdateData(FALSE);
}


void CTabAnimation::OnDeltaposSpin_PosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosX += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosX -= 1.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabAnimation::OnDeltaposSpin_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosY += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosY -= 1.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabAnimation::OnDeltaposSpin_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fPosZ += 1.f;
	else if (pNMUpDown->iDelta > 0)
		m_fPosZ -= 1.f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabAnimation::OnDeltaposSpin_ScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fScaleX += 0.01f;
	else if (pNMUpDown->iDelta > 0)
		m_fScaleX -= 0.01f;

	Update_Transform();

	UpdateData(FALSE);
	*pResult = 0;
}


void CTabAnimation::OnDeltaposSpin_ScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fScaleY += 0.01f;
	else if (pNMUpDown->iDelta > 0)
		m_fScaleY -= 0.01f;

	Update_Transform();

	UpdateData(FALSE);

	*pResult = 0;
}


void CTabAnimation::OnDeltaposSpin_ScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
		m_fScaleZ += 0.01f;
	else if (pNMUpDown->iDelta > 0)
		m_fScaleZ -= 0.01f;

	Update_Transform();

	UpdateData(FALSE);

	*pResult = 0;
}


void CTabAnimation::OnLbnSelchange_BoneList()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	ENGINE::D3DXMESHCONTAINER_DERIVED* pContainer = pObj->Get_Dynamic()->Get_MeshContainer();

	_ulong iIndex = m_BoneList.GetCurSel();
	CString strTemp = L"";
	m_BoneList.GetText(iIndex, strTemp);

	wstring pStr = strTemp;

	map<wstring, _matrix*>::iterator iter_find = m_mapBone.find(pStr);

	UpdateData(FALSE);
}


void CTabAnimation::OnLbnSelchange_AniList()
{
	UpdateData(TRUE);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTotalToolView* pView = dynamic_cast<CTotalToolView*>(pMain->Get_MainWnd().GetPane(0, 0));
	list<ENGINE::CGameObject*>& pList = pView->m_pToolRender->Get_RenderList(L"Dynamic_AniObject");

	auto& iter = pList.front();

	if (iter == nullptr)
		return;

	CTestObj* pObj = dynamic_cast<CTestObj*>(iter);

	ENGINE::D3DXMESHCONTAINER_DERIVED* pContainer = pObj->Get_Dynamic()->Get_MeshContainer();

	LPD3DXANIMATIONCONTROLLER pAniCon = pObj->Get_Dynamic()->Get_AniController()->Get_AniControll();

	_ulong iIndex = m_AniList.GetCurSel();
	
	pObj->Get_Dynamic()->Set_AnimationSet(iIndex);
	
	UpdateData(FALSE);
}


void CTabAnimation::OnBnClicked_ExportAniList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"txt", L"AniList.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||", FALSE);

	CString strMeshPath = L"../../Client/Bin/Resources/Mesh/DynamicMesh/";
	Dlg.m_ofn.lpstrInitialDir = strMeshPath;

	if (IDOK == Dlg.DoModal())
	{
		wofstream fout;

		fout.open(Dlg.GetPathName().GetString());

		if (fout.fail())
			return;

		_ulong iIndex = m_AniList.GetCount();

		for (_ulong i = 0; 0 < iIndex; ++i)
		{
			CString strTemp = L"";
			m_AniList.GetText(i, strTemp);

			fout << strTemp.GetString() << endl;
		}

		fout.close();
	}
	else
		return;

	//WinExec("notepad.exe ../../Data/AniList.txt", SW_SHOW);
	UpdateData(FALSE);
}


void CTabAnimation::OnBnClicked_BoneList()
{
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"txt", L"BoneList.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||", FALSE);

	CString strMeshPath = L"../../Client/Bin/Resources/Mesh/DynamicMesh/";
	Dlg.m_ofn.lpstrInitialDir = strMeshPath;

	if (IDOK == Dlg.DoModal())
	{
		wofstream fout;

		fout.open(Dlg.GetPathName().GetString());

		if (fout.fail())
			return;

		_ulong iIndex = m_BoneList.GetCount();

		for (_ulong i = 0; 0 < iIndex; ++i)
		{
			CString strTemp = L"";
			m_BoneList.GetText(i, strTemp);

			fout << strTemp.GetString() << endl;
		}

		fout.close();
	}
	else
		return;

	//WinExec("notepad.exe ../../Data/AniList.txt", SW_SHOW);
	UpdateData(FALSE);
}


void CTabAnimation::OnBnClicked_Visible()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_VisibleBoneName.GetCheck() == 0)
		CValueMgr::bVisibleBone = FALSE;
	else
		CValueMgr::bVisibleBone = TRUE;

	UpdateData(FALSE);
}
