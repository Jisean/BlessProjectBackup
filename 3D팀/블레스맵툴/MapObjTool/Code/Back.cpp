#include "stdafx.h"
#include "Back.h"
#include "Export_Function.h"
#include "Engine_Include.h"
#include "Layer.h"
#include "MainFrm.h"
#include "MapObjToolView.h"
#include "DynamicCamera.h"
#include "Terrain.h"
#include "FileInfo.h"
#include "ObjectTool.h"
#include "MyForm.h"
#include "Resource.h"
#include "DungeonObject.h"
#include "Transform.h"
#include "CityObject.h"
#include "..\Engine\Resources\Code\CubeColor.h"
#include "..\Engine\Resources\Code\Mesh.h"
#include "NaviCell.h"
#include "DynamicObject.h"

CBack::CBack(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_eSceneType(SCENE_BACK)
, m_pMouseCol(NULL)
, m_pTerrainVtx(NULL)
, m_bGetMousePt(false) 
, m_bCameraFix(false)
, m_bCamSpeedUp(false)
, m_bCamSpeedDown(false)
, m_vGetMousePt(0.f, 0.f, 0.f)
, m_bPickFirst(false)
, m_bPickSecond(false)
, m_bPickCheck(false)
, m_iNaviIndex(0)
, m_pSelectedCell(NULL)
, m_vPickingPos(0.f, 0.f, 0.f)
, m_pOccVtx(NULL)
, m_pOccIdx(NULL)
{
	
}

CBack::~CBack(void)
{

}

HRESULT CBack::InitScene(void)
{
	HRESULT hr = NULL;
	m_pMouseCol = new CMouseCol(m_pDevice);
	FAILED_CHECK_RETURN_MSG(m_pMouseCol->Initialize(), E_FAIL, L"마우스 콜 실패!")


	FAILED_CHECK_RETURN_MSG(Add_TexBuffer(), E_FAIL, L"TexBuffer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Light Add false");

	m_pOccVtx	= new Engine::VTXCOL[8];
	m_pOccIdx	= new Engine::INDEX16[12];

	return S_OK;

}

int CBack::Update(const float& fTimeDelta)
{
	if(GetAsyncKeyState('P'))
	{
		if(m_bCameraFix == true)
			m_bCameraFix = false;
	}
	if(GetAsyncKeyState('O'))
	{
		if(m_bCameraFix == false)
			m_bCameraFix = true;
	}

	if(GetAsyncKeyState('9'))
	{
		if(m_bCamSpeedUp == true || m_bCamSpeedDown == true)
		{
			m_bCamSpeedUp = false;
			m_bCamSpeedDown = false;
		}
	}
	if(GetAsyncKeyState('0'))
	{
		if(m_bCamSpeedUp == false)
		{
			m_bCamSpeedUp = true;
			m_bCamSpeedDown = false;
		}
	}
	if(GetAsyncKeyState('8'))
	{
		if(m_bCamSpeedDown == false)
		{
			m_bCamSpeedDown = true;
			m_bCamSpeedUp = false;
		}
	}

	if(GetAsyncKeyState(VK_SPACE))
	{
		ConstMapMode();
	}
	return Engine::CScene::Update(fTimeDelta);
}

void CBack::Render(void)
{

	Render_FPS();

	Engine::Render_Navi();
	Engine::Render_OccBox();


	MEMORYSTATUS memoryStatus;
	GlobalMemoryStatus(&memoryStatus);

	CString stCamX, stCamY, stCamZ;


	D3DXMATRIX matView, matViewInv;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matViewInv,NULL,&matView);

	stCamX.Format(_T("%.3f"),matViewInv._41);
	stCamY.Format(_T("%.3f"),matViewInv._42);
	stCamZ.Format(_T("%.3f"),matViewInv._43);
	
	wsprintf(m_szMemoryLoad, L"Memory Share Rate: %d%%", memoryStatus.dwMemoryLoad);
	
	SIZE_T dwAvailVirtual = memoryStatus.dwAvailVirtual>>20;
	SIZE_T dwTotalVirtual = memoryStatus.dwTotalVirtual>>20;

	wsprintf(m_szTotal, L"Total Memory: %dMB/%dMB",  dwTotalVirtual - dwAvailVirtual, dwTotalVirtual);




	if(m_bCameraFix == false)
	{
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(475.f, 0.f, 0.f), L"'O':CamFix", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(605.f, 0.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(635.f, 0.f, 0.f), L"'P':CamMove", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	else if (m_bCameraFix == true)
	{
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(475.f, 0.f, 0.f), L"'O':CamFix", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(605.f, 0.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(635.f, 0.f, 0.f), L"'P':CamMove", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	}

	if(m_bCamSpeedUp == false && m_bCamSpeedDown == false)
	{
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(290.f, 25.f, 0.f), L"'8':CamSlow", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(425.f, 25.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(450.f, 25.f, 0.f), L"'9':CamNormal", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(615.f, 25.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(640.f, 25.f, 0.f), L"'0':CamBoost", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	}
	else if( m_bCamSpeedUp == true)
	{
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(290.f, 25.f, 0.f), L"'8':CamSlow", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(425.f, 25.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(450.f, 25.f, 0.f), L"'9':CamNormal", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(615.f, 25.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(640.f, 25.f, 0.f), L"'0':CamBoost", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}

	else if( m_bCamSpeedDown == true)
	{
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(290.f, 25.f, 0.f), L"'8':CamSlow", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(425.f, 25.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(450.f, 25.f, 0.f), L"'9':CamNormal", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(615.f, 25.f, 0.f), L" / ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
		Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(640.f, 25.f, 0.f), L"'0':CamBoost", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	}

	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 50.f, 0.f), L"CameraX", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(710.f, 50.f, 0.f), stCamX, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 75.f, 0.f), L"CameraY", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(710.f, 75.f, 0.f), stCamY, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 100.f, 0.f), L"CameraZ", D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(710.f, 100.f, 0.f), stCamZ, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(0.f,35.f,0.f), m_szMemoryLoad, D3DXCOLOR(1.f,0.f,0.f,1.f));
	Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(0.f,50.f,0.f), m_szTotal, D3DXCOLOR(1.f,0.f,0.f,1.f));

	if(((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_iCurToolIndex == 2)
	{
		if(m_bPickFirst == false && m_bPickSecond == false)
		{
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 125.f, 0.f), L"PointA: False", D3DXCOLOR(1.f,1.f,0.f,1.f));
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 150.f, 0.f), L"PointB: False", D3DXCOLOR(1.f,1.f,0.f,1.f));
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 175.f, 0.f), L"PointC: False", D3DXCOLOR(1.f,1.f,0.f,1.f));
		}
		else if(m_bPickFirst == true && m_bPickSecond == false)
		{
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 125.f, 0.f), L"PointA: True", D3DXCOLOR(1.f,0.f,0.f,1.f));
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 150.f, 0.f), L"PointB: False", D3DXCOLOR(1.f,1.f,0.f,1.f));
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 175.f, 0.f), L"PointC: False", D3DXCOLOR(1.f,1.f,0.f,1.f));
		}
		else
		{
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 125.f, 0.f), L"PointA: True", D3DXCOLOR(1.f,0.f,0.f,1.f));
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 150.f, 0.f), L"PointB: True", D3DXCOLOR(1.f,0.f,0.f,1.f));
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 175.f, 0.f), L"PointC: False", D3DXCOLOR(1.f,1.f,0.f,1.f));
		}


		if(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab3.m_EditMode[1].GetCheck() == TRUE)
		{
			TCHAR szCellIndex[128] = L"";
			wsprintf(szCellIndex, L"CellIndex : %d", m_iNaviIndex);
			Engine::Render_Font(L"Comic Sans Ms", D3DXVECTOR3(600.f, 200.f, 0.f), szCellIndex, D3DXCOLOR(1.f,1.f,0.f,1.f));
		}
	}



}

