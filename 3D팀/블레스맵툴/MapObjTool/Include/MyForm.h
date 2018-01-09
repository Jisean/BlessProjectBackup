#pragma once

#include <afxcmn.h>
#include "MapTool.h"
#include "ObjectTool.h"
#include "NaviMeshTool.h"
#include "SplatingTool.h"
#include "Occlusion.h"

// CMyForm �� ���Դϴ�.

class CMapObjToolView;

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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


