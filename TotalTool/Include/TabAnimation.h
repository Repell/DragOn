#pragma once
#include "afxwin.h"


// CTabAnimation 대화 상자입니다.

class CTabAnimation : public CDialogEx
{
	DECLARE_DYNAMIC(CTabAnimation)

public:
	CTabAnimation(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabAnimation();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	map<wstring, _matrix*>& Get_mapBones();

private:
	void Update_Transform();
	void Add_MeshFrameList();

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnBnClicked_ExportAniList();
	afx_msg void OnBnClicked_BoneList();
	afx_msg void OnBnClicked_LoadXFile();
	afx_msg void OnBnClicked_DeleteXFile();

	afx_msg void OnBnClicked_Visible();

	afx_msg void OnEnChangeEdit_PosX();
	afx_msg void OnEnChangeEdit_PosY();
	afx_msg void OnEnChangeEdit_PosZ();
	afx_msg void OnEnChangeEdit_ScaleX();
	afx_msg void OnEnChangeEdit_ScaleY();
	afx_msg void OnEnChangeEdit_ScaleZ();
	afx_msg void OnDeltaposSpin_PosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_PosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_PosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_ScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_ScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin_ScaleZ(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnLbnSelchange_BoneList();
	afx_msg void OnLbnSelchange_AniList();
private:
	CButton m_VisibleBoneName;
	//vector<_vec3> m_vecBone;
	map<wstring, _matrix*> m_mapBone;
	//ENGINE::CDynamicMesh* m_pMesh;
	CEdit m_CurXFile;
	CListBox m_AniList;
	CListBox m_BoneList;

	ENGINE::MESH_INFO m_MeshInfo;	//Current Mesh INFO
	

	CString strPosX;
	CString strPosY;
	CString strPosZ;
	_float m_fPosX;
	_float m_fPosY;
	_float m_fPosZ;

	CString strScaleX;
	CString strScaleY;
	CString strScaleZ;
	_float m_fScaleX;
	_float m_fScaleY;
	_float m_fScaleZ;


};
