// Pathfind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "Pathfind.h"


// CPathfind 대화 상자입니다.

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


// CPathfind 메시지 처리기입니다.

void CPathfind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

}
