
// TotalToolView.h : CTotalToolView 클래스의 인터페이스
//

#pragma once

#include "ToolRender.h"

class CToolRender;
class CTotalToolDoc;
class CTotalToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CTotalToolView();
	DECLARE_DYNCREATE(CTotalToolView)

// 특성입니다.
public:
	CTotalToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTotalToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // TotalToolView.cpp의 디버그 버전
inline CTotalToolDoc* CTotalToolView::GetDocument() const
   { return reinterpret_cast<CTotalToolDoc*>(m_pDocument); }
#endif