HRESULT CBack::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CBack::Add_TexBuffer(void)
{
	HRESULT hr = NULL;
	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_TOOL
		, Engine::TEXTURE_NORMAL
		, L"Texture_Terrain"
		, L"../bin/Resources/Texture/Terrain/Grass_%d.tga", 2);
	FAILED_CHECK(hr);

	hr = Engine::Add_Texture(m_pDevice
		, RESOURCE_TOOL
		, Engine::TEXTURE_NORMAL
		, L"Texture_Filter"
		, L"../bin/Resources/Texture/Terrain/ColorHeight%d.png", 7);
	FAILED_CHECK(hr);

	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_TOOL
		, Engine::CVIBuffer::BUFFER_TERRAIN
		, L"Buffer_Terrain"
		, VTXCNTX, VTXCNTZ, VTXITV);
	FAILED_CHECK(hr);

	hr = Engine::Add_Buffer(m_pDevice
		, RESOURCE_TOOL
		, Engine::CVIBuffer::BUFFER_CUBECOL
		, L"Buffer_OccBox");
	FAILED_CHECK(hr);

	return S_OK;

}

HRESULT CBack::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Terrain 생성 실패");
	m_pTerrain = ((CTerrain*)pGameObject);
	pLayer->AddObject(L"Terrain" , pGameObject);

	//pGameObject = CDungeonObject::Create(m_pDevice, L"Mesh_Stone");
	//NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Object 생성 실패");	
	//pLayer->AddObject(L"DungeonObject" , pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CBack::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CDynamicCamera::Create(m_pDevice, &D3DXVECTOR3(0.f, 5.f, -10.f), &D3DXVECTOR3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"DynamicCamera 생성 실패");
	pLayer->AddObject(L"DynamicCamera" , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));

	return S_OK;
}

