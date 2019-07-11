// TabEffect.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TotalTool.h"
#include "TabEffect.h"
#include "afxdialogex.h"


// CTabEffect ��ȭ �����Դϴ�.

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


// CTabEffect �޽��� ó�����Դϴ�.


BOOL CTabEffect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CTabEffect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
