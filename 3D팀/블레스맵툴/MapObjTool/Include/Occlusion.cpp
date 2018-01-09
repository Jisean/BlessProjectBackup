// Occlusion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "Occlusion.h"
#include "Export_Function.h"
#include "OcclusionBox.h"
#include "Transform.h"
#include "CubeColor.h"

// COcclusion 대화 상자입니다.

IMPLEMENT_DYNAMIC(COcclusion, CDialog)

COcclusion::COcclusion(CWnd* pParent /*=NULL*/)
: CDialog(COcclusion::IDD, pParent)
, m_fSizeX(0)
, m_fSizeY(0)
, m_fSizeZ(0)
, m_fRotX(0)
, m_fRotY(0)
, m_fRotZ(0)
, m_fPosX(0)
, m_fPosY(0)
, m_fPosZ(0)
, m_iBoxCnt(0)
, m_bStart(true)
, m_fAngleWithCam(0)
{

}

COcclusion::~COcclusion()
{
}

void COcclusion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, OccMode[0]);
	DDX_Control(pDX, IDC_RADIO2, OccMode[1]);
	DDX_Control(pDX, IDC_RADIO3, OccMode[2]);
	DDX_Control(pDX, IDC_RADIO4, OccMode[3]);
	DDX_Text(pDX, IDC_EDIT1, m_fSizeX);
	DDX_Text(pDX, IDC_EDIT2, m_fSizeY);
	DDX_Text(pDX, IDC_EDIT3, m_fSizeZ);
	DDX_Text(pDX, IDC_EDIT4, m_fRotX);
	DDX_Text(pDX, IDC_EDIT5, m_fRotY);
	DDX_Text(pDX, IDC_EDIT6, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT7, m_fPosX);
	DDX_Text(pDX, IDC_EDIT8, m_fPosY);
	DDX_Text(pDX, IDC_EDIT9, m_fPosZ);
	DDX_Control(pDX, IDC_LIST1, OccBoxList);
	DDX_Text(pDX, IDC_EDIT10, m_iBoxCnt);
	DDX_Control(pDX, IDC_RADIO5, m_Decimal[0]);
	DDX_Control(pDX, IDC_RADIO19, m_Decimal[1]);
	DDX_Control(pDX, IDC_RADIO20, m_Decimal[2]);
	DDX_Control(pDX, IDC_RADIO21, m_Decimal[3]);
	if(m_bStart)
	{
		m_Decimal[0].SetCheck(TRUE);
		OccMode[0].SetCheck(TRUE);
		m_bStart = false;
	}
	DDX_Text(pDX, IDC_EDIT11, m_fAngleWithCam);
}


BEGIN_MESSAGE_MAP(COcclusion, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &COcclusion::CreateOccBox)
	ON_BN_CLICKED(IDC_BUTTON8, &COcclusion::CopyOccBox)
	ON_BN_CLICKED(IDC_BUTTON11, &COcclusion::DoChange)
	ON_BN_CLICKED(IDC_BUTTON12, &COcclusion::RedoChange)
	ON_BN_CLICKED(IDC_BUTTON9, &COcclusion::DeleteOccBox)
	ON_BN_CLICKED(IDC_BUTTON5, &COcclusion::SaveOccBox)
	ON_BN_CLICKED(IDC_BUTTON6, &COcclusion::LoadOccBox)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &COcclusion::RotXControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &COcclusion::RotYControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &COcclusion::RotZControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &COcclusion::PosXControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &COcclusion::PosYControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &COcclusion::PosZControl)
	ON_LBN_SELCHANGE(IDC_LIST1, &COcclusion::OnLbnSelchange)
END_MESSAGE_MAP()


// COcclusion 메시지 처리기입니다.

void COcclusion::CreateOccBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(TRUE == OccMode[1].GetCheck())
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranslation(&matWorld, m_fPosX, m_fPosY, m_fPosZ);

		Engine::Add_OccBox(&matWorld);

		CString str;
		str.Format( _T("%d"), OccBoxList.GetCount());
		OccBoxList.AddString(str);
		m_iBoxCnt = OccBoxList.GetCount();
	}
	UpdateData(FALSE);
}

