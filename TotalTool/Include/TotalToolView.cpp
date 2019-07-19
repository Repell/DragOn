
// TotalToolView.cpp : CTotalToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TotalTool.h"
#endif

#include "TotalToolDoc.h"
#include "TotalToolView.h"

#include <crtdbg.h>

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTotalToolView

IMPLEMENT_DYNCREATE(CTotalToolView, CScrollView)

BEGIN_MESSAGE_MAP(CTotalToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

HWND g_hWnd;
HINSTANCE g_hInst;
_float fWheel;

// CTotalToolView 생성/소멸

CTotalToolView::CTotalToolView()
	:bIsInit(FALSE), bSetTex(FALSE),
	m_pDeviceInstance(nullptr), m_pGraphicDev(nullptr), m_pToolRender(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

	
}

CTotalToolView::~CTotalToolView()
{
	//KillTimer(0);

	ENGINE::Safe_Delete(m_pToolRender);

	m_pTime->Get_Instance()->Destroy_Instance();
	ENGINE::Safe_Release(m_pGraphicDev);

	ENGINE::Release_Utility();	//No Management
	//ENGINE::Release_Resources();
	ENGINE::Release_System();
}

BOOL CTotalToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CTotalToolView 그리기

void CTotalToolView::OnDraw(CDC* /*pDC*/)
{
	CTotalToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTotalToolView 인쇄

BOOL CTotalToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTotalToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTotalToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTotalToolView 진단

#ifdef _DEBUG
void CTotalToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTotalToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTotalToolDoc* CTotalToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTotalToolDoc)));
	return (CTotalToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTotalToolView 메시지 처리기


void CTotalToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	g_hWnd = m_hWnd;
	g_hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);

	SetScrollSizes(MM_TEXT, CSize(WINCX, WINCY));
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	RECT rc = {};
	pMain->GetWindowRect(&rc);
	RECT rcView = {};
	this->GetClientRect(&rcView);

	::SetRect(&rc, 0, 0, RESIZEX, RESIZEY);

	int iROW = rc.right - rcView.right;
	int iCOL = rc.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iROW, WINCY + iCOL, SWP_NOZORDER);

	if (FAILED(ENGINE::Ready_GraphicDev(g_hWnd, ENGINE::CGraphicDev::MODE_WIN, WINCX, WINCY, &m_pDeviceInstance)))
	{
		AfxMessageBox(L"Graphic Dev Init Error!");
		return;
	}

	m_pGraphicDev = m_pDeviceInstance->Get_Device();
	m_pGraphicDev->AddRef();

	//Init ToolRender
	if (FAILED(m_pToolRender = CToolRender::Create(m_pGraphicDev)))
	{
		AfxMessageBox(L"ToolRender Init Error!");
		return;
	}

	//Init Time
	m_pTime->Get_Instance()->Init_Time();

	//Default Font
	ENGINE::Ready_Font(m_pGraphicDev, L"Sp", L"스플래2K", 22, 24, FW_NORMAL);

	//Key
	ENGINE::Ready_InputDev(g_hInst, g_hWnd);

	//OnTimer Init
	SetTimer(1, 20, NULL);
	bIsInit = true;
}


void CTotalToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnTimer(nIDEvent);
	g_hWnd = m_hWnd;
	g_hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);



	RECT rcView = {};
	this->GetClientRect(&rcView);
	//Mouse Pos에 vRatio 만큼 곱해줘야 좌표가 정확히 맞음
	CValueMgr::vRatio = _vec2(WINCX / float(rcView.right), WINCY / float(rcView.bottom));

	if (bIsInit)
	{
		//Update
		ENGINE::KeyInput();
		ENGINE::Set_InputDev();
		m_pTime->Get_Instance()->Update_Time();
		m_pToolRender->Update_Render(m_pTime->Get_Instance()->Get_DeltaTime());

		//Late Update
		m_pToolRender->Late_Update_Render();

		//ToolRender 생성 시 작동

		m_pDeviceInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

		m_pToolRender->Render();

		m_pDeviceInstance->Render_End();
	}
	if (!bSetTex)
	{
		m_pToolRender->Set_TextureString();
		bSetTex = true;
	}
}


BOOL CTotalToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	fWheel = _float(zDelta);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