HRESULT CBack::Add_Light(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);

	HRESULT		hr = NULL;
	hr = Engine::Add_Light(m_pDevice, &LightInfo, 0);
	FAILED_CHECK(hr);



	return S_OK;
}

void CBack::SetMainView(CMapObjToolView* pMainView)
{
	m_pMainView = pMainView;
}

void CBack::ConstMapMode(void)
{
	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	MAPLAYER::iterator iter = m_mapLayer.find(LAYER_GAMELOGIC);
	Engine::CLayer* pLayer = iter->second;

	//m_pTerrain = ((CTerrain*)pLayer);
	if(m_bPickCheck == false)
	{
		D3DXVECTOR3 vIndex;

		if(NULL == m_pTerrainVtx)
		{
			m_pTerrainVtx = new Engine::VTXTEX[pMapTool->m_iCntX * pMapTool->m_iCntZ];
			m_pMouseCol->SetSize(pMapTool->m_iCntX, pMapTool->m_iCntZ, pMapTool->m_iItv);

			m_pTerrainVtx = m_pTerrain->GetTerrainVtx();
		}
		m_pMouseCol->PickTerrainVertex(&vIndex, m_pTerrainVtx);


		if(TRUE == pMapTool->m_MapEditMode[1].GetCheck())
		{
			pMapTool->UpdateData(FALSE);
			DOUBLE dChangeSize = pMapTool->m_dPickSize;
			float fHeight = pMapTool->m_fSetHeight;
			//float fRealHeight = fHeight;
			if(TRUE == pMapTool->m_ModifyMode[0].GetCheck())
			{
				//fHeight += 0.05f;
				pMapTool->m_fSetHeight = fHeight;
			}
			else if(TRUE == pMapTool->m_ModifyMode[1].GetCheck())
			{
				//fHeight -= 0.05f;
				pMapTool->m_fSetHeight = fHeight;
			}
			else if(TRUE == pMapTool->m_ModifyMode[2].GetCheck())
			{
				//if(0 <= vIndex.y)
				//	fHeight -= 0.05f;
				//else
				//	fHeight += 0.05f;
				fHeight = vIndex.y;
			}

			HRESULT hr = ((CBack*)Engine::Get_Scene())->m_pTerrain->ChangeHeight(vIndex, fHeight, dChangeSize);
			if(S_OK == hr)
				((CBack*)Engine::Get_Scene())->m_pTerrain->SetNormal();
		}
	}
	//else
	//{
	//	//Engine::VTXTEX* pVtx = NULL;
	//	//&m_vVtxPickingPos, m_pTerrainVtx, m_pBuffer->GetVBInfo(), false, m_fRange);
	//	m_pMouseCol->PickSetTerrain(&m_vPickingPos, m_pTerrainVtx, m_pTerrain->m_pBuffer->GetVBInfo(), true, 3.f);
	//}
}

