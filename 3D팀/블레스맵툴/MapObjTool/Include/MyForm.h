#pragma once

#include <afxcmn.h>
#include "MapTool.h"
#include "ObjectTool.h"
#include "NaviMeshTool.h"
#include "SplatingTool.h"
#include "Occlusion.h"

// CMyForm 폼 뷰입니다.

class CMapObjToolView;

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
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
	CTabCtrl m_Tab;

public:
	CMapObjToolView* m_pView;
	CFont			m_Font;
	CWnd*			m_pWndShow;
	CMapTool		m_Tab1;
	CObjectTool		m_Tab2;
	CNaviMeshTool	m_Tab3;
	CSplatingTool	m_Tab4;
	COcclusion		m_Tab5;
	afx_msg void OnToolTabChange(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


