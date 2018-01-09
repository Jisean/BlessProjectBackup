#include "stdafx.h"
#include "PlayerBar.h"
#include "Include.h"
#include "UI.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer.h"
#include "Export_Function.h"
#include "Texture.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "Status.h"
#include "Players.h"
#include "Stage.h"

CPlayerBar::CPlayerBar(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_bClick(false)
, m_bPressed(false)
, m_fTimeDelta(0.f)
, m_fWaveTime(0.f)
, m_fTime(0.f)
, m_MoveX(0.f)
, m_MoveY(0.f)
, m_MoveSizeX(0.f)
, m_MoveSizeY(0.f)

{
	ZeroMemory(m_fSaveX, sizeof(float) * 5);
	ZeroMemory(m_fSaveY, sizeof(float) * 5);
}

CPlayerBar::~CPlayerBar(void)
{

}

HRESULT CPlayerBar::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	LoadData(L"../bin/Resources/Data/UI/PlayerInfoNew.dat");

	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		m_vecUI_Info[i].fX += ((WINCX / 2.f)) - 400.f;
		m_vecUI_Info[i].fY += ((WINCY/ 2.f)) - 100.f;
		m_vecUI_Info[i].fSizeX *= 0.5f;
		m_vecUI_Info[i].fSizeY *= 0.5f;

		if(i == 5)
		{
			m_fOriHpSize = m_vecUI_Info[i].fSizeX;
			m_fOriHpfX = m_vecUI_Info[i].fX;
		}
		else if(i == 6)
		{
			m_fOriMpSize = m_vecUI_Info[i].fSizeX;
			m_fOriMpfX = m_vecUI_Info[i].fX;
		}


		//__debugbreak();
	}

	return S_OK;
}

_int CPlayerBar::Update(const _float& fTimeDelta)
{
	if(g_bMainQuest)
		return 0;

	if(true == m_bDead)
		return 0;

	if(0 >= m_vecUI_Info.size())
		return 0;

	m_fTimeDelta += fTimeDelta ;
	m_fTime = fTimeDelta;

	if(m_fTimeDelta >= 1.f)
	{
		m_fWaveTime = -0.5f;
	}
	if(m_fTimeDelta >= 2.f)
	{
		m_fWaveTime = 0.5f;
		m_fTimeDelta = 0.f;
	}

	/*if(GetAsyncKeyState(VK_LEFT))
	m_fCnt[0] -= 10.f * Engine::Get_TimeDelta(L"TimeDelta:60");

	if(GetAsyncKeyState(VK_RIGHT))
	m_fCnt[0] += 10.f * Engine::Get_TimeDelta(L"TimeDelta:60");

	if(GetAsyncKeyState(VK_UP))
	m_fCnt[1] -= 10.f * Engine::Get_TimeDelta(L"TimeDelta:60");

	if(GetAsyncKeyState(VK_DOWN))
	m_fCnt[1] += 10.f * Engine::Get_TimeDelta(L"TimeDelta:60");

	*/

	//Move_Tex();
	PlayerBar();
	SetUp();
	CollisionUI();
	//m_pInfo->m_vPos = _vec3(1.f,0.f,0.f);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CPlayerBar::Render(void)
{
	if(true == m_bDead)
		return;

	if(0 >= m_vecUI_Info.size())
		return;

	//Rectangle(GetDC(g_hWnd),m_Rect.left,m_Rect.top,m_Rect.right,m_Rect.bottom);

	vec_Texture::iterator iter = m_vecTexture.begin();
	vec_Texture::iterator iter_end = m_vecTexture.end();

	for(_uint i = 0; i < m_vecTexture.size(); ++i)
	{
		if(i == 2)
			continue;

		m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
		_matrix matOldView, matOldProj;

		m_pDevice->GetTransform(D3DTS_VIEW,&matOldView);
		m_pDevice->GetTransform(D3DTS_PROJECTION,&matOldProj);

		if((*iter) != NULL)
		{
			LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
			if(NULL == pEffect)
				return;

			pEffect->SetTechnique("Default_Device");
			pEffect->AddRef();

			pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
			pEffect->SetMatrix("g_matView",&m_vecUI_Info[i].matView);
			pEffect->SetMatrix("g_matProj",&m_vecUI_Info[i].matProj);

			/*if(i == 5 || i == 6)
			{
			pEffect->SetFloat("g_fWaveTime",m_fWaveTime);
			pEffect->SetFloat("g_fTime", m_fTimeDelta);
			}
			else
			{
			pEffect->SetFloat("g_fWaveTime",0.f);
			pEffect->SetFloat("g_fTime",0.f);
			}
			if(i == 0)
			{
			pEffect->SetFloat("g_fRGB",m_fTimeDelta);
			pEffect->SetFloat("g_fPointPower",2.f);
			}
			else
			{*/
			if(true == m_bClick)
				pEffect->SetFloat("g_fAlpha",0.5f);
			else
				pEffect->SetFloat("g_fAlpha",1.f);

			pEffect->SetFloat("g_fRGB",1.f);
			pEffect->SetFloat("g_fPointPower",1.f);

			//}


			//(*iter)->Set_ConstantTable(pEffect,"g_BaseTexture");


			m_vecTexture[i]->Set_ConstantTable(pEffect,"g_BaseTexture");

			pEffect->Begin(NULL,0);
			pEffect->BeginPass(0);

			m_pBuffer->Render(&m_pInfo->m_matWorld);

			pEffect->EndPass();
			pEffect->End();

			Engine::Safe_Release(pEffect);
			//++iter;
		}

		m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	}



}

HRESULT CPlayerBar::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform",pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer",pComponent);

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader",pComponent);


	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	//for(_uint i = 0; i < m_htComponent.Get_TableSize(); ++i)
	//{
	//	void*	pComponent = m_htComponent.Search_TableData(i);
	//	if(pComponent != NULL)
	//	{
	//		//((Engine::CComponent*)pComponent).
	//	}
	//}
	return S_OK;
}

