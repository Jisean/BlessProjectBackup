
// MapObjToolView.cpp : CMapObjToolView Ŭ������ ����
//

#include "stdafx.h"
#include "MapObjTool.h"

#include "MapObjToolDoc.h"
#include "MapObjToolView.h"
#include "MainFrm.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Back.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;
#pragma warning(disable : 4996)

// CMapObjToolView

IMPLEMENT_DYNCREATE(CMapObjToolView, CView)

BEGIN_MESSAGE_MAP(CMapObjToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapObjToolView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMapObjToolView ����/�Ҹ�

CMapObjToolView::CMapObjToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMapObjToolView::~CMapObjToolView()
{
}

BOOL CMapObjToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMapObjToolView �׸���

void CMapObjToolView::OnDraw(CDC* /*pDC*/)
{
	CMapObjToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

}


// CMapObjToolView �μ�


void CMapObjToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMapObjToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMapObjToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMapObjToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMapObjToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapObjToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMapObjToolView ����

#ifdef _DEBUG
void CMapObjToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapObjToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapObjToolDoc* CMapObjToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapObjToolDoc)));
	return (CMapObjToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapObjToolView �޽��� ó����

void CMapObjToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame*	pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT rcWindow;


	pMainFrm->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect((&rcMainView));

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(NULL, 
		0, 0, 
		int(WINCX + fRowFrm), 
		int(WINCY + fColFrm),
		SWP_NOZORDER);

	g_hWnd = m_hWnd;

	/////////////Deivce//////////
	if(FAILED(Engine::Ready_GraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX,WINCY)))
	{
		AfxMessageBox(L"Device Create Failed");
		return;
	}
	m_pDevice = Engine::Get_GraphicDev();
	m_pDevice->AddRef();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	///////////////Management/////////////
	if(FAILED(Engine::Ready_Management(m_pDevice)))
	{
		AfxMessageBox(L"Management Init Failed");
		return;
	}

	//////////////////Container////////////////////
	if(FAILED(Engine::Reserve_ContainerSize(RESOURCE_END)))
	{
		AfxMessageBox(L"Resource Container Init Failed");
		return;
	}
	/////////////////////NaviCell//////////////////////
	if(FAILED(Engine::Reserve_CellContainerSize(m_pDevice, 1)))
	{
		AfxMessageBox(L"NaviCell Container Init Failed");
		return;
	}
	//////////////////////Occlusion//////////////////////////
	if(FAILED(Engine::Ready_Occlusion(m_pDevice)))
	{
		AfxMessageBox(L"Occlusion Init Failed");
		return;
	}

	/////////////////Input////////////
	if(FAILED(Engine::Ready_InputDev(AfxGetInstanceHandle(), g_hWnd)))
	{
		AfxMessageBox(L"Input Init Failed");
		return;
	}
	////////////Font///////////////////////
	if(FAILED(Engine::Ready_Font(m_pDevice, L"����", 40, 24, FW_HEAVY)))
	{
		AfxMessageBox(L"Resource Container Init Failed");
		return;
	}
	if(FAILED(Engine::Ready_Font(m_pDevice, L"Comic Sans Ms", 20, 12, FW_HEAVY)))	
	{
		AfxMessageBox(L"Resource Container Init Failed");
		return;
	};
	if(FAILED(Engine::Ready_Font(m_pDevice, L"�ü�", 40, 24, FW_HEAVY)))
	{
		AfxMessageBox(L"Resource Container Init Failed");
		return;
	}

	Engine::Init_Time();

	///////////////Scene Back//////////////////
	if(FAILED(Engine::SceneChange(CSceneSelector(CSceneSelector::SCENE_TOOL))))
	{
		AfxMessageBox(L"Back Init Failed");
		return;
	}

	SetTimer(1, 10, NULL);

	m_iCurToolIndex = 0;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

void CMapObjToolView::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	Engine::Release_Utility();
	Engine::Release_Resource();
	Engine::Release_System();

	CView::PostNcDestroy();
}

void CMapObjToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnTimer(nIDEvent);
	Engine::Set_Time();
	Engine::Update_Management(Engine::Get_Time());
	Engine::Update_InputDev();
	Engine::Render_Management();
}
void CMapObjToolView::SetCurToolIndex(int& iIndex)
{
	m_iCurToolIndex = iIndex;
}
void CMapObjToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	switch(m_iCurToolIndex)
	{
	case 0:
		((CBack*)Engine::Get_Scene())->ConstMapMode();
		break;
	case 1:
		((CBack*)Engine::Get_Scene())->ConstObjectMode();
		//m_pManagement->Update();
		break;
	case 2:
		((CBack*)Engine::Get_Scene())->NavimeshMode();
		break;
	case 3:
		((CBack*)Engine::Get_Scene())->ConstSplatingMode();
		break;
	case 4:
		((CBack*)Engine::Get_Scene())->ConstOcclusionBox();
		break;
	
	}
	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}

void CMapObjToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(GetAsyncKeyState(VK_LBUTTON))
	{
		switch(m_iCurToolIndex)
		{
		case 0:
			//((CBack*)Engine::Get_Scene())->ConstMapMode();
			break;
		}
	}
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}
