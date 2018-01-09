#pragma once
#include "afxwin.h"


// CNaviMeshTool 대화 상자입니다.
namespace Engine
{
	class CNaviCell;
}

class CNaviMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CNaviMeshTool)

public:
	CNaviMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNaviMeshTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NAVIMESHTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SetPickPos(D3DXVECTOR3 vIndex);
	void SetPickCell(Engine::CNaviCell* pCell);
public:
	int m_iNaviCnt;
	bool m_bStart;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	D3DXVECTOR3 m_vPointA;
	D3DXVECTOR3 m_vPointB;
	D3DXVECTOR3 m_vPointC;
	Engine::CNaviCell* m_pCurCell;
public:
	CButton m_NaviMode[4];
	CButton m_CreateMode[2];
	CButton m_DeleteMode[2];
	CButton m_EditMode[2];
	CButton m_PickingMode[2];
	CButton m_PosDecimal[6];
	CButton m_CellType[3];
	CButton m_SwapOption[3];
	afx_msg void OnNaveSave();
	afx_msg void OnNaviLoad();
	afx_msg void OnPointAposX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointAposY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointAposZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointBposX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointBposY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointBposZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointCposX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointCposY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPointCposZ(NMHDR *pNMHDR, LRESULT *pResult);
	
	float m_fNearRange;
	CButton m_WireFrame;
	afx_msg void OnWireFrameCheck();
	CButton m_ObjectWire;
	afx_msg void OnObjectWire();
	
	afx_msg void OnSwapPoint();
	afx_msg void OnCellEdit();
};
