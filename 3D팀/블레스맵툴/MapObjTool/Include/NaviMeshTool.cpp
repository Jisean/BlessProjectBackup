// NaviMeshTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "NaviMeshTool.h"
#include "..\Engine\Utility\Code\NaviCell.h"
#include "Export_Function.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "MyForm.h"


// CNaviMeshTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviMeshTool, CDialog)

CNaviMeshTool::CNaviMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(CNaviMeshTool::IDD, pParent)
	, m_iNaviCnt(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_vPointA(0.f, 0.f, 0.f)
	, m_vPointB(0.f, 0.f, 0.f)
	, m_vPointC(0.f, 0.f, 0.f)
	, m_bStart(true)
	, m_pCurCell(NULL)
	, m_fNearRange(0)
{

}

CNaviMeshTool::~CNaviMeshTool()
{
}

void CNaviMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iNaviCnt);
	DDX_Control(pDX, IDC_RADIO1, m_NaviMode[0]);
	DDX_Control(pDX, IDC_RADIO2, m_NaviMode[1]);
	DDX_Control(pDX, IDC_RADIO3, m_NaviMode[2]);
	DDX_Control(pDX, IDC_RADIO4, m_NaviMode[3]);
	DDX_Control(pDX, IDC_RADIO5, m_CreateMode[0]);
	DDX_Control(pDX, IDC_RADIO6, m_CreateMode[1]);
	DDX_Control(pDX, IDC_RADIO7, m_DeleteMode[0]);
	DDX_Control(pDX, IDC_RADIO8, m_DeleteMode[1]);
	DDX_Control(pDX, IDC_RADIO9, m_PickingMode[0]);
	DDX_Control(pDX, IDC_RADIO10, m_PickingMode[1]);
	DDX_Control(pDX, IDC_RADIO11, m_EditMode[0]);
	DDX_Control(pDX, IDC_RADIO12, m_EditMode[1]);
	DDX_Control(pDX, IDC_RADIO13, m_PosDecimal[0]);
	DDX_Control(pDX, IDC_RADIO14, m_PosDecimal[1]);
	DDX_Control(pDX, IDC_RADIO15, m_PosDecimal[2]);
	DDX_Control(pDX, IDC_RADIO16, m_PosDecimal[3]);
	DDX_Control(pDX, IDC_RADIO17, m_PosDecimal[4]);
	DDX_Control(pDX, IDC_RADIO18, m_PosDecimal[5]);
	DDX_Text(pDX, IDC_EDIT2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT3, m_fPosY);
	DDX_Text(pDX, IDC_EDIT4, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT5, m_vPointA.x);
	DDX_Text(pDX, IDC_EDIT6, m_vPointA.y);
	DDX_Text(pDX, IDC_EDIT7, m_vPointA.z);
	DDX_Text(pDX, IDC_EDIT8, m_vPointB.x);
	DDX_Text(pDX, IDC_EDIT9, m_vPointB.y);
	DDX_Text(pDX, IDC_EDIT10, m_vPointB.z);
	DDX_Text(pDX, IDC_EDIT11, m_vPointC.x);
	DDX_Text(pDX, IDC_EDIT12, m_vPointC.y);
	DDX_Text(pDX, IDC_EDIT13, m_vPointC.z);
	DDX_Text(pDX, IDC_EDIT14, m_fNearRange);
	DDX_Control(pDX, IDC_RADIO19, m_CellType[0]);
	DDX_Control(pDX, IDC_RADIO20, m_CellType[1]);
	DDX_Control(pDX, IDC_RADIO21, m_CellType[2]);
	DDX_Control(pDX, IDC_RADIO22, m_SwapOption[0]);
	DDX_Control(pDX, IDC_RADIO23, m_SwapOption[1]);
	DDX_Control(pDX, IDC_RADIO24, m_SwapOption[2]);

	if(m_bStart == true)
	{
		m_NaviMode[0].SetCheck(TRUE);
		m_CreateMode[0].SetCheck(TRUE);
		m_DeleteMode[0].SetCheck(TRUE);
		m_PickingMode[0].SetCheck(TRUE);
		m_EditMode[0].SetCheck(TRUE);
		m_PosDecimal[0].SetCheck(TRUE);
		m_CellType[0].SetCheck(TRUE);
		m_SwapOption[0].SetCheck(TRUE);
		m_bStart = false;
	}



	DDX_Control(pDX, IDC_CHECK1, m_WireFrame);

	DDX_Control(pDX, IDC_CHECK2, m_ObjectWire);
}


