// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "MapTool.h"
#include "Back.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include "SplatingTool.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Texture.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include "NaviMeshTool.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
	, m_iTextureMount(0)
	, m_iCntX(VTXCNTX)
	, m_iCntZ(VTXCNTZ)
	, m_iItv(VTXITV)
	, m_dPickSize(0.0)
	, m_fSetHeight(0.f)
	, m_SetHeight(_T(""))
	, m_UVCnt(_T(""))
	, m_fUVCnt(30.f)
	, m_bTextureLoad(false)
	, m_fSmoothPower(0.06f)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CntX);
	DDX_Text(pDX, IDC_EDIT2, m_CntZ);
	DDX_Text(pDX, IDC_EDIT3, m_Itv);
	DDX_Control(pDX, IDC_LIST1, m_MapTextureList);;
	DDX_Text(pDX, IDC_EDIT4, m_PickSize);
	DDX_Control(pDX, IDC_RADIO1, m_MapEditMode[0]);
	DDX_Control(pDX, IDC_RADIO2, m_MapEditMode[1]);

	DDX_Control(pDX, IDC_RADIO3, m_ModifyMode[0]);
	DDX_Control(pDX, IDC_RADIO4, m_ModifyMode[1]);
	DDX_Control(pDX, IDC_RADIO5, m_ModifyMode[2]);

	DDX_Control(pDX, IDC_RADIO6, m_Gradient[0]);
	DDX_Control(pDX, IDC_RADIO7, m_Gradient[1]);
	DDX_Control(pDX, IDC_RADIO8, m_Gradient[2]);

	DDX_Control(pDX, IDC_RADIO10, m_MainHeightMode[0]);
	DDX_Control(pDX, IDC_RADIO11, m_MainHeightMode[1]);
	DDX_Control(pDX, IDC_RADIO12, m_SoftHeightMode[0]);
	DDX_Control(pDX, IDC_RADIO13, m_SoftHeightMode[1]);
	DDX_Control(pDX, IDC_RADIO14, m_SmoothUpDown[0]);
	DDX_Control(pDX, IDC_RADIO15, m_SmoothUpDown[1]);
	if(m_bStart)
	{
		m_Gradient->SetCheck(TRUE);
		m_MapEditMode->SetCheck(TRUE);
		m_ModifyMode->SetCheck(TRUE);
		m_MainHeightMode->SetCheck(TRUE);
		m_SoftHeightMode->SetCheck(TRUE);
		m_SmoothUpDown->SetCheck(TRUE);
		m_bStart = false;
	}


	DDX_Text(pDX, IDC_EDIT6, m_SetHeight);
	DDX_Text(pDX, IDC_EDIT5, m_UVCnt);
	DDX_Control(pDX, IDC_CHECK1, m_WireFrame);
	DDX_Text(pDX, IDC_EDIT7, m_fSmoothPower);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnCreateTerrain)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::TextureSetting)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::SaveTerrain)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::LoadTerrain)
	ON_EN_CHANGE(IDC_EDIT1, &CMapTool::OnVtxX)
	ON_EN_CHANGE(IDC_EDIT2, &CMapTool::OnVtxZ)
	ON_EN_CHANGE(IDC_EDIT3, &CMapTool::OnInterval)
	ON_EN_CHANGE(IDC_EDIT4, &CMapTool::OnPickSize)
	ON_EN_CHANGE(IDC_EDIT6, &CMapTool::OnHeight)
	ON_EN_CHANGE(IDC_EDIT5, &CMapTool::OnUVCount)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnWireFrameCheck)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.

void CMapTool::OnCreateTerrain()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	((CBack*)Engine::Get_Scene())->MakeTerrainBuffer(m_iCntX, m_iCntZ, m_iItv);
	((CBack*)Engine::Get_Scene())->MakeTerrainLayer(m_iCntX, m_iCntZ, m_fUVCnt);
	((CBack*)Engine::Get_Scene())->m_pTerrain->ResetAlphaMaps();


	if(m_bTextureLoad == true)
	{
		//map<const TCHAR*, Engine::CComponent*>::iterator iter = pTerrain->m_mapComponent.find(L"FilterTexture");

		Engine::Delete_ByKey(RESOURCE_TOOL, L"Texture_Filter");

		//pTerrain->m_mapComponent.erase(iter);
		Engine::Add_Texture(Engine::Get_GraphicDev()
			, RESOURCE_TOOL
			, Engine::TEXTURE_NORMAL
			, L"Texture_Filter"
			, L"../bin/Resources/Texture/Terrain/ColorHeight%d.png", 7);
		Engine::CComponent*		pComponent = NULL;

		pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Filter");
		((CBack*)Engine::Get_Scene())->m_pTerrain->m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
		((CBack*)Engine::Get_Scene())->m_pTerrain->m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"FilterTexture", pComponent));
	}


}

