#pragma once
#include "afxwin.h"


// CSplatingTool 대화 상자입니다.

class CSplatingTool : public CDialog
{
	DECLARE_DYNAMIC(CSplatingTool)

public:
	CSplatingTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSplatingTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLATINGTOOL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	bool m_bStart;
	bool m_bTexLoad;
public:
	CString m_AlphaRadius;
	CString m_BrushSize;
	float m_fBrushSize;
	float m_fAlphaRadius;
	CButton m_SplatingMode[3];
	int	 m_iCurrentFilter;
	CListBox m_TileList;
public:
	afx_msg void OnTileList();
	afx_msg void OnEraseSplating();
	afx_msg void OnBrushSize();
	afx_msg void OnAlphaRadius();
	CStatic m_Picture;
	afx_msg void OnDrawingEnd();
	afx_msg void OnDeleteSplating();
	afx_msg void OnSaveSplating();

	void ActivateButton();
};