BEGIN_MESSAGE_MAP(CNaviMeshTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviMeshTool::OnNaveSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviMeshTool::OnNaviLoad)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CNaviMeshTool::OnPointAposX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CNaviMeshTool::OnPointAposY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CNaviMeshTool::OnPointAposZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CNaviMeshTool::OnPointBposX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CNaviMeshTool::OnPointBposY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CNaviMeshTool::OnPointBposZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CNaviMeshTool::OnPointCposX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CNaviMeshTool::OnPointCposY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CNaviMeshTool::OnPointCposZ)
	ON_BN_CLICKED(IDC_CHECK1, &CNaviMeshTool::OnWireFrameCheck)
	ON_BN_CLICKED(IDC_CHECK2, &CNaviMeshTool::OnObjectWire)
	ON_BN_CLICKED(IDC_BUTTON3, &CNaviMeshTool::OnSwapPoint)
	ON_BN_CLICKED(IDC_BUTTON4, &CNaviMeshTool::OnCellEdit)
END_MESSAGE_MAP()


// CNaviMeshTool 메시지 처리기입니다.

void CNaviMeshTool::SetPickPos(D3DXVECTOR3 vIndex)
{
	UpdateData(TRUE);

	m_fPosX = vIndex.x;
	m_fPosY = vIndex.y;
	m_fPosZ = vIndex.z;

	UpdateData(FALSE);
}

void CNaviMeshTool::SetPickCell(Engine::CNaviCell* pCell)
{
	UpdateData(TRUE);

	if(pCell != NULL)
	{
		m_pCurCell = pCell;

		m_vPointA.x = pCell->GetPoint(Engine::CNaviCell::POINT_A)->x;
		m_vPointA.y = pCell->GetPoint(Engine::CNaviCell::POINT_A)->y;
		m_vPointA.z = pCell->GetPoint(Engine::CNaviCell::POINT_A)->z;

		m_vPointB.x = pCell->GetPoint(Engine::CNaviCell::POINT_B)->x;
		m_vPointB.y = pCell->GetPoint(Engine::CNaviCell::POINT_B)->y;
		m_vPointB.z = pCell->GetPoint(Engine::CNaviCell::POINT_B)->z;

		m_vPointC.x = pCell->GetPoint(Engine::CNaviCell::POINT_C)->x;
		m_vPointC.y = pCell->GetPoint(Engine::CNaviCell::POINT_C)->y;
		m_vPointC.z = pCell->GetPoint(Engine::CNaviCell::POINT_C)->z;
	}
	else
	{
		m_pCurCell = NULL;

		m_vPointA.x = 0.f;
		m_vPointA.y = 0.f;
		m_vPointA.z = 0.f;

		m_vPointB.x = 0.f;
		m_vPointB.y = 0.f;
		m_vPointB.z = 0.f;

		m_vPointC.x = 0.f;
		m_vPointC.y = 0.f;
		m_vPointC.z = 0.f;
	}

	UpdateData(FALSE);

}

