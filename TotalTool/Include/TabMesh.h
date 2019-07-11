#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTabMesh 대화 상자입니다.

class CTabMesh : public CDialogEx
{
	DECLARE_DYNAMIC(CTabMesh)

public:
	CTabMesh(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabMesh();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABTWO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	//Initialize
	virtual BOOL OnInitDialog();
	//Esc X
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//Func
	void ImportMeshFolder();
	void Release_MapMesh(_ushort eType);
	void Update_Transform();

	//Afx
	afx_msg void OnBnClicked_RenderSolid();
	afx_msg void OnBnClicked_RenderWireFrm();

	//Tree Controll
	afx_msg void OnTvnSelchanged_Item(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClicked_AddMesh();		//Static
	afx_msg void OnBnClicked_DeleteStatic();
	afx_msg void OnBnClicked_AddDynamic();
	afx_msg void OnBnClicked_DeleteDynamic();

	//Mesh Transform Spin Controll
	afx_msg void OnDeltaposSpin_PosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_PosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_PosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_RotX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_RotY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_RotZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_ScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_ScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_ScaleZ(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnChangeEdit_PosX();
	afx_msg void OnEnChangeEdit_PosY();
	afx_msg void OnEnChangeEdit_PosZ();
	afx_msg void OnEnChangeEdit_RotX();
	afx_msg void OnEnChangeEdit_RotY();
	afx_msg void OnEnChangeEdit_RotZ();
	afx_msg void OnEnChangeEdit_ScaleX();
	afx_msg void OnEnChangeEdit_ScaleY();
	afx_msg void OnEnChangeEdit_ScaleZ();

	//Pick Object
	afx_msg void OnLbnSelchange_PickStaticObj();
	afx_msg void OnLbnSelchange_PickDynamicObj();

	//Save & Load Static Mesh List
	afx_msg void OnBnClicked_SaveMesh();
	afx_msg void OnBnClicked_LoadMesh();
	afx_msg void OnBnClicked_SaveMesh_Dynamic();
	afx_msg void OnBnClicked_LoadMesh_Dynamic();

private:
	CTreeCtrl m_TreeList;
	CFileFind Finder;
	//list<ENGINE::MESH_INFO*> m_MeshList;
	map<wstring, ENGINE::MESH_INFO*> m_mapMesh[3];
	CString m_strCurrentMesh;

	HTREEITEM hLevel1;				//Mesh
	//HTREEITEM	 hLevel2;	//[0]Static / [1] Dynamic

private:
	//Render 
	CButton m_RenderRadio[2];

	//Mehs ListBox
	CListBox m_staticBox;
	CListBox m_DynamicBox;
	_bool bMeshSwitch; // 0 Static 1 Dynamic
	_int m_iIndex;

	//Mesh Transform
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;
	_float m_fPosX;
	_float m_fPosY;
	_float m_fPosZ;

	CString m_strRotX;
	CString m_strRotY;
	CString m_strRotZ;
	_float m_fRotX;
	_float m_fRotY;
	_float m_fRotZ;

	CString m_strScaleX;
	CString m_strScaleY;
	CString m_strScaleZ;
	_float m_fScaleX;
	_float m_fScaleY;
	_float m_fScaleZ;

	//Mesh State
	CEdit m_MeshType;
	CEdit m_MeshName;

public:

};