void COcclusion::CopyOccBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(TRUE == OccMode[1].GetCheck() && 0 != OccBoxList.GetCount() && 0 <= OccBoxList.GetCurSel())
	{
		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		Engine::Add_OccBox(&pInfo->m_matWorld);

		CString str;
		str.Format( _T("%d"), OccBoxList.GetCount());
		OccBoxList.AddString(str);
		m_iBoxCnt = OccBoxList.GetCount();

		OccBoxList.SetCurSel(OccBoxList.GetCount() - 1);
		Engine::CTransform* pInfoCopy = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfoCopy->m_vScale.x					= 	pInfo->m_vScale.x;			
		pInfoCopy->m_vScale.y					= 	pInfo->m_vScale.y;			
		pInfoCopy->m_vScale.z					= 	pInfo->m_vScale.z;			
		pInfoCopy->m_fAngle[Engine::ANGLE_X]	= 	pInfo->m_fAngle[Engine::ANGLE_X];
		pInfoCopy->m_fAngle[Engine::ANGLE_Y]	= 	pInfo->m_fAngle[Engine::ANGLE_Y];
		pInfoCopy->m_fAngle[Engine::ANGLE_Z]	= 	pInfo->m_fAngle[Engine::ANGLE_Z];
		pInfoCopy->m_vPos.x						= 	pInfo->m_vPos.x;
		pInfoCopy->m_vPos.y						= 	pInfo->m_vPos.y;
		pInfoCopy->m_vPos.z						= 	pInfo->m_vPos.z;

		pInfoCopy->Update();

		SetInformation(pInfoCopy);
	}
}

void COcclusion::DoChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if(TRUE == OccMode[2].GetCheck())
	{
		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_vScale.x					= 	m_fSizeX;
		pInfo->m_vScale.y					= 	m_fSizeY;
		pInfo->m_vScale.z					= 	m_fSizeZ;
		pInfo->m_fAngle[Engine::ANGLE_X]	= 	D3DXToRadian(m_fRotX);
		pInfo->m_fAngle[Engine::ANGLE_Y]	= 	D3DXToRadian(m_fRotY);
		pInfo->m_fAngle[Engine::ANGLE_Z]	= 	D3DXToRadian(m_fRotZ);
		pInfo->m_vPos.x						= 	m_fPosX;
		pInfo->m_vPos.y						= 	m_fPosY;
		pInfo->m_vPos.z						= 	m_fPosZ;

		pInfo->Update();
	}
}

void COcclusion::RedoChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

	SetInformation(pInfo);
}

void COcclusion::DeleteOccBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(TRUE == OccMode[3].GetCheck())
	{
		int iChangeCount = OccBoxList.GetCount() - OccBoxList.GetCurSel() - 1;
		int iChange = OccBoxList.GetCurSel();
		Engine::COcclusionMgr::GetInstance()->Delete_OccBox(OccBoxList.GetCurSel());
		OccBoxList.DeleteString(OccBoxList.GetCurSel());
		CString Tempstr;
		if(iChangeCount > 0)
		{
			for(int i = 0; i < iChangeCount; ++i)
			{
				OccBoxList.GetText(i + iChange, Tempstr);
				int iTemp = _ttoi(Tempstr);
				CString strTemp;
				strTemp.Format( _T("%d"), --iTemp);
				OccBoxList.InsertString(i + iChange ,strTemp);
				OccBoxList.DeleteString(_ttoi(Tempstr));
			}
		}
		m_iBoxCnt = OccBoxList.GetCount();
		if(iChange < OccBoxList.GetCount())
			OccBoxList.SetCurSel(iChange);
		else
			OccBoxList.SetCurSel(OccBoxList.GetCount() - 1);

		UpdateData(FALSE);
	}
}

void COcclusion::SetInformation(Engine::CTransform* pInfo)
{
	m_fSizeX	= pInfo->m_vScale.x;
	m_fSizeY	= pInfo->m_vScale.y;
	m_fSizeZ	= pInfo->m_vScale.z;
	m_fRotX		= D3DXToDegree(pInfo->m_fAngle[Engine::ANGLE_X]);
	m_fRotY		= D3DXToDegree(pInfo->m_fAngle[Engine::ANGLE_Y]);
	m_fRotZ		= D3DXToDegree(pInfo->m_fAngle[Engine::ANGLE_Z]);
	m_fPosX		= pInfo->m_vPos.x;
	m_fPosY		= pInfo->m_vPos.y;
	m_fPosZ		= pInfo->m_vPos.z;

	UpdateData(FALSE);
}

void COcclusion::ResetInformation(void)
{
	m_fSizeX	= 0.f;
	m_fSizeY	= 0.f;
	m_fSizeZ	= 0.f;
	m_fRotX		= 0.f;
	m_fRotY		= 0.f;
	m_fRotZ		= 0.f;
	m_fPosX		= 0.f;
	m_fPosY		= 0.f;
	m_fPosZ		= 0.f;

	UpdateData(FALSE);
}

