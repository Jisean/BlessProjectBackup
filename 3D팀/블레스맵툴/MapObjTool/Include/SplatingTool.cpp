// SplatingTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "SplatingTool.h"
#include "Back.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Terrain.h"
#include "Texture.h"


// CSplatingTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSplatingTool, CDialog)

CSplatingTool::CSplatingTool(CWnd* pParent /*=NULL*/)
	: CDialog(CSplatingTool::IDD, pParent)
	, m_fBrushSize(30.f)
	, m_fAlphaRadius(15.f)
	, m_bStart(true)
	, m_iCurrentFilter(0)
	, m_AlphaRadius(_T(""))
	, m_BrushSize(_T(""))
{

}

CSplatingTool::~CSplatingTool()
{
}

void CSplatingTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	CMapTool* pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	DDX_Text(pDX, IDC_EDIT1, m_BrushSize);
	DDX_Text(pDX, IDC_EDIT2, m_AlphaRadius);
	DDX_Control(pDX, IDC_RADIO1, m_SplatingMode[0]);
	DDX_Control(pDX, IDC_RADIO2, m_SplatingMode[1]);
	DDX_Control(pDX, IDC_RADIO3, m_SplatingMode[2]);

	DDX_Control(pDX, IDC_LIST1, m_TileList);

	if(m_bStart)
	{
		m_iCurrentFilter = 0;
		m_SplatingMode->SetCheck(TRUE);
		m_bStart = false;
	}
	DDX_Control(pDX, IDC_IMAGE, m_Picture);
}


BEGIN_MESSAGE_MAP(CSplatingTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSplatingTool::OnTileList)
	ON_BN_CLICKED(IDC_BUTTON2, &CSplatingTool::OnEraseSplating)
	ON_EN_CHANGE(IDC_EDIT1, &CSplatingTool::OnBrushSize)
	ON_EN_CHANGE(IDC_EDIT2, &CSplatingTool::OnAlphaRadius)
	ON_BN_CLICKED(IDC_BUTTON6, &CSplatingTool::OnDrawingEnd)
	ON_BN_CLICKED(IDC_BUTTON5, &CSplatingTool::OnDeleteSplating)
	ON_BN_CLICKED(IDC_BUTTON3, &CSplatingTool::OnSaveSplating)
END_MESSAGE_MAP()


// CSplatingTool �޽��� ó�����Դϴ�.

void CSplatingTool::OnTileList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;


	CString strSelectName;

	int iSelectNum = m_TileList.GetCurSel();

	if(iSelectNum < 0)
		return;

	m_TileList.GetText(iSelectNum, strSelectName);

	map<CString, CImage*>::iterator iter = pMapTool->m_MapTileImage.find(strSelectName);

	if(iter == pMapTool->m_MapTileImage.end())
		return;

	m_Picture.SetBitmap(*iter->second);

	int i = 0;

	for(i = 0; i < strSelectName.GetLength(); ++i)
	{
		if(isdigit(strSelectName[i]) != 0)		// ��ȯ���� 0�� ���� ���� ������ ���
			break;								// ��ȯ���� 0�� �ƴ� �洩�� ���� ������ ���
	}

	((CBack*)Engine::Get_Scene())->m_pTerrain->m_iFilterCnt = m_TileList.GetCurSel();
	((CBack*)Engine::Get_Scene())->m_pTerrain->m_iFilterViewCnt = m_TileList.GetCurSel();

	m_iCurrentFilter = m_TileList.GetCurSel();

	if(m_TileList.GetCurSel() == 0)
	{
		m_SplatingMode[0].SetCheck(TRUE);
		m_SplatingMode[1].SetCheck(FALSE);
		m_SplatingMode[2].SetCheck(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
	}

	strSelectName.Delete(0, i);
	
	UpdateData(FALSE);

}

void CSplatingTool::OnEraseSplating()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	((CBack*)Engine::Get_Scene())->m_pTerrain->DeleteAlphaMaps();
}

void CSplatingTool::OnBrushSize()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);
	m_fBrushSize = (float)_wtof(m_BrushSize);
	UpdateData(FALSE);
}

void CSplatingTool::OnAlphaRadius()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);
	m_fAlphaRadius = (float)_wtof(m_AlphaRadius);
	UpdateData(FALSE);
}