void CNaviMeshTool::OnNaveSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"*.dat", this);

	// 1. 저장 모드 지원 2. 파일의 확장자명 3. 최초에 띄워 줄 문자열 
	// 4. 중복 파일 처리에 대한 매개 변수 5. 저장 형식에 따른 확장자명

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	SetCurrentDirectory(szDirPath);
	// 응용 프로그램의 현재 작업 디렉토리를 지정된 디렉토리로 설정해주는 함수

	PathRemoveFileSpec(szDirPath);
	// 마지막 경로를 제외한 경로를 추출하는 함수

	lstrcat(szDirPath, L"..\\..\\Resources\\Data");

	Dlg.m_ofn.lpstrInitialDir = szDirPath;		// 상대 경로

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	typedef vector<Engine::CNaviCell*>		VECCELL;
	VECCELL vecNavi = Engine::Get_Cell();
	VECCELL::iterator iter = vecNavi.begin();

	int iNaviNum = Engine::Get_NaviMeshSize();
	WriteFile(hFile, &iNaviNum, sizeof(int), &dwByte, NULL);

	for(iter; iter != vecNavi.end(); ++iter)
	{
		D3DXVECTOR3 vPoint[3];
		DWORD		dwType;
		vPoint[0] = *((*iter)->GetPoint(Engine::CNaviCell::POINT_A));
		vPoint[1] = *((*iter)->GetPoint(Engine::CNaviCell::POINT_B));
		vPoint[2] = *((*iter)->GetPoint(Engine::CNaviCell::POINT_C));
		dwType = ((*iter)->GetType());

		WriteFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);
		WriteFile(hFile, &dwType, sizeof(DWORD), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CNaviMeshTool::OnNaviLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::Reset_NaviMesh();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"dat", NULL,//화일명 없음 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		NULL,NULL);

	//옵션 선택 부분.
	if(Dlg.DoModal() == IDOK)
	{
		CString strPathName = Dlg.GetPathName();//path를 돌려줌
		wstring wstrPath = strPathName;
	}

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	int iNaviNum;
	ReadFile(hFile, &iNaviNum, sizeof(int), &dwByte, NULL);
	Engine::Reserve_CellContainerSize(Engine::Get_GraphicDev(), iNaviNum);

	for(int i = 0; i < iNaviNum; ++i)
	{
		D3DXVECTOR3 vPoint[3];
		ReadFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);
		DWORD		dwType;
		ReadFile(hFile, &dwType, sizeof(DWORD), &dwByte, NULL);

		Engine::Add_Cell(&vPoint[0], &vPoint[1], &vPoint[2], dwType);
	}
	Engine::Link_Cell();

	m_iNaviCnt = iNaviNum;
	UpdateData(FALSE);

	CloseHandle(hFile);
}