void CMapTool::TextureSetting()
{
	CSplatingTool* pSplatTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab4;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	

	BROWSEINFO BrInfo;	
	::ZeroMemory(&BrInfo,sizeof(BROWSEINFO));
	::ZeroMemory(m_szBuffer, 512); 

	BrInfo.hwndOwner = GetSafeHwnd(); 

	BrInfo.lpszTitle = _T("�ҷ� �� ������ �����ϼ���");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pItemIdList = NULL;

	pItemIdList = ::SHBrowseForFolder(&BrInfo);

	if(pItemIdList == NULL)
		return;
	
	pSplatTool->m_TileList.ResetContent();
	m_MapTextureList.ResetContent();

	::SHGetPathFromIDList(pItemIdList, m_szBuffer);



	m_FolderName = m_szBuffer;
	m_iTextureMount = 0;


	CFileFind file;

	BOOL bFind;


	m_TempPathName = m_FolderName + _TEXT("\\*.tga"); 

	bFind = file.FindFile(m_TempPathName);

	CString strFileName;
	CString strFileNamePng;
	while(bFind)
	{
		if(!bFind)
		{
			file.Close();
			return;
		}
		else
		{
			bFind = file.FindNextFileW();
			strFileName = file.GetFileTitle() + _T(".tga");
			strFileNamePng = file.GetFileTitle() +_T(".png");
			++m_iTextureMount;
			
			CImage* pTileImage = new CImage;

			CString stFilePath = file.GetFilePath();

			PathRemoveFileSpec((LPWSTR)(LPCTSTR)stFilePath);//���� �̸��� ������
			TCHAR* tcFilePath = new TCHAR[128];
			wsprintf(tcFilePath, stFilePath);
			_tcscat(tcFilePath,_T("\\"));
			_tcscat(tcFilePath,strFileNamePng);



			pTileImage->Load(tcFilePath);

			m_MapTileImage.insert(make_pair(strFileName,pTileImage));
			m_MapTextureList.AddString(strFileName);
			pSplatTool->m_TileList.AddString(strFileName);
		}
	};

	CString FullPath = m_FolderName + _T("\\Tile_%d.tga");
	TCHAR* pFullPath = (TCHAR*)(LPCTSTR)FullPath;
	((CBack*)(Engine::Get_Scene()))->m_pTerrain->m_pTexture = NULL;



	Engine::Delete_ByKey(RESOURCE_TOOL, L"Texture_Terrain");
	HRESULT hr;
	hr = Engine::Add_Texture(Engine::Get_GraphicDev()
		, RESOURCE_TOOL
		, Engine::TEXTURE_NORMAL
		, L"Texture_Terrain"
		, FullPath, m_iTextureMount);
	((CBack*)(Engine::Get_Scene()))->m_pTerrain->SetTexture();
	file.Close();

	if(m_bTextureLoad == false)
		pSplatTool->ActivateButton();


	m_bTextureLoad = true;

}


void CMapTool::SaveTerrain()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"*.dat", this);

	// 1. ���� ��� ���� 2. ������ Ȯ���ڸ� 3. ���ʿ� ��� �� ���ڿ� 
	// 4. �ߺ� ���� ó���� ���� �Ű� ���� 5. ���� ���Ŀ� ���� Ȯ���ڸ�

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	SetCurrentDirectory(szDirPath);
	// ���� ���α׷��� ���� �۾� ���丮�� ������ ���丮�� �������ִ� �Լ�

	PathRemoveFileSpec(szDirPath);
	// ������ ��θ� ������ ��θ� �����ϴ� �Լ�

	lstrcat(szDirPath, L"..\\..\\Resources\\Data");

	Dlg.m_ofn.lpstrInitialDir = szDirPath;		// ��� ���

	Engine::VTXTEX* pTerrainVtx = ((CBack*)Engine::Get_Scene())->m_pTerrain->GetTerrainVtx();

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	WriteFile(hFile, &m_iCntX, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &m_iCntZ, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &m_iItv, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &m_fUVCnt, sizeof(float), &dwByte, NULL);

	int		iIndex = 0;
	for(int z = 0; z < m_iCntZ; ++z)
	{
		for(int x = 0; x < m_iCntX; ++x)
		{
			iIndex = z * m_iCntX + x;

			WriteFile(hFile, &pTerrainVtx[iIndex].vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		}
	}

	CloseHandle(hFile);
}

