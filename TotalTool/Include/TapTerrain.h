#pragma once
#include "afxwin.h"


// CTapTerrain 대화 상자입니다.

class CTapTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(CTapTerrain)

public:
	CTapTerrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTapTerrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABONE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Add_TextureString(const _tchar* pStr, _int pNum);

private:
	//Initialize
	virtual BOOL OnInitDialog();

	//Terrain Render Option
	afx_msg void OnBnClicked_RenderSolid();
	afx_msg void OnBnClicked_RenderWireframe();
	afx_msg void OnBnClicked_RenderAxis();
	afx_msg void OnBnClicked_NoRender();
	
	//Terrain Size Controll Func
	afx_msg void OnDeltapos_Spin_SizeX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_SizeZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_Interval(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_Detail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClicked_SetTerrain();
	afx_msg void OnBnClicked_ResetTerrain();

	//Terrain Pos /Rot Control Func
	afx_msg void OnDeltapos_Spin_PosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_PosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_PosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_RotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_RotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltapos_Spin_RotZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChange_SetPosX();
	afx_msg void OnEnChange_SetPosY();
	afx_msg void OnEnChangeEdit_SetPosZ();
	afx_msg void OnEnChangeEdit_SetRotX();
	afx_msg void OnEnChangeEdit_SetRotY();
	afx_msg void OnEnChangeEdit_SetRotZ();

	//Terrain Texture ListBox Controll Func
	afx_msg void OnLbnSelchange_TextureBox();
	
	//Terrain Save & Load
	afx_msg void OnBnClicked_SaveTerrain();
	afx_msg void OnBnClicked_LoadTerrain();

	//Tap Controll
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	//Render State
	CButton m_renderRadio[4];

	//Texture ListBox
	CListBox m_TextureListBox;

	//Terrain Size Controll Value
	CString m_strSize_X;
	CString m_strSize_Z;
	CString m_strInterval;
	CString m_strDetail;
	WORD m_uSizeX;
	WORD m_uSizeZ;
	WORD m_uInterval;
	WORD m_uDetail;

	//Terrain Pos / Rot
	CString m_strPos_X;
	CString m_strPos_Y;
	CString m_strPos_Z;
	CString m_strRot_X;
	CString m_strRot_Y;
	CString m_strRot_Z;
	_float m_fPosX;
	_float m_fPosY;
	_float m_fPosZ;
	_float m_fRotX;
	_float m_fRotY;
	_float m_fRotZ;



public:
	_uint m_iTex;
	CString strKey;



};
