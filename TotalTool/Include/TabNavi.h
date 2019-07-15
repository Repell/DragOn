#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTabNavi ��ȭ �����Դϴ�.

class CTabNavi : public CDialogEx
{
	DECLARE_DYNAMIC(CTabNavi)

public:
	CTabNavi(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabNavi();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABTHREE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void Insert_Navi(_int idx);
	void Update_Transform();

public:	//func
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


private:	//afx
	afx_msg void OnTvnSelchanged_NaviTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClicked_CreateCheck();

	afx_msg void OnEnChangeEdit_PosX();
	afx_msg void OnEnChangeEdit_PosY();
	afx_msg void OnEnChangeEdit_PosZ();
	afx_msg void OnDeltaposSpin_PosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_PosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_PosZ(NMHDR *pNMHDR, LRESULT *pResult);


	afx_msg void OnBnClicked_SaveNavi();
	afx_msg void OnBnClicked_LoadNavi();
	afx_msg void OnBnClicked_DeleteNavi();

public:
	void Pick_ObjectVtx();

private:

	_int iNaviNumb;
	//Tree Ctn
	CTreeCtrl m_TreeList;
	HTREEITEM hLevel1;
	HTREEITEM hLevel2;
	_int iNaviNum;
	_int iTriNum;

	//Navi Transform
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;
	_float m_fPosX;
	_float m_fPosY;
	_float m_fPosZ;

	BOOL m_bClick;
	CButton Check_MakeLine;

public:
	_int m_iMeshIndex;
	CString strCurType;
	CString strCurMesh;
};
