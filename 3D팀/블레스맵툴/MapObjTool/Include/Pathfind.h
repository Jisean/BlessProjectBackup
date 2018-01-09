#pragma once


// CPathfind 대화 상자입니다.

class CPathfind : public CDialog
{
	DECLARE_DYNAMIC(CPathfind)

public:
	CPathfind(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathfind();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATHFIND };

public:
	struct MESH_PATH
	{
		TCHAR*		tcMeshKey;
		TCHAR*		tcMeshFile;
		TCHAR*		tcMeshPath;
	};

public:
	list <MESH_PATH> m_PathList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
