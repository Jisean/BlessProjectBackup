#pragma once
#include "afxwin.h"


// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	CString m_CntX;
	CString m_CntZ;
	CString m_Itv;
	CString m_PickSize;
	CString m_SetHeight;
	CString m_UVCnt;
	int		m_iCntX;
	int		m_iCntZ;
	int		m_iItv;
	float	m_fSetHeight;
	float	m_fUVCnt;

	int m_iTextureMount;
	CString			m_TempPathName;
	TCHAR			m_szBuffer[MAX_PATH];
	CString			m_FolderName;
	bool			m_bStart;
	bool			m_bTextureLoad;
	double			m_dPickSize;
	map<CString, CImage*> m_MapTileImage;

public:
	CListBox m_MapTextureList;
	CButton m_MapEditMode[2];
	CButton m_ModifyMode[3];
	CButton m_Gradient[3];
	CButton m_MainHeightMode[2];
	CButton m_SoftHeightMode[2];
	CButton m_SmoothUpDown[2];
	CButton m_Pick2Check;
public:
	afx_msg void OnCreateTerrain();
	afx_msg void TextureSetting();
	afx_msg void SaveTerrain();
	afx_msg void LoadTerrain();
	afx_msg void OnVtxX();
	afx_msg void OnVtxZ();
	afx_msg void OnInterval();
	afx_msg void OnPickSize();
	afx_msg void OnHeight();
	
	afx_msg void OnUVCount();
	
	CButton m_WireFrame;
	afx_msg void OnWireFrameCheck();
	float m_fSmoothPower;
};
