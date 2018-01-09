
// MapObjToolView.h : CMapObjToolView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CMapObjToolView();
	DECLARE_DYNCREATE(CMapObjToolView)

// Ư���Դϴ�.
public:
	CMapObjToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	int m_iCurToolIndex;

private:
	LPDIRECT3DDEVICE9		m_pDevice;

public:
	void	SetCurToolIndex(int& iIndex);

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
	virtual ~CMapObjToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // MapObjToolView.cpp�� ����� ����
inline CMapObjToolDoc* CMapObjToolView::GetDocument() const
   { return reinterpret_cast<CMapObjToolDoc*>(m_pDocument); }
#endif

