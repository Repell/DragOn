#pragma once


// CTabEffect ��ȭ �����Դϴ�.

class CTabEffect : public CDialogEx
{
	DECLARE_DYNAMIC(CTabEffect)

public:
	CTabEffect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabEffect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABFOUR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
