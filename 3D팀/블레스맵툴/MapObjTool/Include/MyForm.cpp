// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "MapObjToolView.h"
#include "MapTool.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{
	m_pWndShow = NULL;
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_WM_KEYDOWN()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnToolTabChange)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//m_Font.CreatePointFont(180, L"궁서");
	//GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);

	CString str1 = _T("Terrain");
	CString str2 = _T("Object");
	CString str3 = _T("NaviMesh");
	CString str4 = _T("Splatting");
	CString str5 = _T("TriggerBox");

	m_Tab.InsertItem(1, str1);
	m_Tab.InsertItem(2, str2);
	m_Tab.InsertItem(3, str3);
	m_Tab.InsertItem(4, str4);
	m_Tab.InsertItem(5, str5);

	CRect Rect;
	m_Tab.GetClientRect(&Rect);

	m_Tab1.Create(IDD_MAPTOOL, &m_Tab);
	m_Tab1.SetWindowPos(NULL, 5, 25, Rect.Width() - 12, Rect.Height() - 33,	SWP_SHOWWINDOW | SWP_NOZORDER);

	m_Tab2.Create(IDD_OBJECTTOOL, &m_Tab);
	m_Tab2.SetWindowPos(NULL, 5, 25, Rect.Width(), Rect.Height() - 33,	SWP_NOZORDER);

	m_Tab3.Create(IDD_NAVIMESHTOOL, &m_Tab);
	m_Tab3.SetWindowPos(NULL, 5, 25, Rect.Width() - 12, Rect.Height() - 33,	SWP_NOZORDER);

	m_Tab4.Create(IDD_SPLATINGTOOL, &m_Tab);
	m_Tab4.SetWindowPos(NULL, 5, 25, Rect.Width() - 12, Rect.Height() - 33,	SWP_NOZORDER);

	m_Tab5.Create(IDD_OCCLUSION, &m_Tab);
	m_Tab5.SetWindowPos(NULL, 5, 25, Rect.Width() - 12, Rect.Height() - 33,	SWP_NOZORDER);
}

void CMyForm::OnToolTabChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_pWndShow != NULL)
	{
		m_pWndShow->ShowWindow(SW_HIDE);
		m_pWndShow = NULL;
	}

	int iIndex = m_Tab.GetCurSel();

	switch(iIndex)
	{
	case 0:
		m_Tab1.ShowWindow(SW_SHOW);
		m_pWndShow = &m_Tab1;
		break;
	case 1:
		m_Tab2.ShowWindow(SW_SHOW);
		m_pWndShow = &m_Tab2;
		break;
	case 2:
		m_Tab3.ShowWindow(SW_SHOW);
		m_pWndShow = &m_Tab3;
		break;
	case 3:
		m_Tab4.ShowWindow(SW_SHOW);
		m_pWndShow = &m_Tab4;
		break;
	case 4:
		m_Tab5.ShowWindow(SW_SHOW);
		m_pWndShow = &m_Tab5;
		break;
	}
	m_pView->SetCurToolIndex(iIndex);

	*pResult = 0;
	UpdateData(FALSE);
}
