// ObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapObjTool.h"
#include "ObjectTool.h"
#include "FileInfo.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "FileInfo.h"
#include "GameObject.h"
#include "Transform.h"
#include "MainFrm.h"
#include "MapObjToolView.h"
#include "Scene.h"
#include "Back.h"
#include "..\Engine\Utility\Code\Layer.h"
#include "DungeonObject.h"
#include "CityObject.h"
#include "DungeonObject.h"
#include "DMObject.h"
#include "GuiscardObject.h"
#include "NaviMeshTool.h"
#include "MyForm.h"
#include "DynamicObject.h"



// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
: CDialog(CObjectTool::IDD, pParent)
,m_bStart(true)
,m_bFirstLoad(false)
,m_bFirstDynamicLoad(false)
, fScaleX(0)
, fScaleY(0)
, fScaleZ(0)
, fRotX(0)
, fRotY(0)
, fRotZ(0)
, fPosX(0)
, fPosY(0)
, fPosZ(0)
, m_stCurrentMeshKey(_T(""))
{

}

CObjectTool::~CObjectTool()
{
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO5, m_StaticAreaRadio[0]);
	DDX_Control(pDX, IDC_RADIO6, m_StaticAreaRadio[1]);
	DDX_Control(pDX, IDC_RADIO1, m_ObjModeRadio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_ObjModeRadio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_ObjModeRadio[2]);
	DDX_Control(pDX, IDC_RADIO4, m_ObjModeRadio[3]);
	DDX_Control(pDX, IDC_RADIO10, m_ObjTypeRadio[0]);
	DDX_Control(pDX, IDC_RADIO11, m_ObjTypeRadio[1]);
	DDX_Control(pDX, IDC_LIST1, m_StaticList);
	DDX_Control(pDX, IDC_LIST2, m_DynamicList);
	DDX_Control(pDX, IDC_RADIO12, m_PosDecimal[0]);
	DDX_Control(pDX, IDC_RADIO13, m_PosDecimal[1]);
	DDX_Control(pDX, IDC_RADIO14, m_PosDecimal[2]);
	DDX_Control(pDX, IDC_RADIO15, m_PosDecimal[3]);
	DDX_Control(pDX, IDC_RADIO16, m_PosDecimal[4]);
	DDX_Control(pDX, IDC_RADIO17, m_PosDecimal[5]);
	if(m_bStart)
	{
		m_StaticAreaRadio[0].SetCheck(TRUE);
		m_ObjModeRadio[0].SetCheck(TRUE);
		m_ObjTypeRadio[0].SetCheck(TRUE);
		m_PosDecimal[0].SetCheck(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		m_bStart = false;
	}
	DDX_Text(pDX, IDC_EDIT1, fScaleX);
	DDX_Text(pDX, IDC_EDIT2, fScaleY);
	DDX_Text(pDX, IDC_EDIT3, fScaleZ);
	DDX_Text(pDX, IDC_EDIT4, fRotX);
	DDX_Text(pDX, IDC_EDIT5, fRotY);
	DDX_Text(pDX, IDC_EDIT6, fRotZ);
	DDX_Text(pDX, IDC_EDIT7, fPosX);
	DDX_Text(pDX, IDC_EDIT8, fPosY);
	DDX_Text(pDX, IDC_EDIT9, fPosZ);

	DDX_Text(pDX, IDC_EDIT10, m_stCurrentMeshKey);
	DDX_Control(pDX, IDC_CHECK1, m_WireFrame);
	DDX_Control(pDX, IDC_CHECK2, m_PotentialCollision);
	DDX_Control(pDX, IDC_CHECK3, m_ColDisvisable);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectTool::OnStaticListSel)
	ON_LBN_SELCHANGE(IDC_LIST2, &CObjectTool::OnDynamicListSel)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectTool::OnInfoReset)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjectTool::OnInfoEdit)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectTool::OnResetStaticList)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnResetDynamicList)
	ON_BN_CLICKED(IDC_BUTTON5, &CObjectTool::OnObjSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CObjectTool::OnObjLoad)
	ON_BN_CLICKED(IDC_BUTTON9, &CObjectTool::OnLoadStaticMesh)
	ON_BN_CLICKED(IDC_BUTTON10, &CObjectTool::OnLoadDynamicMesh)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CObjectTool::OnRotXControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CObjectTool::OnRotYControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CObjectTool::OnRotZControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CObjectTool::OnPosXControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CObjectTool::OnPosYControl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CObjectTool::OnPosZControl)
	ON_BN_CLICKED(IDC_CHECK1, &CObjectTool::OnWireFrameCheck)
	ON_BN_CLICKED(IDC_CHECK2, &CObjectTool::OnPotentialCollision)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.

