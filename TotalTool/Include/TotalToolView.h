
// TotalToolView.h : CTotalToolView Ŭ������ �������̽�
//

#pragma once

#include "ToolRender.h"

class CToolRender;
class CTotalToolDoc;
class CTotalToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CTotalToolView();
	DECLARE_DYNCREATE(CTotalToolView)

// Ư���Դϴ�.
public:
	CTotalToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTotalToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:
	TABSTATE m_eTabState;

public:
	virtual void OnInitialUpdate();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

public:
	bool bIsInit;
	bool bSetTex;

public:
	ENGINE::CGraphicDev*	m_pDeviceInstance;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	ENGINE::CTimeMgr*							m_pTime;
	CToolRender*					m_pToolRender;

};

#ifndef _DEBUG  // TotalToolView.cpp�� ����� ����
inline CTotalToolDoc* CTotalToolView::GetDocument() const
   { return reinterpret_cast<CTotalToolDoc*>(m_pDocument); }
#endif