void CBack::ConstObjectMode(void)
{

	CObjectTool* pObjectTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab2;
	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;

	if(TRUE == pObjectTool->m_ObjModeRadio[1].GetCheck())
	{
		// 설치
		D3DXVECTOR3 vIndex;

		if(NULL == m_pTerrainVtx)
		{
			m_pTerrainVtx = new Engine::VTXTEX[pMapTool->m_iCntX * pMapTool->m_iCntZ];
			m_pMouseCol->SetSize(pMapTool->m_iCntX, pMapTool->m_iCntZ, pMapTool->m_iItv);

			m_pTerrainVtx = m_pTerrain->GetTerrainVtx();
		}

		m_pMouseCol->PickTerrain(&vIndex, m_pTerrainVtx);

		Engine::CGameObject*	pGameObject = NULL;
		MAPLAYER::iterator iter = m_mapLayer.find(LAYER_GAMELOGIC);

		const TCHAR* strKey = (TCHAR*)(LPCTSTR)pObjectTool->m_strCurKey;


		if(TRUE == pObjectTool->m_ObjTypeRadio[0].GetCheck())
		{
			if(TRUE == pObjectTool->m_StaticAreaRadio[0].GetCheck())
			{
				TCHAR* pp = new TCHAR[50];
				wsprintf(pp, pObjectTool->m_strCurKey);
				pGameObject = CDungeonObject::Create(m_pDevice, pp, vIndex);
				iter->second->AddObject( pp, pGameObject);
			}
			else if(TRUE == pObjectTool->m_StaticAreaRadio[1].GetCheck())
			{
				TCHAR* pp = new TCHAR[50];
				wsprintf(pp, pObjectTool->m_strCurKey);
				pGameObject = CCityObject::Create(m_pDevice, pp, vIndex);
				iter->second->AddObject( pp, pGameObject);
			}
		}
		else if(TRUE == pObjectTool->m_ObjTypeRadio[1].GetCheck())
		{
			//다이나믹
			TCHAR* pp = new TCHAR[50];
			wsprintf(pp, pObjectTool->m_strCurKey);
			pGameObject = CDynamicObject::Create(m_pDevice, pp, vIndex);
			iter->second->AddObject( pp, pGameObject);
		}


		if(pGameObject)
		{
			m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, iter->second));
			m_ObjectList.push_back(pGameObject);
		}

		m_ObjectList.sort(CBack::Compare);

	}


	else if(TRUE == pObjectTool->m_ObjModeRadio[2].GetCheck())
	{
		// 선택 및 수정
		D3DXVECTOR3 vIndex;

		OBJECTLIST::iterator iter		= m_ObjectList.begin();
		OBJECTLIST::iterator iter_end	= m_ObjectList.end();

		for(iter; iter != iter_end; ++iter)
		{
			((CLandObject*)(*iter))->SetCollision(false);
		}
		iter = m_ObjectList.begin();

		const Engine::CComponent* pInfoComponenet;
		const Engine::CComponent* pMeshComponenet;
		for(iter; iter != iter_end; ++iter)
		{
			pInfoComponenet = (*iter)->GetComponent(L"Transform");
			pMeshComponenet = (*iter)->GetComponent(L"Mesh");

			D3DXVECTOR3 temp;
			if(m_pMouseCol->MeshPick(&temp,((Engine::CMesh*)pMeshComponenet)->GetMesh(),((Engine::CTransform*)pInfoComponenet)->m_matWorld))
				break;
		}
		if(iter == iter_end)
		{
			iter = m_ObjectList.begin();
			for(iter; iter != iter_end; ++iter)
			{
				((CLandObject*)(*iter))->SetCollision(false);
			}
			pObjectTool->m_stCurrentMeshKey = L"";
			pObjectTool->SetCurObject();
			pObjectTool->OnInfoReset();
			
			return;
		}

		((CLandObject*)(*iter))->SetCollision(true);
	
		pObjectTool->m_stCurrentMeshKey = ((CLandObject*)(*iter))->GetKey();
		pObjectTool->SetCurObject(*iter);
		pObjectTool->OnInfoReset();
		
	}
	else if(TRUE == pObjectTool->m_ObjModeRadio[0].GetCheck())
	{
		// 마우스좌표 받기 및 수정
		D3DXVECTOR3 vIndex;

		if(NULL == m_pTerrainVtx)
		{
			m_pTerrainVtx = new Engine::VTXTEX[pMapTool->m_iCntX * pMapTool->m_iCntZ];
			m_pMouseCol->SetSize(pMapTool->m_iCntX, pMapTool->m_iCntZ, pMapTool->m_iItv);

			m_pTerrainVtx = m_pTerrain->GetTerrainVtx();
		}

		m_pMouseCol->PickTerrain(&vIndex, m_pTerrainVtx);

		pObjectTool->SetPickPos(vIndex);
	}
	else if(TRUE == pObjectTool->m_ObjModeRadio[3].GetCheck())
	{
		// 삭제
		D3DXVECTOR3 vIndex;

		OBJECTLIST::iterator iter		= m_ObjectList.begin();
		OBJECTLIST::iterator iter_end	= m_ObjectList.end();

		for(iter; iter != iter_end; ++iter)
		{
			((CLandObject*)(*iter))->SetCollision(false);
		}
		iter = m_ObjectList.begin();

		const Engine::CComponent* pInfoComponenet;
		const Engine::CComponent* pMeshComponenet;
		for(iter; iter != iter_end; ++iter)
		{
			pInfoComponenet = (*iter)->GetComponent(L"Transform");
			pMeshComponenet = (*iter)->GetComponent(L"Mesh");

			D3DXVECTOR3 temp;
			if(m_pMouseCol->MeshPick(&temp,((Engine::CMesh*)pMeshComponenet)->GetMesh(),((Engine::CTransform*)pInfoComponenet)->m_matWorld))
				break;
		}

		if(iter == iter_end)
		{
			iter = m_ObjectList.begin();
			for(iter; iter != iter_end; ++iter)
			{
				((CLandObject*)(*iter))->SetCollision(false);
			}
			pObjectTool->m_stCurrentMeshKey = L"";
			pObjectTool->SetCurObject();
			pObjectTool->OnInfoReset();
			return;
		}

		pObjectTool->SetCurObject(*iter);
		pObjectTool->OnInfoReset();

		MAPLAYER::iterator pMapIter = m_mapLayer.find(LAYER_GAMELOGIC);
		pMapIter->second->DeleteByCompare(*iter);

		m_ObjectList.erase(iter);
		Engine::Safe_Release(pObjectTool->m_pCurObject);

		pObjectTool->m_stCurrentMeshKey = L"";
		pObjectTool->SetCurObject();
		pObjectTool->OnInfoReset();
	}


}