void CObjectTool::OnStaticListSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(LB_ERR != m_StaticList.GetCurSel())
	{
		if(m_ObjTypeRadio[0].GetCheck() == TRUE)
			m_StaticList.GetText(m_StaticList.GetCurSel(), m_strCurKey);
	}


}

void CObjectTool::OnDynamicListSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(LB_ERR != m_DynamicList.GetCurSel())
	{
		if(m_ObjTypeRadio[1].GetCheck() == TRUE)
			m_DynamicList.GetText(m_DynamicList.GetCurSel(), m_strCurKey);
	}
}



void CObjectTool::OnInfoReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");

		fScaleX = ((Engine::CTransform*)pComponent)->m_vScale.x;
		fScaleY = ((Engine::CTransform*)pComponent)->m_vScale.y;
		fScaleZ = ((Engine::CTransform*)pComponent)->m_vScale.z;

		fRotX = ((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_X];
		fRotY = ((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_Y];
		fRotZ = ((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_Z];
		fRotX = D3DXToDegree(fRotX);
		fRotY = D3DXToDegree(fRotY);
		fRotZ = D3DXToDegree(fRotZ);

		fPosX = ((Engine::CTransform*)pComponent)->m_vPos.x;
		fPosY = ((Engine::CTransform*)pComponent)->m_vPos.y;
		fPosZ = ((Engine::CTransform*)pComponent)->m_vPos.z;

		m_PotentialCollision.SetCheck(((Engine::CTransform*)pComponent)->m_bCollision);

		//m_stCurrentMeshKey = ((Engine::CTransform*)pComponent)->m_tcKey;
	
	}
	else
	{
		m_fScaleX = 0.f;
		m_fScaleY = 0.f;
		m_fScaleZ = 0.f;

		m_fRotX = 0.f;
		m_fRotY = 0.f;
		m_fRotZ = 0.f;

		m_fTransX = 0.f;
		m_fTransY = 0.f;
		m_fTransZ = 0.f;

		m_PotentialCollision.SetCheck(FALSE);


		
	}

	UpdateData(FALSE);
}

void CObjectTool::OnInfoEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if(m_pCurObject)
	{
		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_vScale.x = fScaleX;
		((Engine::CTransform*)pComponent)->m_vScale.y = fScaleY;
		((Engine::CTransform*)pComponent)->m_vScale.z = fScaleZ;

		((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(fRotX);
		((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(fRotY);
		((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian(fRotZ);

		((Engine::CTransform*)pComponent)->m_vPos.x = fPosX;
		((Engine::CTransform*)pComponent)->m_vPos.y = fPosY;
		((Engine::CTransform*)pComponent)->m_vPos.z = fPosZ;
	}

	UpdateData(FALSE);

	Engine::Update_Management(Engine::Get_Time());
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
}

void CObjectTool::OnResetStaticList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	m_StaticList.ResetContent();


}

void CObjectTool::OnResetDynamicList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);

	m_DynamicList.ResetContent();
}

void CObjectTool::OnObjSave()
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

	const Engine::CLayer* pLayer = Engine::Get_Scene()->GetLayer(CBack::LAYER_GAMELOGIC);

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	int StaticCnt = 0;
	int DynamicCnt = 0;

	CString pBuffer;

	int iObjNum = m_StaticList.GetCount();

	if(iObjNum !=0)
	{
		WriteFile(hFile, &iObjNum, sizeof(int), &dwByte, NULL);

		for(StaticCnt; StaticCnt < m_StaticList.GetCount(); ++StaticCnt)
		{
			TCHAR pObjectKey[50];
			ZeroMemory(pObjectKey, sizeof(TCHAR) * 50);

			m_StaticList.GetText(StaticCnt, pBuffer);
			lstrcpy(pObjectKey, (LPCTSTR)pBuffer);

			list<Engine::CGameObject*>* ObjList = const_cast<Engine::CLayer*>(pLayer)->GetObjectList((TCHAR*)(LPCTSTR)pBuffer);
			if(NULL == ObjList)
			{
				WriteFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);
				int iNum = 0;
				WriteFile(hFile, &iNum, sizeof(int), &dwByte, NULL);
				continue;
			}

			list<Engine::CGameObject*>::iterator iter = ObjList->begin();
			list<Engine::CGameObject*>::iterator iter_end = ObjList->end();

			WriteFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);
			int iNum = ObjList->size();
			WriteFile(hFile, &iNum, sizeof(int), &dwByte, NULL);

			for(iter; iter != iter_end; ++iter)
			{
				const Engine::CComponent* pComponent = (*iter)->GetComponent(L"Transform");
				if(m_StaticAreaRadio[1].GetCheck() == TRUE)
					WriteFile(hFile, &((Engine::CTransform*)pComponent)->m_bCollision, sizeof(bool), &dwByte, NULL);

				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_fAngle, sizeof(float) * Engine::ANGLE_END, &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);
			}
		}
	}

	int iDynamicNum = m_DynamicList.GetCount();

	if(iDynamicNum != 0)
	{
		WriteFile(hFile, &iDynamicNum, sizeof(int), &dwByte, NULL);

		for(DynamicCnt; DynamicCnt< m_DynamicList.GetCount(); ++DynamicCnt)
		{
			TCHAR pObjectKey[50];
			ZeroMemory(pObjectKey, sizeof(TCHAR) * 50);

			m_DynamicList.GetText(DynamicCnt, pBuffer);
			lstrcpy(pObjectKey, (LPCTSTR)pBuffer);

			list<Engine::CGameObject*>* ObjList = const_cast<Engine::CLayer*>(pLayer)->GetObjectList((TCHAR*)(LPCTSTR)pBuffer);
			if(NULL == ObjList)
			{
				WriteFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);
				int iNum = 0;
				WriteFile(hFile, &iNum, sizeof(int), &dwByte, NULL);
				continue;
			}

			list<Engine::CGameObject*>::iterator iter = ObjList->begin();
			list<Engine::CGameObject*>::iterator iter_end = ObjList->end();

			WriteFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);
			int iNum = ObjList->size();
			WriteFile(hFile, &iNum, sizeof(int), &dwByte, NULL);

			for(iter; iter != iter_end; ++iter)
			{
				const Engine::CComponent* pComponent = (*iter)->GetComponent(L"Transform");
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_fAngle, sizeof(float) * Engine::ANGLE_END, &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
				WriteFile(hFile, ((Engine::CTransform*)pComponent)->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);
			}
		}
	}
	CloseHandle(hFile);
}

