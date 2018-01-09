#pragma once


// CPathfind ��ȭ �����Դϴ�.

class CPathfind : public CDialog
{
	DECLARE_DYNAMIC(CPathfind)

public:
	CPathfind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathfind();

// ��ȭ ���� �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