void CBack::ConstSplatingMode(void)
{
	CSplatingTool* pSplatTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab4;
	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	MAPLAYER::iterator iter = m_mapLayer.find(LAYER_GAMELOGIC);
	Engine::CLayer* pLayer = iter->second;

	//m_pTerrain = ((CTerrain*)pLayer);

	D3DXVECTOR3 vIndex;

	if(NULL == m_pTerrainVtx)
	{
		m_pTerrainVtx = new Engine::VTXTEX[pMapTool->m_iCntX * pMapTool->m_iCntZ];
		m_pMouseCol->SetSize(pMapTool->m_iCntX, pMapTool->m_iCntZ, pMapTool->m_iItv);

		m_pTerrainVtx = m_pTerrain->GetTerrainVtx();
	}
	m_pMouseCol->PickTerrainVertex(&vIndex, m_pTerrainVtx);

	

	if(pSplatTool->IsWindowVisible() == TRUE)
	{
		if(pSplatTool->m_SplatingMode[1].GetCheck() == TRUE)
			((CBack*)Engine::Get_Scene())->m_pTerrain->DrawAlphamaps(vIndex,0, pSplatTool->m_fBrushSize, pSplatTool->m_fAlphaRadius);
		else if(pSplatTool->m_SplatingMode[2].GetCheck() == TRUE)
			((CBack*)Engine::Get_Scene())->m_pTerrain->EraseAlphamaps(vIndex,0, pSplatTool->m_fBrushSize, pSplatTool->m_fAlphaRadius);
	}
}