void CObjectTool::OnObjLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_StaticList.GetCount() == 0)
	{
		AfxMessageBox(L"Load Static Mesh First!");
		return;
	}

	if(m_DynamicList.GetCount() == 0)
	{
		AfxMessageBox(L"Load Dynamic Mesh First!");
		return;
	}


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

	int iObjSize = 0;
	ReadFile(hFile, &iObjSize, sizeof(int), &dwByte, NULL);

	const Engine::CLayer* pLayer = Engine::Get_Scene()->GetLayer(CBack::LAYER_GAMELOGIC);

	for(int i = 0; i < iObjSize; ++i)
	{
		//TCHAR* tcFilePath = new TCHAR[128];
		TCHAR* pObjectKey = new TCHAR[50];
		//ZeroMemory(pObjectKey, sizeof(TCHAR) * 50);

		ReadFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);
		//const_cast<Engine::CLayer*>(pLayer)->DeleteByKey(pObjectKey);

		int iNum;
		ReadFile(hFile, &iNum, sizeof(int), &dwByte, NULL);
		if(0 == iNum)
			continue;

		Engine::CGameObject*	pGameObject = NULL;

		for(int i = 0; i < iNum; ++i)
		{
			if(m_StaticAreaRadio[0].GetCheck())
			{
				pGameObject = CDungeonObject::Create(Engine::Get_GraphicDev(),pObjectKey,D3DXVECTOR3(0.f,0.f,0.f));
				const_cast<Engine::CLayer*>(pLayer)->AddObject(pObjectKey , pGameObject);
			}
			else if(m_StaticAreaRadio[1].GetCheck())
			{
				pGameObject = CCityObject::Create(Engine::Get_GraphicDev(),pObjectKey,D3DXVECTOR3(0.f,0.f,0.f));
				const_cast<Engine::CLayer*>(pLayer)->AddObject(pObjectKey , pGameObject);
			}


			const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");
			if(m_StaticAreaRadio[1].GetCheck() == TRUE)
				ReadFile(hFile, &((Engine::CTransform*)pComponent)->m_bCollision, sizeof(bool), &dwByte, NULL);

			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_fAngle, sizeof(float) * Engine::ANGLE_END, &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);

			((CBack*)Engine::Get_Scene())->m_ObjectList.push_back(pGameObject);
		}
	}

	int iDynamicSize = 0;
	ReadFile(hFile, &iDynamicSize, sizeof(int), &dwByte, NULL);

	for(int i = 0; i < iDynamicSize; ++i)
	{
		TCHAR* pObjectKey = new TCHAR[50];

		ReadFile(hFile, pObjectKey, sizeof(TCHAR) * 50, &dwByte, NULL);
		//const_cast<Engine::CLayer*>(pLayer)->DeleteByKey(pObjectKey);

		int iNum;
		ReadFile(hFile, &iNum, sizeof(int), &dwByte, NULL);
		if(0 == iNum)
			continue;

		Engine::CGameObject*	pGameObject = NULL;

		for(int i = 0; i < iNum; ++i)
		{
			pGameObject = CDynamicObject::Create(Engine::Get_GraphicDev(),pObjectKey,D3DXVECTOR3(0.f,0.f,0.f));
			const_cast<Engine::CLayer*>(pLayer)->AddObject(pObjectKey , pGameObject);

			const Engine::CComponent* pComponent = pGameObject->GetComponent(L"Transform");
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_fAngle, sizeof(float) * Engine::ANGLE_END, &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_vDir, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, ((Engine::CTransform*)pComponent)->m_matWorld, sizeof(D3DXMATRIX), &dwByte, NULL);

			((CBack*)Engine::Get_Scene())->m_ObjectList.push_back(pGameObject);
		}
	}

	((CBack*)Engine::Get_Scene())->m_ObjectList.sort(CBack::Compare);

	CloseHandle(hFile);
}

