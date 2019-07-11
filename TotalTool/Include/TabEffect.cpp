// TabEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "TabEffect.h"
#include "afxdialogex.h"


// CTabEffect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabEffect, CDialogEx)

CTabEffect::CTabEffect(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TABFOUR, pParent)
{

}

CTabEffect::~CTabEffect()
{
}

void CTabEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabEffect, CDialogEx)
END_MESSAGE_MAP()


// CTabEffect 메시지 처리기입니다.


BOOL CTabEffect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CTabEffect::PreTranslateMessage(MSG* pMsg)
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