void CNaviMeshTool::OnPointAposX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosX = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointA.x;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointA.x += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointA.x += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointA.x += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointA.x += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointA.x += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointA.x;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointA.x -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointA.x -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointA.x -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointA.x -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointA.x -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_A)->x = m_vPointA.x;

		stPosX.Format(_T("%.5f"),m_vPointA.x);

		SetDlgItemText(IDC_EDIT5,stPosX);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointAposY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosY = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointA.y;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointA.y += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointA.y += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointA.y += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointA.y += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointA.y += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointA.y;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointA.y -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointA.y -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointA.y -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointA.y -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointA.y -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_A)->y = m_vPointA.y;

		stPosY.Format(_T("%.5f"),m_vPointA.y);

		SetDlgItemText(IDC_EDIT6,stPosY);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointAposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosZ = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointA.z;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointA.z += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointA.z += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointA.z += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointA.z += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointA.z += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointA.z;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointA.z -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointA.z -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointA.z -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointA.z -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointA.z -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_A)->z = m_vPointA.z;

		stPosZ.Format(_T("%.5f"),m_vPointA.z);

		SetDlgItemText(IDC_EDIT7,stPosZ);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointBposX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosX = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointB.x;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointB.x += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointB.x += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointB.x += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointB.x += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointB.x += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointB.x;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointB.x -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointB.x -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointB.x -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointB.x -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointB.x -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_B)->x = m_vPointB.x;

		stPosX.Format(_T("%.5f"),m_vPointB.x);

		SetDlgItemText(IDC_EDIT8,stPosX);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointBposY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosY = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointB.y;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointB.y += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointB.y += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointB.y += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointB.y += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointB.y += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointB.y;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointB.y -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointB.y -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointB.y -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointB.y -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointB.y -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_B)->y = m_vPointB.y;

		stPosY.Format(_T("%.5f"),m_vPointB.y);

		SetDlgItemText(IDC_EDIT9,stPosY);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointBposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosZ = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointB.z;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointB.z += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointB.z += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointB.z += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointB.z += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointB.z += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointB.z;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointB.z -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointB.z -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointB.z -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointB.z -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointB.z -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_B)->z = m_vPointB.z;

		stPosZ.Format(_T("%.5f"),m_vPointB.z);

		SetDlgItemText(IDC_EDIT10,stPosZ);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointCposX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosX = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointC.x;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointC.x += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointC.x += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointC.x += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointC.x += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointC.x += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointC.x;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointC.x -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointC.x -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointC.x -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointC.x -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointC.x -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_C)->x = m_vPointC.x;

		stPosX.Format(_T("%.5f"),m_vPointC.x);

		SetDlgItemText(IDC_EDIT11,stPosX);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointCposY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosY = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointC.y;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointC.y += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointC.y += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointC.y += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointC.y += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointC.y += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointC.y;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointC.y -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointC.y -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointC.y -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointC.y -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointC.y -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_C)->y = m_vPointC.y;

		stPosY.Format(_T("%.5f"),m_vPointC.y);

		SetDlgItemText(IDC_EDIT12,stPosY);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnPointCposZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurCell)
	{
		CString stPosZ = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++m_vPointC.z;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointC.z += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointC.z += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointC.z += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointC.z += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointC.z += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--m_vPointC.z;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				m_vPointC.z -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				m_vPointC.z -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				m_vPointC.z -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				m_vPointC.z -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				m_vPointC.z -= 0.00001f;
		}

		m_pCurCell->GetPoint(Engine::CNaviCell::POINT_C)->z = m_vPointC.z;

		stPosZ.Format(_T("%.5f"),m_vPointC.z);

		SetDlgItemText(IDC_EDIT13,stPosZ);
	}
	*pResult = 0;
}

void CNaviMeshTool::OnWireFrameCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMapTool* pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;

	if(m_WireFrame.GetCheck() == TRUE)
	{
		pMapTool->m_WireFrame.SetCheck(TRUE);
	}
	else
	{
		pMapTool->m_WireFrame.SetCheck(FALSE);
	}
}
void CNaviMeshTool::OnObjectWire()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	CObjectTool* pObjTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab2;

	if(m_ObjectWire.GetCheck() == TRUE)
	{
		pObjTool->m_WireFrame.SetCheck(TRUE);
	}
	else
	{
		pObjTool->m_WireFrame.SetCheck(FALSE);
	}

}

void CNaviMeshTool::OnSwapPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_pCurCell)
	{
		D3DXVECTOR3 vTemp;
		D3DXVec3Normalize(&vTemp,&vTemp);
		if(m_SwapOption[0].GetCheck() == TRUE)
		{
			vTemp = m_vPointA;
			m_vPointA = m_vPointB;
			m_vPointB = vTemp;
			*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_A) = m_vPointA;
			*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_B) = m_vPointB;

		}
		else if(m_SwapOption[1].GetCheck() == TRUE)
		{
			vTemp = m_vPointB;
			m_vPointB = m_vPointC;
			m_vPointC = vTemp;
			*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_B) = m_vPointB;
			*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_C) = m_vPointC;
		}
		else if(m_SwapOption[2].GetCheck() == TRUE)
		{
			vTemp = m_vPointA;
			m_vPointA = m_vPointC;
			m_vPointC = vTemp;
			*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_A) = m_vPointA;
			*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_C) = m_vPointC;
		}
	}
	UpdateData(FALSE);
}

void CNaviMeshTool::OnCellEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_pCurCell)
	{
		*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_A) = m_vPointA;
		*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_B) = m_vPointB;
		*m_pCurCell->GetPoint(Engine::CNaviCell::POINT_C) = m_vPointC;
	}
	UpdateData(FALSE);
}