void CObjectTool::OnLoadStaticMesh()
{


	HRESULT hr = NULL;
	CFileFind  finder, texfinder;
	BOOL bWorking = false;
	BOOL bTexWorking = false;
	vector<CString> vec1stPath, vec2ndPath, vec3rdPath;

	wofstream		SaveFile,SaveTex;



	if(m_StaticAreaRadio[0].GetCheck())
	{
		bWorking = finder.FindFile(_T("..//bin/Resources/Mesh/StaticMesh/Dungeon/*.*"));
		bTexWorking = texfinder.FindFile(_T("..//bin/Resources/Mesh/StaticMesh/Dungeon/Texture/*.tga"));
		GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
		SaveFile.open(L"..//bin/Resources/DungeonMeshPath.txt", ios::out);
		SaveTex.open(L"..//bin/Resources/DungeonMeshTexPath.txt", ios::out);
	}

	if(m_StaticAreaRadio[1].GetCheck())
	{
		bWorking = finder.FindFile(_T("..//bin/Resources/Mesh/StaticMesh/City/*.*"));
		bTexWorking = texfinder.FindFile(_T("..//bin/Resources/Mesh/StaticMesh/City/Texture/*.tga"));
		GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
		SaveFile.open(L"..//bin/Resources/CityMeshPath.txt", ios::out);
		SaveTex.open(L"..//bin/Resources/CityMeshTexPath.txt", ios::out);
	}

	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);


	//텍스쳐 검색해서 경로처리
	while(bTexWorking)
	{
		bTexWorking = texfinder.FindNextFileW();
		if(!texfinder.IsDirectory() && !texfinder.IsDots())
		{
			CString TexturePath = texfinder.GetFilePath();//경로
			CString TextureKey = texfinder.GetFileName();//키값
			//PathRemoveExtension((LPWSTR)(LPCTSTR)TextureKey);//확장자 날린다


			TCHAR* tcFilePath = new TCHAR[128];// = (TCHAR*)(LPCTSTR)(FilePath);
			wsprintf(tcFilePath, TexturePath);
			TCHAR* tcFileKey= new TCHAR[128];
			wsprintf(tcFileKey, TextureKey);



			if(m_bFirstLoad == false)
			{
				vecTexKey.push_back(tcFileKey);
				vecTexPath.push_back(tcFilePath);
			}
		}

	}

	if(m_bFirstLoad == false)
	{
		vector<TCHAR*>::iterator Pathiter = vecTexPath.begin();
		vector<TCHAR*>::iterator Keyiter = vecTexKey.begin();
		vector<TCHAR*>::iterator Pathiter_End = vecTexPath.end();
		vector<TCHAR*>::iterator Keyiter_End = vecTexKey.end();


		bool bEndofVector = false;

		while(!bEndofVector)
		{
			if(FAILED(Engine::Add_Texture(Engine::Get_GraphicDev()
				, RESOURCE_TOOL
				, Engine::TEXTURE_MESH
				, (*Keyiter)
				, (*Pathiter))))
			{
				MessageBox(L"Texture Load Failed",*Keyiter);
			}

			TCHAR szPath[MAX_PATH] = L"";
			TCHAR szDirPath[MAX_PATH] = L"";
			GetCurrentDirectory(sizeof(szDirPath),szDirPath);

			PathRelativePathTo((LPWSTR)szPath,
				szDirPath, FILE_ATTRIBUTE_DIRECTORY,
				*Pathiter,FILE_ATTRIBUTE_NORMAL);

			SaveTex <<(*Keyiter) << L"|";
			SaveTex << szPath << endl;


			++Keyiter;
			++Pathiter;

			if(Keyiter == Keyiter_End && Pathiter == Pathiter_End)
			{
				bEndofVector = true;
			}

		}

		bEndofVector = false;



		while(!bEndofVector)
		{
			vecTexKey.pop_back();
			vecTexPath.pop_back();
			if(vecTexKey.size() == 0 && vecTexPath.size() == 0)
			{
				bEndofVector = true;
				vecTexKey.clear();
				vecTexPath.clear();
				break;
			}

		}
	}


	//StaticMesh 폴더에서 경로찾기
	while(bWorking)
	{
		bWorking = finder.FindNextFileW();
		if(finder.IsDirectory() && !finder.IsDots())
		{
			vec1stPath.push_back(finder.GetFilePath());
		}
	}


	//City, Dungeon 폴더에서 경로찾기
	vector<CString>::iterator Firstiter = vec1stPath.begin();
	vector<CString>::iterator Firstiter_end = vec1stPath.end();



	//경로에서 문자열 처리
	Firstiter = vec1stPath.begin();
	for(Firstiter; Firstiter < Firstiter_end ; ++Firstiter)
	{
		bWorking = finder.FindFile(*Firstiter + _T("\\*.x"));
		while(bWorking)
		{
			bWorking = finder.FindNextFileW();
			if(finder.IsDirectory() && !finder.IsDots())
				continue;
			else
			{
				CString FilePath = finder.GetFilePath();//경로
				PathRemoveFileSpec((LPWSTR)(LPCTSTR)FilePath);//파일 이름을 날린다
				CString FileKey = finder.GetFileName();//컴포넌트 키값
				PathRemoveExtension((LPWSTR)(LPCTSTR)FileKey);//확장자 날린다
				CString FileName = finder.GetFileName();//x파일


				TCHAR* tcFilePath = new TCHAR[128];// = (TCHAR*)(LPCTSTR)(FilePath);
				wsprintf(tcFilePath, FilePath);
				_tcscat(tcFilePath,_T("\\"));
				TCHAR* tcFileKey= new TCHAR[128];
				wsprintf(tcFileKey, FileKey);
				TCHAR* tcFileName= new TCHAR[128];
				wsprintf(tcFileName, FileName);



				if(m_bFirstLoad == false)
				{
					vecFileKey.push_back(tcFileKey);
					vecFilename.push_back(tcFileName);
					vecFilePath.push_back(tcFilePath);
				}



				m_StaticList.AddString((LPWSTR)(LPCTSTR)FileKey);
			}
		}
	}



	//매쉬 등록
	if(m_bFirstLoad == false)
	{
		vector<TCHAR*>::iterator Pathiter = vecFilePath.begin();
		vector<TCHAR*>::iterator Nameiter = vecFilename.begin();
		vector<TCHAR*>::iterator Keyiter = vecFileKey.begin();
		vector<TCHAR*>::iterator Pathiter_End = vecFilePath.end();
		vector<TCHAR*>::iterator Nameiter_End = vecFilename.end();
		vector<TCHAR*>::iterator Keyiter_End = vecFileKey.end();


		bool bEndofVector = false;

		while(!bEndofVector)
		{
			if(FAILED(Engine::Add_Mesh(Engine::Get_GraphicDev()
				, RESOURCE_TOOL
				, Engine::MESH_STATIC
				, (*Keyiter)
				, (*Pathiter)
				, (*Nameiter))))
			{
				MessageBox(L"Mesh Load Failed Or Aready Exist Mesh",*Nameiter);
			}

			TCHAR szPath[MAX_PATH] = L"";
			TCHAR szDirPath[MAX_PATH] = L"";
			GetCurrentDirectory(sizeof(szDirPath),szDirPath);

			PathRelativePathTo((LPWSTR)szPath,
				szDirPath, FILE_ATTRIBUTE_DIRECTORY,
				*Pathiter,FILE_ATTRIBUTE_NORMAL);

			SaveFile <<(*Keyiter) << L"|";
			SaveFile << szPath << L"|";
			SaveFile << (*Nameiter) << endl;


			++Keyiter;
			++Pathiter;
			++Nameiter;

			if(Keyiter == Keyiter_End && Pathiter == Pathiter_End && Nameiter == Nameiter_End)
			{
				bEndofVector = true;
			}

		}

		bEndofVector = false;



		while(!bEndofVector)
		{
			vecFileKey.pop_back();
			vecFilename.pop_back();
			vecFilePath.pop_back();
			if(vecFileKey.size() == 0 && vecFilePath.size() == 0 && vecFilename.size() == 0)
			{
				bEndofVector = true;
				vecFileKey.clear();
				vecFilename.clear();
				vecFilePath.clear();
				break;
			}

		}


		m_bFirstLoad = true;
	}

	bool bErase = false;

	while(!bErase)
	{
		vec1stPath.pop_back();

		if(vec1stPath.size() == 0 )
		{
			bErase = true;
			vec1stPath.clear();
			break;
		}

	}



	SaveTex.close();

	SaveFile.close();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::OnLoadDynamicMesh()
{
	HRESULT hr = NULL;
	CFileFind finder, texfinder;
	bool bWorking = false;
	bool bTexWorking = false;
	vector<CString> vec1stPath, vec2ndPath;

	bWorking = finder.FindFile(_T("..//bin/Resources/Mesh/DynamicMesh/*.*"));
	bTexWorking = texfinder.FindFile(_T("..//bin/Resources/Mesh/DynamicMesh/Texture/*.tga"));
	GetDlgItem(IDC_BUTTON10)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);

	while(bWorking)
	{
		bWorking = finder.FindNextFileW();
		if(finder.IsDirectory() && !finder.IsDots())
		{
			vec1stPath.push_back(finder.GetFilePath());
		}
	}


	//텍스쳐 검색해서 경로처리
	while(bTexWorking)
	{
		bTexWorking = texfinder.FindNextFileW();
		if(!texfinder.IsDirectory() && !texfinder.IsDots())
		{
			CString TexturePath = texfinder.GetFilePath();//경로
			CString TextureKey = texfinder.GetFileName();//키값
			//PathRemoveExtension((LPWSTR)(LPCTSTR)TextureKey);//확장자 날린다


			TCHAR* tcFilePath = new TCHAR[128];// = (TCHAR*)(LPCTSTR)(FilePath);
			wsprintf(tcFilePath, TexturePath);
			TCHAR* tcFileKey= new TCHAR[128];
			wsprintf(tcFileKey, TextureKey);



			if(m_bFirstDynamicLoad == false)
			{
				vecTexKey.push_back(tcFileKey);
				vecTexPath.push_back(tcFilePath);
			}
		}

	}

	if(m_bFirstDynamicLoad == false)
	{
		vector<TCHAR*>::iterator Pathiter = vecTexPath.begin();
		vector<TCHAR*>::iterator Keyiter = vecTexKey.begin();
		vector<TCHAR*>::iterator Pathiter_End = vecTexPath.end();
		vector<TCHAR*>::iterator Keyiter_End = vecTexKey.end();


		bool bEndofVector = false;

		while(!bEndofVector)
		{
			if(FAILED(Engine::Add_Texture(Engine::Get_GraphicDev()
				, RESOURCE_TOOL
				, Engine::TEXTURE_MESH
				, (*Keyiter)
				, (*Pathiter))))
			{
				MessageBox(L"Texture Load Failed",*Keyiter);
			}

			TCHAR szPath[MAX_PATH] = L"";
			TCHAR szDirPath[MAX_PATH] = L"";
			GetCurrentDirectory(sizeof(szDirPath),szDirPath);

			PathRelativePathTo((LPWSTR)szPath,
				szDirPath, FILE_ATTRIBUTE_DIRECTORY,
				*Pathiter,FILE_ATTRIBUTE_NORMAL);



			++Keyiter;
			++Pathiter;

			if(Keyiter == Keyiter_End && Pathiter == Pathiter_End)
			{
				bEndofVector = true;
			}

		}

		bEndofVector = false;



		while(!bEndofVector)
		{
			vecTexKey.pop_back();
			vecTexPath.pop_back();
			if(vecTexKey.size() == 0 && vecTexPath.size() == 0)
			{
				bEndofVector = true;
				vecTexKey.clear();
				vecTexPath.clear();
				break;
			}

		}
	}

	//각 폴더에서 경로찾기
	vector<CString>::iterator Firstiter = vec1stPath.begin();
	vector<CString>::iterator Firstiter_end = vec1stPath.end();

	Firstiter = vec1stPath.begin();
	for(Firstiter; Firstiter < Firstiter_end ; ++Firstiter)
	{
		bWorking = finder.FindFile(*Firstiter + _T("\\*.x"));
		while(bWorking)
		{
			bWorking = finder.FindNextFileW();
			if(finder.IsDirectory() && !finder.IsDots())
				continue;
			else
			{
				CString FilePath = finder.GetFilePath();//경로
				PathRemoveFileSpec((LPWSTR)(LPCTSTR)FilePath);//파일 이름을 날린다
				CString FileKey = finder.GetFileName();//컴포넌트 키값
				PathRemoveExtension((LPWSTR)(LPCTSTR)FileKey);//확장자 날린다
				CString FileName = finder.GetFileName();//x파일


				TCHAR* tcFilePath = new TCHAR[128];// = (TCHAR*)(LPCTSTR)(FilePath);
				wsprintf(tcFilePath, FilePath);
				_tcscat(tcFilePath,_T("\\"));
				TCHAR* tcFileKey= new TCHAR[128];
				wsprintf(tcFileKey, FileKey);
				TCHAR* tcFileName= new TCHAR[128];
				wsprintf(tcFileName, FileName);



				if(m_bFirstDynamicLoad == false)
				{
					vecFileKey.push_back(tcFileKey);
					vecFilename.push_back(tcFileName);
					vecFilePath.push_back(tcFilePath);
				}



				m_DynamicList.AddString((LPWSTR)(LPCTSTR)FileKey);
			}
		}
	}

	if(m_bFirstDynamicLoad == false)
	{
		vector<TCHAR*>::iterator Pathiter = vecFilePath.begin();
		vector<TCHAR*>::iterator Nameiter = vecFilename.begin();
		vector<TCHAR*>::iterator Keyiter = vecFileKey.begin();
		vector<TCHAR*>::iterator Pathiter_End = vecFilePath.end();
		vector<TCHAR*>::iterator Nameiter_End = vecFilename.end();
		vector<TCHAR*>::iterator Keyiter_End = vecFileKey.end();


		bool bEndofVector = false;

		while(!bEndofVector)
		{
			if(FAILED(Engine::Add_Mesh(Engine::Get_GraphicDev()
				, RESOURCE_TOOL
				, Engine::MESH_STATIC
				, (*Keyiter)
				, (*Pathiter)
				, (*Nameiter))))
			{
				MessageBox(L"Mesh Load Failed Or Aready Exist Mesh",*Nameiter);
			}

			TCHAR szPath[MAX_PATH] = L"";
			TCHAR szDirPath[MAX_PATH] = L"";
			GetCurrentDirectory(sizeof(szDirPath),szDirPath);

			PathRelativePathTo((LPWSTR)szPath,
				szDirPath, FILE_ATTRIBUTE_DIRECTORY,
				*Pathiter,FILE_ATTRIBUTE_NORMAL);


			++Keyiter;
			++Pathiter;
			++Nameiter;

			if(Keyiter == Keyiter_End && Pathiter == Pathiter_End && Nameiter == Nameiter_End)
			{
				bEndofVector = true;
			}

		}

		bEndofVector = false;



		while(!bEndofVector)
		{
			vecFileKey.pop_back();
			vecFilename.pop_back();
			vecFilePath.pop_back();
			if(vecFileKey.size() == 0 && vecFilePath.size() == 0 && vecFilename.size() == 0)
			{
				bEndofVector = true;
				vecFileKey.clear();
				vecFilename.clear();
				vecFilePath.clear();
				break;
			}

		}


		m_bFirstDynamicLoad = true;
	}

	bool bErase = false;

	while(!bErase)
	{
		vec1stPath.pop_back();

		if(vec1stPath.size() == 0 )
		{
			bErase = true;
			vec1stPath.clear();
			break;
		}

	}

	
	


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CObjectTool::SetPickPos(D3DXVECTOR3 vPos)
{
	UpdateData(TRUE);

	fPosX = vPos.x;
	fPosY = vPos.y;
	fPosZ = vPos.z;

	UpdateData(FALSE);
}
void CObjectTool::OnRotXControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		if(pNMUpDown->iDelta < 0)
		{
			++fRotX;
			
			if(fRotX > 361)
				fRotX = (int)fRotX % 360;
			else if(fRotX > 360)
				fRotX = 1;
		}
		else
		{
			--fRotX;
			if(fRotX < 0)
				fRotX = 359;
		}
		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_X] = D3DXToRadian(fRotX);

		SetDlgItemInt(IDC_EDIT4,(float)fRotX);

	}
	*pResult = 0;
}