HRESULT CPlayerBar::SetComponent(WORD wContainerID, const TCHAR* pResourcekey)
{
	Engine::CComponent* pComponent = NULL;
	Engine::CTexture*	pTexture = NULL;

	pComponent = Engine::Clone_Resource(RESOURCE_UI,pResourcekey);
	pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pTexture,E_FAIL);
	m_vecTexture.push_back(pTexture);

	return S_OK;
}

CPlayerBar* CPlayerBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerBar* pGameObject = new CPlayerBar(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CPlayerBar::Free(void)
{
	vec_UI::iterator iter_Info = m_vecUI_Info.begin();

	vec_Texture::iterator iter = m_vecTexture.begin();
	vec_Texture::iterator iter_end = m_vecTexture.end();

	while(true)
	{
		if(m_vecTexture.size() == 0)
			break;

		//m_vecUI_Info.erase(iter_Info);
		Engine::Safe_Release(*iter);
		m_vecTexture.erase(iter);

		if(m_vecTexture.size() != 0)
		{
			iter = m_vecTexture.begin();

			if(iter == m_vecTexture.end() - 1)
				iter = m_vecTexture.begin();
		}

	}
	m_vecUI_Info.clear();
	//m_vecTexture.clear();

	CUI::Free();
}

void CPlayerBar::PlayerBar()
{

	Engine::CStatus* pStatus = const_cast<Engine::CStatus*>(((CPlayers*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front())->GetStatus());


	float fHp = pStatus->m_tRealStat.m_fCurHp / pStatus->m_tRealStat.m_fMaxHp;
	float fMp = pStatus->m_tRealStat.m_fCurMana / pStatus->m_tRealStat.m_fMaxMana;

	// Hp 
	m_vecUI_Info[5].fSizeX = m_fOriHpSize * fHp;
	m_vecUI_Info[5].fX = m_fOriHpfX + (m_fOriHpSize * (1.f - fHp));

	// Mp
	m_vecUI_Info[6].fSizeX = m_fOriMpSize * fMp;
	m_vecUI_Info[6].fX = m_fOriMpfX - (m_fOriMpSize * (1.f - fMp));
}

void CPlayerBar::SetUp()
{
	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		D3DXMatrixOrthoLH(&m_vecUI_Info[i].matProj, WINCX, WINCY, 0.f, 1.f);

		D3DXMatrixIdentity(&m_vecUI_Info[i].matView);
		m_vecUI_Info[i].matView._11 = m_vecUI_Info[i].fSizeX;
		m_vecUI_Info[i].matView._22 = m_vecUI_Info[i].fSizeY;
		m_vecUI_Info[i].matView._33 = 1.f;

		// -0.5	-> -400		,	0.5	-> 400
		m_vecUI_Info[i].matView._41 = m_vecUI_Info[i].fX - (WINCX >> 1);
		m_vecUI_Info[i].matView._42 = -m_vecUI_Info[i].fY + (WINCY >> 1);
	}

}

void CPlayerBar::CollisionUI()
{

}

void CPlayerBar::LoadData(const wstring& wstrPath)
{
	HANDLE	hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;



	while(true)
	{
		UI_INFO* pUI_Info = new UI_INFO;
		ZeroMemory(pUI_Info,sizeof(UI_INFO));

		ReadFile(hFile,&pUI_Info->TextureKey,sizeof(pUI_Info->TextureKey),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->TextureName,sizeof(pUI_Info->TextureKey),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->bCheck,sizeof(bool),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->bPressed,sizeof(bool),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->bPush,sizeof(bool),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->ContainerNumber,sizeof(WORD),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->fSizeX,sizeof(float),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->fSizeY,sizeof(float),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->fX,sizeof(float),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->fY,sizeof(float),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->matProj,sizeof(D3DXMATRIX),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->matView,sizeof(D3DXMATRIX),&dwByte,NULL);
		ReadFile(hFile,&pUI_Info->wSortNumber,sizeof(WORD),&dwByte,NULL);

		if(dwByte == 0)	 
		{				 
			Engine::Safe_Delete(pUI_Info);
			break;
		}

		if(m_iCnt > 6)
		{
			SetComponent(pUI_Info->ContainerNumber,pUI_Info->TextureKey);
			m_vecUI_Info.push_back(*pUI_Info);

			Engine::Safe_Delete(pUI_Info);
		}
		else
		{
			Engine::Safe_Delete(pUI_Info);
			++m_iCnt;
		}

		m_vecUI_Info;

	}

	CloseHandle(hFile);
}

void CPlayerBar::Move_Tex(void)
{
	float fTemp2 = 1.f;
	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)
		{
			m_vecUI_Info[i].fX -= fTemp2;
			m_MoveX -= fTemp2;
		}

		if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)
		{
			m_vecUI_Info[i].fX += fTemp2;
			m_MoveX += fTemp2;
		}

		if(Engine::GetDIKeyState(DIK_UP) & 0x80)
		{
			m_vecUI_Info[i].fY += fTemp2;
			m_MoveY += fTemp2;
		}

		if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)
		{
			m_vecUI_Info[i].fY -= fTemp2;
			m_MoveY -= fTemp2;
		}

		//cout << " X :" << m_vecUI_Info[i].fX << " / " << " Y : " << m_vecUI_Info[i].fY << endl;

	}



	//cout << " X :" << m_MoveX << " / " << " Y : " << m_MoveY << endl;

}

