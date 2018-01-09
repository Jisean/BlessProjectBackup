
// MapObjToolView.h : CMapObjToolView 클래스의 인터페이스
//


#pragma once

#include "Include.h"
#include "Engine_Include.h"

namespace Engine
{
	class CLayer;
	class CManagement;
	class CResourceMgr;
	class CTimeMgr;
	class CGraphicDev;
}

class CMapObjToolDoc;
class CMapObjToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMapObjToolView();
	DECLARE_DYNCREATE(CMapObjToolView)

// 특성입니다.
public:
	CMapObjToolDoc* GetDocument() const;

// 작업입니다.
public:
	int m_iCurToolIndex;

private:
	LPDIRECT3DDEVICE9		m_pDevice;

public:
	void	SetCurToolIndex(int& iIndex);

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
	virtual ~CMapObjToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MapObjToolView.cpp의 디버그 버전
inline CMapObjToolDoc* CMapObjToolView::GetDocument() const
   { return reinterpret_cast<CMapObjToolDoc*>(m_pDocument); }
#endif