void CSplatingTool::OnDrawingEnd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	

	UpdateData(TRUE);
	CTerrain* pTerrain = ((CBack*)Engine::Get_Scene())->m_pTerrain;

	LPDIRECT3DTEXTURE9* pAlphaTexture = pTerrain->m_pFilterTexture->GetTexture(m_iCurrentFilter - 1);


	if(m_iCurrentFilter - 1 == 0)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight0.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else if(m_iCurrentFilter - 1 == 1)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight1.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else if(m_iCurrentFilter - 1 == 2)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight2.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else if(m_iCurrentFilter - 1 == 3)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight3.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else if(m_iCurrentFilter - 1 == 4)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight4.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else if(m_iCurrentFilter - 1 == 5)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight5.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else if(m_iCurrentFilter - 1 == 6)
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight6.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
	else
		return;
	map<const TCHAR*, Engine::CComponent*>::iterator iter = pTerrain->m_mapComponent.find(L"FilterTexture");

	Engine::Delete_ByKey(RESOURCE_TOOL, L"Texture_Filter");

	pTerrain->m_mapComponent.erase(iter);
	Engine::Add_Texture(Engine::Get_GraphicDev()
		, RESOURCE_TOOL
		, Engine::TEXTURE_NORMAL
		, L"Texture_Filter"
		, L"../bin/Resources/Texture/Terrain/ColorHeight%d.png", 7);
	Engine::CComponent*		pComponent = NULL;

	pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Filter");
	pTerrain->m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	pTerrain->m_mapComponent.insert(map<const TCHAR*, Engine::CComponent*>::value_type(L"FilterTexture", pComponent));


	UpdateData(FALSE);
}

void CSplatingTool::OnDeleteSplating()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	if(AfxMessageBox(L"Are you sure Reset all Splating Maps?",MB_OKCANCEL) == IDOK)
	{
		CTerrain* pTerrain = ((CBack*)Engine::Get_Scene())->m_pTerrain;

		pTerrain->ResetAlphaMaps();
	}

	UpdateData(FALSE);


}

void CSplatingTool::OnSaveSplating()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CObjectTool*	pObjTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab2;

	CTerrain* pTerrain = ((CBack*)Engine::Get_Scene())->m_pTerrain;

	for(int i=0; i<7; ++i)
	{
		LPDIRECT3DTEXTURE9* pAlphaTexture = pTerrain->m_pFilterTexture->GetTexture(i);

		if(pObjTool->m_StaticAreaRadio[0].GetCheck() == TRUE)
		{
			if(i == 0)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter0.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 1)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter1.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 2)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter2.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 3)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter3.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 4)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter4.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 5)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter5.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 6)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ATFilter6.png", D3DXIFF_PNG, *pAlphaTexture, NULL);

		}
		else if(pObjTool->m_StaticAreaRadio[1].GetCheck() == TRUE)
		{
			if(i == 0)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter0.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 1)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter1.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 2)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter2.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 3)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter3.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 4)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter4.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 5)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter5.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 6)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/CityFilter6.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
		}
		else if(pObjTool->m_StaticAreaRadio[2].GetCheck() == TRUE)
		{
			if(i == 0)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter0.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 1)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter1.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 2)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter2.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 3)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter3.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 4)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter4.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 5)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter5.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 6)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ARFilter6.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
		}
		else if(pObjTool->m_StaticAreaRadio[3].GetCheck() == TRUE)
		{
			if(i == 0)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter0.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 1)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter1.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 2)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter2.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 3)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter3.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 4)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter4.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 5)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter5.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 6)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/DMFilter6.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
		}
		else if(pObjTool->m_StaticAreaRadio[4].GetCheck() == TRUE)
		{
			if(i == 0)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter0.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 1)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter1.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 2)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter2.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 3)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter3.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 4)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter4.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 5)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter5.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
			else if(i == 6)
				D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/GuiscardFilter6.png", D3DXIFF_PNG, *pAlphaTexture, NULL);
		}

	}

	AfxMessageBox(L"Save Succesful! Check Terrain Folder.");


}

void CSplatingTool::ActivateButton()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	UpdateData(FALSE);
}