HRESULT CBack::MakeTerrainBuffer(WORD wCntX, WORD wCntZ, WORD wItv)
{
	Engine::Delete_ByKey(RESOURCE_TOOL, L"Buffer_Terrain");

	HRESULT hr;
	hr = Engine::Add_Buffer(m_pDevice, RESOURCE_TOOL, 
		Engine::CVIBuffer::BUFFER_TERRAIN, L"Buffer_Terrain", wCntX, wCntZ, wItv);
	FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CBack::MakeTerrainLayer(WORD wCntX, WORD wCntZ, float fUVCnt)
{
	MAPLAYER::iterator iter = m_mapLayer.find(LAYER_GAMELOGIC);
	Engine::CLayer* pLayer = iter->second;
	pLayer->DeleteByKey(L"Terrain");
	m_pTerrainVtx = NULL;
	m_pTerrain = NULL;

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CTerrain::Create(m_pDevice, wCntX, wCntZ, fUVCnt);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	m_pTerrain = ((CTerrain*)pGameObject);
	pLayer->AddObject(L"Terrain", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));	

	return S_OK;
}
void CBack::NavimeshMode(void)
{
	CMapTool*		pMapTool		= &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	CObjectTool*	pObjectTool		= &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab2;
	CNaviMeshTool*	pNavimeshTool	= &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab3;

	D3DXVECTOR3 vIndex;


	if(NULL == m_pTerrainVtx) //지형이 세팅되지 않았을경우 세팅한다.
	{
		m_pTerrainVtx = new Engine::VTXTEX[pMapTool->m_iCntX * pMapTool->m_iCntZ];
		m_pMouseCol->SetSize(pMapTool->m_iCntX, pMapTool->m_iCntZ, pMapTool->m_iItv);

		m_pTerrainVtx = m_pTerrain->GetTerrainVtx();
	}

	if(TRUE == pNavimeshTool->m_PickingMode[0].GetCheck()) 
	{
		//지형만 클릭대상이 된 경우. 클릭된 버텍스의 좌표값을 받아온다.
		m_pMouseCol->PickTerrain(&vIndex, m_pTerrainVtx);
	}
	else if(TRUE == pNavimeshTool->m_PickingMode[1].GetCheck())
	{
		//메쉬까지 클릭대상이 된 경우. 메쉬를 순회해서 해당 메쉬의 클릭된 버텍스 좌표값을 받아온다.
		OBJECTLIST::iterator iter		= m_ObjectList.begin();
		OBJECTLIST::iterator iter_end	= m_ObjectList.end();

		const Engine::CComponent* pInfoComponenet;
		const Engine::CComponent* pMeshComponenet;
		for(iter; iter != iter_end; ++iter)
		{
			pInfoComponenet = (*iter)->GetComponent(L"Transform");
			pMeshComponenet = (*iter)->GetComponent(L"Mesh");

			D3DXVECTOR3 temp;
			if(m_pMouseCol->MeshPick(&vIndex,((Engine::CMesh*)pMeshComponenet)->GetMesh(),((Engine::CTransform*)pInfoComponenet)->m_matWorld))
			{
				//vIndex = vIndex + ((Engine::CTransform*)pInfoComponenet)->m_vPos;
				D3DXVec3TransformCoord(&vIndex, &vIndex, &((Engine::CTransform*)pInfoComponenet)->m_matWorld);
				break;
			}
		}
		if(iter == iter_end)
		{
			//메쉬가 아니라면 지형이므로 지형에 클릭된 버텍스의 좌표값을 받아온다.
			m_pMouseCol->PickTerrain(&vIndex, m_pTerrainVtx);
		}
	}
	//받아온 정보값으로 
	pNavimeshTool->SetPickPos(vIndex);


	if (vIndex.x < 0 || vIndex.z < 0)
	{
		return;
	}

	if(TRUE == pNavimeshTool->m_NaviMode[1].GetCheck())
	{
		// 3점 찍기
		if(TRUE == pNavimeshTool->m_CreateMode[0].GetCheck())
		{
			if(!m_bPickFirst)
			{
				//if(TRUE == pNavimeshTool->m_PickingMode[0].GetCheck())
					Engine::Get_NearPoint(vIndex, pNavimeshTool->m_fNearRange);

				m_vPoint[0] = vIndex;
				m_bPickFirst = true;
			}
			else if(m_bPickFirst && !m_bPickSecond)
			{
				//if(TRUE == pNavimeshTool->m_PickingMode[0].GetCheck())
					Engine::Get_NearPoint(vIndex, pNavimeshTool->m_fNearRange);
				m_vPoint[1] = vIndex;
				m_bPickSecond = true;
			}
			else if(m_bPickFirst && m_bPickSecond)
			{
				//if(TRUE == pNavimeshTool->m_PickingMode[0].GetCheck())
					Engine::Get_NearPoint(vIndex, pNavimeshTool->m_fNearRange);
				m_vPoint[2] = vIndex;
				m_bPickFirst = false;
				m_bPickSecond = false;
			}
			BuildNavimesh();
		}
		// 1점 찍기
		if(TRUE == pNavimeshTool->m_CreateMode[1].GetCheck())
		{
			if(Engine::Get_NaviMeshSize() == 0)
				return;

			m_vPoint[0] = vIndex;

			if(Engine::Set_OnePoint(m_vPoint[1], m_vPoint[2], m_vPoint[0]))
				BuildNavimesh();
			else
				return;
		}
	}

	else if(TRUE == pNavimeshTool->m_NaviMode[2].GetCheck())
	{
		// 피킹해서 점 이동
		if(TRUE == pNavimeshTool->m_EditMode[0].GetCheck())
		{
			if(Engine::Get_NaviMeshSize() == 0)
				return;
			if(!m_bGetMousePt)
			{
				Engine::Get_NearPoint(vIndex, pNavimeshTool->m_fNearRange);
				m_vGetMousePt = vIndex;
				m_bGetMousePt = true;
			}
			else
			{
				Engine::Change_Point(vIndex, m_vGetMousePt);
				m_vGetMousePt = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_bGetMousePt = false;
			}
		}

		//피킹하고 값넣어서 이동
		else if(TRUE == pNavimeshTool->m_EditMode[1].GetCheck())
		{
			vector<Engine::CNaviCell*> vecCell = Engine::Get_Cell();
			vector<Engine::CNaviCell*>::iterator Navi_iter = vecCell.begin();
			vector<Engine::CNaviCell*>::iterator Navi_iter_end = vecCell.end();
			D3DXVECTOR3 temp;
			for(Navi_iter; Navi_iter != Navi_iter_end; ++Navi_iter)
			{
				if(m_pMouseCol->PickNaviCell(&temp, *Navi_iter, &m_iNaviIndex))
				{
					m_pSelectedCell = (*Navi_iter);
					pNavimeshTool->SetPickCell(m_pSelectedCell);
					break;
				}
				else
					pNavimeshTool->SetPickCell(NULL);
			}


		}
	}
	else if(TRUE == pNavimeshTool->m_NaviMode[3].GetCheck())
	{
		if(TRUE == pNavimeshTool->m_DeleteMode[0].GetCheck())
		{
			// Vtx 삭제
			if(Engine::Get_NaviMeshSize() == 0)
				return;

			if(!m_bGetMousePt)
			{
				Engine::Get_NearPoint(vIndex, pNavimeshTool->m_fNearRange);
				m_vGetMousePt = vIndex;
				m_bGetMousePt = true;
			}
			else
			{
				Engine::Unlink_Cell();

				Engine::Delete_Point(vIndex, m_vGetMousePt);
				m_vGetMousePt = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_bGetMousePt = false;

				Engine::Reset_Index();

				Engine::Link_Cell();
			}
		}
		if(TRUE == pNavimeshTool->m_DeleteMode[1].GetCheck())
		{
			// Tri 삭제
			if(Engine::Get_NaviMeshSize() == 0)
				return;

			if(!m_bGetMousePt)
			{
				//m_pNaviMgr->GetNearPoint(vIndex);
				m_vGetMousePt = vIndex;
				m_bGetMousePt = true;
			}
			else
			{
				Engine::Unlink_Cell();

				Engine::Delete_Triangle(vIndex, m_vGetMousePt);
				m_vGetMousePt = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_bGetMousePt = false;

				Engine::Reset_Index();

				Engine::Link_Cell();
			}
		}
	}
	pNavimeshTool->m_iNaviCnt = Engine::Get_NaviMeshSize();

	Engine::Update_Management(Engine::Get_Time());

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	pNavimeshTool->UpdateData(FALSE);

}