void COcclusion::AngleWithCamUpdate(void)
{
	D3DXMATRIX matViewInv;	
	Engine::Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matViewInv);
	D3DXMatrixInverse(&matViewInv,NULL,&matViewInv);
	D3DXVECTOR3 vCamEye(matViewInv._41, matViewInv._42, matViewInv._43);

	D3DXVECTOR3 vMin, vMax, vCenter;
	Engine::COcclusionMgr::GetInstance()->Get_MinMax(OccBoxList.GetCurSel(), &vMin, &vMax, &vCenter);

	Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());
	D3DXVECTOR3 vOccDir, vCamDir;
	vCamDir = vCenter - vCamEye;
	vCamDir.y = 0.f;
	vOccDir = -pInfo->m_vDir;
	D3DXVec3Normalize(&vCamDir, &vCamDir);
	D3DXVec3Normalize(&vOccDir, &vOccDir);

	m_fAngleWithCam = D3DXToDegree(acos(D3DXVec3Dot(&vCamDir, &vOccDir)));
	if(matViewInv._41 < vCenter.x)
		m_fAngleWithCam = -m_fAngleWithCam;

	UpdateData(FALSE);
}

void COcclusion::RotXControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(OccBoxList.GetCount() && TRUE == OccMode[2].GetCheck())
	{
		UpdateData(TRUE);
		if(pNMUpDown->iDelta < 0)
		{
			++m_fRotX;

			if(m_fRotX > 361)
				m_fRotX = (float)((int)m_fRotX % 360);
			else if(m_fRotX > 360.f)
				m_fRotX = 1;
		}
		else
		{
			--m_fRotX;
			if(m_fRotX < 0)
				m_fRotX = 359;
		}
		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(m_fRotX);

		

		SetDlgItemInt(IDC_EDIT4, (float)m_fRotX);
		DoChange();
		UpdateData(FALSE);
	}

	*pResult = 0;
}

