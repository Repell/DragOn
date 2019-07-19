
// TotalToolView.cpp : CTotalToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

HWND g_hWnd;
HINSTANCE g_hInst;
_float fWheel;

// CTotalToolView ����/�Ҹ�

CTotalToolView::CTotalToolView()
	:bIsInit(FALSE), bSetTex(FALSE),
	m_pDeviceInstance(nullptr), m_pGraphicDev(nullptr), m_pToolRender(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CTotalToolView �׸���

void CTotalToolView::OnDraw(CDC* /*pDC*/)
{
	CTotalToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CTotalToolView �μ�

BOOL CTotalToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTotalToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTotalToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CTotalToolView ����

#ifdef _DEBUG
void CTotalToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTotalToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTotalToolDoc* CTotalToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTotalToolDoc)));
	return (CTotalToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTotalToolView �޽��� ó����


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
	ENGINE::Ready_Font(m_pGraphicDev, L"Sp", L"���÷�2K", 22, 24, FW_NORMAL);

	//Key
	ENGINE::Ready_InputDev(g_hInst, g_hWnd);

	//OnTimer Init
	SetTimer(1, 20, NULL);
	bIsInit = true;
}


void CTotalToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnTimer(nIDEvent);
	g_hWnd = m_hWnd;
	g_hInst = (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE);



	RECT rcView = {};
	this->GetClientRect(&rcView);
	//Mouse Pos�� vRatio ��ŭ ������� ��ǥ�� ��Ȯ�� ����
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

		//ToolRender ���� �� �۵�

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	fWheel = _float(zDelta);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