void CObjectTool::OnRotYControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		if(pNMUpDown->iDelta < 0)
		{
			++fRotY;

			if(fRotY > 361)
				fRotY = (int)fRotY % 360;
			else if(fRotY > 360)
				fRotY = 1;
		}
		else
		{
			--fRotY;
			if(fRotY < 0)
				fRotY = 359;
		}
		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(fRotY);

		SetDlgItemInt(IDC_EDIT5,(float)fRotY);

	}
	*pResult = 0;
}

void CObjectTool::OnRotZControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		if(pNMUpDown->iDelta < 0)
		{
			++fRotZ;

			if(fRotZ > 361)
				fRotZ = (int)fRotZ % 360;
			else if(fRotZ > 360)
				fRotZ = 1;
		}
		else
		{
			--fRotZ;
			if(fRotZ < 0)
				fRotZ = 359;
		}
		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");

		((Engine::CTransform*)pComponent)->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian(fRotZ);

		SetDlgItemInt(IDC_EDIT6,(float)fRotZ);

	}
	*pResult = 0;
}

void CObjectTool::OnPosXControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		CString stPosX = _T("0.0000");
		
		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++fPosX;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				fPosX += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				fPosX += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				fPosX += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				fPosX += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				fPosX += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--fPosX;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				fPosX -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				fPosX -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				fPosX -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				fPosX -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				fPosX -= 0.00001f;
		}

		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");
		((Engine::CTransform*)pComponent)->m_vPos.x = fPosX;

		stPosX.Format(_T("%.5f"),fPosX);

		SetDlgItemText(IDC_EDIT7,stPosX);


	}
	*pResult = 0;
}