void COcclusion::RotYControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(OccBoxList.GetCount() && TRUE == OccMode[2].GetCheck())
	{
		UpdateData(TRUE);
		if(pNMUpDown->iDelta < 0)
		{
			++m_fRotY;

			if(m_fRotY > 361)
				m_fRotY = (float)((int)m_fRotY % 360);
			else if(m_fRotY > 360.f)
				m_fRotY = 1;
		}
		else
		{
			--m_fRotY;
			if(m_fRotY < 0)
				m_fRotY = 359;
		}
		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(m_fRotY);

		
		SetDlgItemInt(IDC_EDIT5, (float)m_fRotY);
		DoChange();
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void COcclusion::RotZControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(OccBoxList.GetCount() && TRUE == OccMode[2].GetCheck())
	{
		UpdateData(TRUE);
		if(pNMUpDown->iDelta < 0)
		{
			++m_fRotZ;

			if(m_fRotZ > 361)
				m_fRotZ = (float)((int)m_fRotZ % 360);
			else if(m_fRotZ > 360.f)
				m_fRotZ = 1;
		}
		else
		{
			--m_fRotZ;
			if(m_fRotZ < 0)
				m_fRotZ = 359;
		}
		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian(m_fRotZ);

		
		SetDlgItemInt(IDC_EDIT6, (float)m_fRotZ);
		DoChange();
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void COcclusion::PosXControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(OccBoxList.GetCount() && TRUE == OccMode[2].GetCheck())
	{
		UpdateData(TRUE);
		CString stPosX = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_Decimal[0].GetCheck() == TRUE)
				++m_fPosX;
			else if(m_Decimal[1].GetCheck() == TRUE)
				m_fPosX += 0.1f;
			else if(m_Decimal[2].GetCheck() == TRUE)
				m_fPosX += 0.01f;
			else if(m_Decimal[3].GetCheck() == TRUE)
				m_fPosX += 0.001f;
		}
		else
		{
			if(m_Decimal[0].GetCheck() == TRUE)
				--m_fPosX;
			else if(m_Decimal[1].GetCheck() == TRUE)
				m_fPosX -= 0.1f;
			else if(m_Decimal[2].GetCheck() == TRUE)
				m_fPosX -= 0.01f;
			else if(m_Decimal[3].GetCheck() == TRUE)
				m_fPosX -= 0.001f;
		}

		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_vPos.x = m_fPosX;

		stPosX.Format(_T("%.5f"), m_fPosX);

		
		SetDlgItemText(IDC_EDIT7, stPosX);
		DoChange();
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void COcclusion::PosYControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(OccBoxList.GetCount() && TRUE == OccMode[2].GetCheck())
	{
		UpdateData(TRUE);
		CString stPosY = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_Decimal[0].GetCheck() == TRUE)
				++m_fPosY;
			else if(m_Decimal[1].GetCheck() == TRUE)
				m_fPosY += 0.1f;
			else if(m_Decimal[2].GetCheck() == TRUE)
				m_fPosY += 0.01f;
			else if(m_Decimal[3].GetCheck() == TRUE)
				m_fPosY += 0.001f;
		}
		else
		{
			if(m_Decimal[0].GetCheck() == TRUE)
				--m_fPosY;
			else if(m_Decimal[1].GetCheck() == TRUE)
				m_fPosY -= 0.1f;
			else if(m_Decimal[2].GetCheck() == TRUE)
				m_fPosY -= 0.01f;
			else if(m_Decimal[3].GetCheck() == TRUE)
				m_fPosY -= 0.001f;
		}

		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_vPos.y = m_fPosY;

		stPosY.Format(_T("%.5f"), m_fPosY);

		
		SetDlgItemText(IDC_EDIT8, stPosY);
		DoChange();
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void COcclusion::PosZControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(OccBoxList.GetCount() && TRUE == OccMode[2].GetCheck())
	{
		UpdateData(TRUE);
		CString stPosZ = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_Decimal[0].GetCheck() == TRUE)
				++m_fPosZ;
			else if(m_Decimal[1].GetCheck() == TRUE)
				m_fPosZ += 0.1f;
			else if(m_Decimal[2].GetCheck() == TRUE)
				m_fPosZ += 0.01f;
			else if(m_Decimal[3].GetCheck() == TRUE)
				m_fPosZ += 0.001f;
		}
		else
		{
			if(m_Decimal[0].GetCheck() == TRUE)
				--m_fPosZ;
			else if(m_Decimal[1].GetCheck() == TRUE)
				m_fPosZ -= 0.1f;
			else if(m_Decimal[2].GetCheck() == TRUE)
				m_fPosZ -= 0.01f;
			else if(m_Decimal[3].GetCheck() == TRUE)
				m_fPosZ -= 0.001f;
		}

		Engine::CTransform* pInfo = Engine::Get_OccBox(OccBoxList.GetCurSel());

		pInfo->m_vPos.x = m_fPosZ;
		

		stPosZ.Format(_T("%.5f"), m_fPosZ);

		
		SetDlgItemText(IDC_EDIT9, stPosZ);
		DoChange();
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void COcclusion::SaveOccBox()
{
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

	int iCount = Engine::COcclusionMgr::GetInstance()->GetVecOccBoxSize();
	WriteFile(hFile, &iCount, sizeof(int), &dwByte, NULL);

	for (int i = 0; i < iCount; ++i)
	{
		Engine::CTransform* pOccBox = Engine::Get_OccBox(i);

		WriteFile(hFile, &pOccBox->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);
		WriteFile(hFile, &pOccBox->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
		WriteFile(hFile, &pOccBox->m_fAngle, sizeof(float) * 3, &dwByte, NULL);
		WriteFile(hFile, &pOccBox->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		WriteFile(hFile, &pOccBox->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void COcclusion::LoadOccBox()
{
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
	int iCount = 0;
	ReadFile(hFile, &iCount, sizeof(int), &dwByte, NULL);

	int		iIndex = 0;
	for(int i = 0; i < iCount; ++i)
	{
		D3DXMATRIX matWorld;
		ReadFile(hFile, &matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);
		Engine::Add_OccBox(&matWorld);
		Engine::CTransform* pOccBox = Engine::Get_OccBox(i);

		ReadFile(hFile, &pOccBox->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, &pOccBox->m_fAngle, sizeof(float) * 3, &dwByte, NULL);
		ReadFile(hFile, &pOccBox->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, &pOccBox->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
		pOccBox->Update();

		CString str;
		str.Format( _T("%d"), OccBoxList.GetCount());
		OccBoxList.AddString(str);
		m_iBoxCnt = OccBoxList.GetCount();
	}

	CloseHandle(hFile);

	UpdateData(FALSE);
}

void COcclusion::OnLbnSelchange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::CTransform* pOccBox = Engine::Get_OccBox(OccBoxList.GetCurSel());

	for(unsigned int i = 0; i < Engine::COcclusionMgr::GetInstance()->GetVecOccBoxSize(); ++i)
		Engine::Get_OccBoxVtx(i)->Change_BoxColor(D3DCOLOR_ARGB(255, 0, 255, 0));

	Engine::Get_OccBoxVtx(OccBoxList.GetCurSel())->Change_BoxColor(D3DCOLOR_ARGB(255, 255, 0, 0));

	SetInformation(pOccBox);
	AngleWithCamUpdate();
}