void CBack::ComparePoint(D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2, D3DXVECTOR3& vPoint3)
{
	D3DXVECTOR3		vPoint[3];

	vPoint[0] = ((vPoint1.x < vPoint2.x) ? vPoint1 : vPoint2);
	vPoint[0] = ((vPoint[0].x < vPoint3.x) ? vPoint[0] : vPoint3);

	D3DXVECTOR3		vCompareXPoint[2];
	if(vPoint[0] == vPoint1)
	{
		vCompareXPoint[0] = vPoint2;
		vCompareXPoint[1] = vPoint3;
	}
	else if(vPoint[0] == vPoint2)
	{
		vCompareXPoint[0] = vPoint1;
		vCompareXPoint[1] = vPoint3;
	}
	else
	{
		vCompareXPoint[0] = vPoint1;
		vCompareXPoint[1] = vPoint2;
	}

	vPoint[1] = ((vCompareXPoint[0].z > vCompareXPoint[1].z) ? vCompareXPoint[0] : vCompareXPoint[1]);
	vPoint[2] = ((vCompareXPoint[0].z < vCompareXPoint[1].z) ? vCompareXPoint[0] : vCompareXPoint[1]);




	vPoint1 = vPoint[0];
	vPoint2 = vPoint[1];
	vPoint3 = vPoint[2];
}

void CBack::BuildNavimesh(void)
{
	CNaviMeshTool*	pNavimeshTool	= &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab3;

	if(!m_bPickFirst && !m_bPickSecond)
	{
		ComparePoint(m_vPoint[0], m_vPoint[1], m_vPoint[2]);
		Engine::Unlink_Cell();
		if(pNavimeshTool->m_CellType[0].GetCheck() == TRUE)
			Engine::Add_Cell(&m_vPoint[0], &m_vPoint[1], &m_vPoint[2],Engine::TYPE_TERRAIN);

		else if(pNavimeshTool->m_CellType[1].GetCheck() == TRUE)
			Engine::Add_Cell(&m_vPoint[0], &m_vPoint[1], &m_vPoint[2],Engine::TYPE_CELL);

		else if(pNavimeshTool->m_CellType[2].GetCheck() == TRUE)
			Engine::Add_Cell(&m_vPoint[0], &m_vPoint[1], &m_vPoint[2],Engine::TYPE_MESH);

		pNavimeshTool->m_vPointA = m_vPoint[0];
		pNavimeshTool->m_vPointB = m_vPoint[1];
		pNavimeshTool->m_vPointC = m_vPoint[2];
		Engine::Link_Cell();

		ZeroMemory(m_vPoint, sizeof(D3DXVECTOR3) * 3);
	}
}



