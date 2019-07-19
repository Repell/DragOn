#pragma once
#include "afxcmn.h"

#include "TapTerrain.h"	//ONE
#include "TabMesh.h"		//TWO
#include "TabNavi.h"		//THREE
#include "TabEffect.h"	//FOUR
#include "TabAnimation.h" //FIVE	

// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

public:
	enum eTab_ID
	{

	};

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	afx_msg void OnTcnSelchange_Tab(NMHDR *pNMHDR, LRESULT *pResult);

public:
	TABSTATE m_eTab;
	CTabCtrl m_Tab;

	CWnd* m_pWndShow;
	CWnd* m_WndCpy;

	CString m_TabONE;
	CString m_TabTWO;
	CString m_TabTHREE;
	CString m_TabFOUR;
	CString m_TabFIVE;

	CTapTerrain m_pTapTerrain;
	CTabMesh		m_pTabMesh;
	CTabNavi		m_pTabNavi;
	CTabEffect	m_pTabEffect;
	CTabAnimation		m_pTabAnimation;

};


