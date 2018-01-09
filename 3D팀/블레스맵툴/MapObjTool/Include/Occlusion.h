#pragma once
#include "afxwin.h"
#include "Transform.h"

// COcclusion ��ȭ �����Դϴ�.

class COcclusion : public CDialog
{
	DECLARE_DYNAMIC(COcclusion)

public:
	COcclusion(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COcclusion();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OCCLUSION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void CreateOccBox();
	afx_msg void CopyOccBox();
	CButton OccMode[4];
	float m_fSizeX;
	float m_fSizeY;
	float m_fSizeZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	afx_msg void DoChange();
	afx_msg void RedoChange();
	afx_msg void DeleteOccBox();
	CListBox OccBoxList;
	afx_msg void SaveOccBox();
	afx_msg void LoadOccBox();
	bool	m_bStart;

public:
	void	SetInformation(Engine::CTransform* pInfo);
	void	ResetInformation(void);
	void	AngleWithCamUpdate(void);
	int m_iBoxCnt;
	afx_msg void RotXControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void RotYControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void RotZControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void PosXControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void PosYControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void PosZControl(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_Decimal[4];
	afx_msg void OnLbnSelchange();
	float m_fAngleWithCam;
};
