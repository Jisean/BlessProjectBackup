// Pathfind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "Pathfind.h"


// CPathfind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathfind, CDialog)

CPathfind::CPathfind(CWnd* pParent /*=NULL*/)
	: CDialog(CPathfind::IDD, pParent)
{

}

CPathfind::~CPathfind()
{
}

void CPathfind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPathfind, CDialog)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathfind �޽��� ó�����Դϴ�.

void CPathfind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

}