void CObjectTool::OnPosYControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		CString stPosY = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++fPosY;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				fPosY += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				fPosY += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				fPosY += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				fPosY += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				fPosY += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--fPosY;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				fPosY -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				fPosY -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				fPosY -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				fPosY -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				fPosY -= 0.00001f;
		}

		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");
		((Engine::CTransform*)pComponent)->m_vPos.y = fPosY;

		stPosY.Format(_T("%.5f"),fPosY);

		SetDlgItemText(IDC_EDIT8,stPosY);


	}
	*pResult = 0;
}

void CObjectTool::OnPosZControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_pCurObject)
	{
		CString stPosZ = _T("0.0000");

		if(pNMUpDown->iDelta < 0)
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				++fPosZ;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				fPosZ += 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				fPosZ += 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				fPosZ += 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				fPosZ += 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				fPosZ += 0.00001f;
		}
		else
		{
			if(m_PosDecimal[0].GetCheck() == TRUE)
				--fPosZ;
			else if(m_PosDecimal[1].GetCheck() == TRUE)
				fPosZ -= 0.1f;
			else if(m_PosDecimal[2].GetCheck() == TRUE)
				fPosZ -= 0.01f;
			else if(m_PosDecimal[3].GetCheck() == TRUE)
				fPosZ -= 0.001f;
			else if(m_PosDecimal[4].GetCheck() == TRUE)
				fPosZ -= 0.0001f;
			else if(m_PosDecimal[5].GetCheck() == TRUE)
				fPosZ -= 0.00001f;
		}

		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");
		((Engine::CTransform*)pComponent)->m_vPos.z = fPosZ;

		stPosZ.Format(_T("%.5f"),fPosZ);

		SetDlgItemText(IDC_EDIT9,stPosZ);


	}
	*pResult = 0;
}

void CObjectTool::OnWireFrameCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNaviMeshTool* pNaviTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab3;

	if(m_WireFrame.GetCheck() == TRUE)
	{
		pNaviTool->m_ObjectWire.SetCheck(TRUE);
	}
	else
	{
		pNaviTool->m_ObjectWire.SetCheck(FALSE);
	}
}

void CObjectTool::OnPotentialCollision()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_pCurObject)
	{
		
		const Engine::CComponent* pComponent = m_pCurObject->GetComponent(L"Transform");

		if(((Engine::CTransform*)pComponent)->m_bCollision == false)
		{
			((Engine::CTransform*)pComponent)->m_bCollision = TRUE;
			m_PotentialCollision.SetCheck(TRUE);
		}
		else if(((Engine::CTransform*)pComponent)->m_bCollision == true)
		{
			((Engine::CTransform*)pComponent)->m_bCollision = FALSE;
			m_PotentialCollision.SetCheck(FALSE);
		}

	}
}
