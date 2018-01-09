#pragma once
#include "afxwin.h"
#include "Resource.h"


// CObjectTool 대화 상자입니다.

namespace Engine
{
	class CGameObject;
	class CTransform;
}

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJECTTOOL };

public:
	CString m_strCurKey;
	bool	m_bStart;
	bool	m_bFirstLoad;
	bool	m_bFirstDynamicLoad;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fTransX;
	float m_fTransY;
	float m_fTransZ;
	Engine::CGameObject*	m_pCurObject;
	Engine::CTransform*		m_pBeforeTrans;
	vector<TCHAR*> vecFilename, vecFileKey, vecFilePath;
	vector<TCHAR*> vecTexKey, vecTexPath;


public:
	void	SetCurObject(Engine::CGameObject* pObject = NULL)
	{
		m_pCurObject = pObject;
	}
	void	SetPickPos(D3DXVECTOR3 vPos);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStaticListSel();
	afx_msg void OnDynamicListSel();
	afx_msg void OnInfoReset();
	afx_msg void OnInfoEdit();
	afx_msg void OnResetStaticList();
	afx_msg void OnResetDynamicList();
	afx_msg void OnObjSave();
	afx_msg void OnObjLoad();
	afx_msg void OnLoadStaticMesh();
	afx_msg void OnLoadDynamicMesh();

public:
	CButton m_StaticAreaRadio[2];
	CButton m_ObjModeRadio[5];
	CButton m_ObjTypeRadio[2];
	CButton m_PosDecimal[6];
	CListBox m_StaticList;
	CListBox m_DynamicList;
	
	float fScaleX;
	float fScaleY;
	float fScaleZ;
	float fRotX;
	float fRotY;
	float fRotZ;
	float fPosX;
	float fPosY;
	float fPosZ;
	afx_msg void OnRotXControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRotYControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRotZControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPosXControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPosYControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPosZControl(NMHDR *pNMHDR, LRESULT *pResult);
	
	CString m_stCurrentMeshKey;
	CButton m_WireFrame;
	afx_msg void OnWireFrameCheck();
	afx_msg void OnPotentialCollision();
	CButton m_PotentialCollision;
	CButton m_ColDisvisable;
};