bool CBack::Compare(Engine::CGameObject* pDest, Engine::CGameObject* pSource)
{
	const Engine::CComponent* pDestInfo = pDest->GetComponent(L"Transform");
	const Engine::CComponent* pSourInfo = pSource->GetComponent(L"Transform");

	return ((Engine::CTransform*)pDestInfo)->m_vPos.z < ((Engine::CTransform*)pSourInfo)->m_vPos.z;
}

CBack*	CBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBack*	pBack = new CBack(pDevice);
	if(FAILED(pBack->InitScene()))
	{
		Engine::Safe_Release(pBack);
	}

	return pBack;

}
void CBack::Free(void)
{
	Safe_Delete_Array(m_pOccVtx);
	Safe_Delete_Array(m_pOccIdx);
	Engine::CScene::Free();
}

void CBack::ConstOcclusionBox(void)
{
	CMapTool*		pMapTool		= &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	COcclusion*	pOcclusionTool	= &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab5;

	if(NULL == m_pTerrainVtx)
	{
		m_pTerrainVtx = new Engine::VTXTEX[pMapTool->m_iCntX * pMapTool->m_iCntZ];
		m_pMouseCol->SetSize(pMapTool->m_iCntX, pMapTool->m_iCntZ, pMapTool->m_iItv);

		m_pTerrainVtx = m_pTerrain->GetTerrainVtx();
	}

	D3DXVECTOR3 vIndex;
	bool bBoxCol = false;
	unsigned int iIndex = 0;
	for(iIndex; iIndex < Engine::COcclusionMgr::GetInstance()->GetVecOccBoxSize(); ++iIndex)
	{
		DWORD dwTriCnt = 0;
		D3DXMATRIX matWorld = Engine::Get_OccBox(iIndex)->m_matWorld;
		Engine::Get_OccBoxVtx(iIndex)->GetVtxInfo(m_pOccVtx);
		Engine::Get_OccBoxVtx(iIndex)->GetIdxInfo(m_pOccIdx, dwTriCnt);

		bBoxCol = m_pMouseCol->PickOccBox(&vIndex, m_pOccVtx, m_pOccIdx, &matWorld);

		if(bBoxCol)
			break;
	}

	for(unsigned int i = 0; i < Engine::COcclusionMgr::GetInstance()->GetVecOccBoxSize(); ++i)
		Engine::Get_OccBoxVtx(i)->Change_BoxColor(D3DCOLOR_ARGB(255, 0, 255, 0));

	if(bBoxCol)
	{
		Engine::Get_OccBoxVtx(iIndex)->Change_BoxColor(D3DCOLOR_ARGB(255, 255, 0, 0));
		pOcclusionTool->SetInformation(Engine::Get_OccBox(iIndex));
		pOcclusionTool->OccBoxList.SetCurSel(iIndex);
	}
	else
		m_pMouseCol->PickTerrain(&vIndex, m_pTerrainVtx);

	if(TRUE == pOcclusionTool->OccMode[0].GetCheck())
	{
		if(!bBoxCol)
		{
			pOcclusionTool->m_fPosX = vIndex.x;
			pOcclusionTool->m_fPosY = vIndex.y;
			pOcclusionTool->m_fPosZ = vIndex.z;
		}
		pOcclusionTool->UpdateData(FALSE);
	}
	else if(TRUE == pOcclusionTool->OccMode[1].GetCheck())
	{
		if(!bBoxCol)
		{
			pOcclusionTool->m_fPosX = vIndex.x;
			pOcclusionTool->m_fPosY = vIndex.y;
			pOcclusionTool->m_fPosZ = vIndex.z;
		}
		pOcclusionTool->UpdateData(FALSE);
	}
	else if(TRUE == pOcclusionTool->OccMode[2].GetCheck())
	{
		if(!bBoxCol)
		{
			pOcclusionTool->m_fPosX = vIndex.x;
			pOcclusionTool->m_fPosY = vIndex.y;
			pOcclusionTool->m_fPosZ = vIndex.z;
		}
		pOcclusionTool->UpdateData(FALSE);
	}
}