void CMapTool::LoadTerrain()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L"dat", NULL,//ȭ�ϸ� ���� 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		NULL,NULL);

	//�ɼ� ���� �κ�.
	if(Dlg.DoModal() == IDOK)
	{
		CString strPathName = Dlg.GetPathName();//path�� ������
		wstring wstrPath = strPathName;
	}

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	ReadFile(hFile, &m_iCntX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &m_iCntZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &m_iItv, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &m_fUVCnt, sizeof(float), &dwByte, NULL);

	UpdateData(FALSE);

	((CBack*)Engine::Get_Scene())->MakeTerrainBuffer(m_iCntX, m_iCntZ, m_iItv);
	((CBack*)Engine::Get_Scene())->MakeTerrainLayer(m_iCntX, m_iCntZ, m_fUVCnt);

	Engine::VTXTEX* pTerrainVtx = ((CBack*)Engine::Get_Scene())->m_pTerrain->GetTerrainVtx();

	int		iIndex = 0;
	for(int z = 0; z < m_iCntZ; ++z)
	{
		for(int x = 0; x < m_iCntX; ++x)
		{
			iIndex = z * m_iCntX + x;

			ReadFile(hFile, &pTerrainVtx[iIndex].vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		}
	}

	((CBack*)Engine::Get_Scene())->m_pTerrain->m_pBuffer->SetVtxInfo(pTerrainVtx);
	((CBack*)Engine::Get_Scene())->m_pTerrain->SetNormal();


	if(m_bTextureLoad == true)
	{
		//map<const TCHAR*, Engine::CComponent*>::iterator iter = pTerrain->m_mapComponent.find(L"FilterTexture");

		Engine::Delete_ByKey(RESOURCE_TOOL, L"Texture_Filter");

		//pTerrain->m_mapComponent.erase(iter);
		Engine::Add_Texture(Engine::Get_GraphicDev()
			, RESOURCE_TOOL
			, Engine::TEXTURE_NORMAL
			, L"Texture_Filter"
			, L"../bin/Resources/Texture/Terrain/ColorHeight%d.png", 7);
		Engine::CComponent*		pComponent = NULL;

		pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Filter");
		((CBack*)Engine::Get_Scene())->m_pTerrain->m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
		((CBack*)Engine::Get_Scene())->m_pTerrain->m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"FilterTexture", pComponent));
	}

	CloseHandle(hFile);
}


void CMapTool::OnVtxX()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_iCntX = (int)_wtof(m_CntX);
	UpdateData(FALSE);
}

void CMapTool::OnVtxZ()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_iCntZ = (int)_wtof(m_CntZ);
	UpdateData(FALSE);
}

void CMapTool::OnInterval()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);
	m_iItv = (int)_wtof(m_Itv);
	UpdateData(FALSE);
}

void CMapTool::OnPickSize()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_dPickSize = (double)_wtof(m_PickSize);
	UpdateData(FALSE);
}

void CMapTool::OnHeight()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_fSetHeight = (float)_wtof(m_SetHeight);
	UpdateData(FALSE);
}

void CMapTool::OnUVCount()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_fUVCnt = (float)_wtof(m_UVCnt);
	UpdateData(FALSE);
}

void CMapTool::OnWireFrameCheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CNaviMeshTool* pNaviTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab3;

	if(m_WireFrame.GetCheck() == TRUE)
	{
		pNaviTool->m_WireFrame.SetCheck(TRUE);
	}
	else
	{
		pNaviTool->m_WireFrame.SetCheck(FALSE);
	}
}
