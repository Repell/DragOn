
// TotalTool.h : TotalTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CTotalToolApp:
// �� Ŭ������ ������ ���ؼ��� TotalTool.cpp�� �����Ͻʽÿ�.
//

class CTotalToolApp : public CWinAppEx
{
public:
	CTotalToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTotalToolApp theApp;
