#pragma once
#include "afxwin.h"


// CSplatingTool ��ȭ �����Դϴ�.

class CSplatingTool : public CDialog
{
	DECLARE_DYNAMIC(CSplatingTool)

public:
	CSplatingTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSplatingTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SPLATINGTOOL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
