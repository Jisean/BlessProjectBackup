#include "stdafx.h"
#include "PlayerInfo.h"
#include "Include.h"
#include "UI.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer.h"
#include "Export_Function.h"
#include "Texture.h"
#include "KeyMgr.h"
#include "FontMgr.h"


CPlayerInfo::CPlayerInfo(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_bActivate(false)
, m_bPush(false)
, m_bPressed(false)
{

}

CPlayerInfo::~CPlayerInfo(void)
{

}

HRESULT CPlayerInfo::Initialize(void)
{
	m_iCnt = 0;
	FAILED_CHECK(AddComponent());
	LoadData(L"../bin/Resources/Data/UI/PlayerState.dat");

	for(_uint i = 0; i < 8; ++i)
	{
		m_fSaveX[i] = 0.f;
		m_fSaveY[i] = 0.f;
	}
	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		m_vecUI_Info[i].fY += 100.f;
		m_vecUI_Info[i].fX -= 50.f;
		m_vecUI_Info[i].fSizeX *= 0.4f;
		m_vecUI_Info[i].fSizeY *= 0.4f;

		if(i == 2)
		{
			m_vecUI_Info[i].fX += 50.f;
			m_vecUI_Info[i].fY += 50.f;
		}
		if(i == 0)
		{
			m_vecUI_Info[i].fY -= 20.f;
		}
		if(i == 1)
			m_vecUI_Info[i].fY -= 10.f;

	}

	return S_OK;
}

_int CPlayerInfo::Update(const _float& fTimeDelta)
{
	if(true == m_bDead)
		return 0;

	/*if(CKeyMgr::GetInstance()->GetKeyUp('C'))
	{
		if(true == m_bActivate)
			m_bActivate = false;
		else
			m_bActivate = true;
	}*/

	if(false == m_bActivate)
		return 0;

	if(0 >= m_vecUI_Info.size())
		return 0;

	SetUp();
	CollisionUI();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CPlayerInfo::Render(void)
{
	if(true == m_bDead)
		return;

	if(false == m_bActivate)
		return;

	if(0 >= m_vecUI_Info.size())
		return;


	vec_Texture::iterator iter = m_vecTexture.begin();
	vec_Texture::iterator iter_end = m_vecTexture.end();

	for(_uint i = 0; i < m_vecTexture.size(); ++i)
	{
		m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);      //소스의 블랜딩값은 소스의 알파값을 블렌딩한다
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//데스트블렌딩 값은 소스의 알파값의 반대값을 블렌딩한다.
		//m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(0,0,0,0));

		_matrix matOldView, matOldProj;

		m_pDevice->GetTransform(D3DTS_VIEW,&matOldView);
		m_pDevice->GetTransform(D3DTS_PROJECTION,&matOldProj);

		m_pDevice->SetTransform(D3DTS_VIEW, &m_vecUI_Info[i].matView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_vecUI_Info[i].matProj);

		if((*iter) != NULL)
		{
			LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
			if(NULL == pEffect)
				return;

			pEffect->SetTechnique("PlayerInfo");
			pEffect->AddRef();

			pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
			pEffect->SetMatrix("g_matView",&m_vecUI_Info[i].matView);
			pEffect->SetMatrix("g_matProj",&m_vecUI_Info[i].matProj);

			pEffect->SetFloat("g_fAlpha",1.f);

			pEffect->SetFloat("g_fRGB",1.f);
			pEffect->SetFloat("g_fPointPower",1.f);

			(*iter)->Set_ConstantTable(pEffect,"g_BaseTexture");

			pEffect->Begin(NULL,0);
			pEffect->BeginPass(0);

			m_pBuffer->Render(&m_pInfo->m_matWorld);

			pEffect->EndPass();
			pEffect->End();

			Engine::Safe_Release(pEffect);
			++iter;

		}
		m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}



}

HRESULT CPlayerInfo::AddComponent(void)
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
	
	return S_OK;
}

HRESULT CPlayerInfo::SetComponent(WORD wContainerID, const TCHAR* pResourcekey)
{
	Engine::CComponent* pComponent = NULL;
	Engine::CTexture*	pTexture = NULL;

	pComponent = Engine::Clone_Resource(RESOURCE_UI,pResourcekey);
	pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pTexture,E_FAIL);
	m_vecTexture.push_back(pTexture);

	return S_OK;
}

CPlayerInfo* CPlayerInfo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerInfo* pGameObject = new CPlayerInfo(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CPlayerInfo::Free(void)
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

void CPlayerInfo::SetUp()
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

void CPlayerInfo::CollisionUI()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	////////////////////////////////////////////////////////////////////////// 백업
	/*RECT rcUI = {long(m_vecUI_Info[0].fX - m_vecUI_Info[0].fSizeX / 2.f)	
		, long(m_vecUI_Info[0].fY - m_vecUI_Info[0].fSizeY / 2.f) 
		, long(m_vecUI_Info[0].fX + m_vecUI_Info[0].fSizeX  / 2.f) - (LONG)335.f
		, long(m_vecUI_Info[0].fY + m_vecUI_Info[0].fSizeY / 2.f) - (LONG)210.f };*/

	_uint iIndex = 0;
	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		RECT rcUI = {long(m_vecUI_Info[0].fX - m_vecUI_Info[0].fSizeX / 2.f)	
		, long(m_vecUI_Info[0].fY - m_vecUI_Info[0].fSizeY / 2.f) 
		, long(m_vecUI_Info[0].fX + m_vecUI_Info[0].fSizeX  / 2.f) - (LONG)40.f
		, long(m_vecUI_Info[0].fY + m_vecUI_Info[0].fSizeY / 2.f) - (LONG)230.f };

		m_Rect = rcUI;

		if(PtInRect(&m_Rect,ptMouse))
		{
			if(CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
			{
				m_bPush = true;

				for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
				{
					m_fSaveX[i] = m_vecUI_Info[i].fX - (int)ptMouse.x;
					m_fSaveY[i] = m_vecUI_Info[i].fY - (int)ptMouse.y;
				}
			}
		}
	}
	if(CKeyMgr::GetInstance()->GetKeyUp(VK_LBUTTON))
	{
		m_bPressed = true;
		m_bPush = false;
	}
	if(true == m_bPush && false == m_bPressed)
	{
		for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
		{
			m_vecUI_Info[i].fX = ptMouse.x + m_fSaveX[i];
			m_vecUI_Info[i].fY = ptMouse.y + m_fSaveY[i];
		}
	}
	if(false == m_bPush && true == m_bPressed)
	{
		for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
		{
			m_vecUI_Info[i].fX = m_vecUI_Info[i].fX;
			m_vecUI_Info[i].fY = m_vecUI_Info[i].fY;
		}

		m_bPressed = false;
	}
}

void CPlayerInfo::LoadData(const wstring& wstrPath)
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

		if(m_iCnt == 0 || m_iCnt == 1 || m_iCnt == 5)
		{
			SetComponent(pUI_Info->ContainerNumber,pUI_Info->TextureKey);
			m_vecUI_Info.push_back(*pUI_Info);

			Engine::Safe_Delete(pUI_Info);
		}
		else
			Engine::Safe_Delete(pUI_Info);

		++m_iCnt;
	
	}

	CloseHandle(hFile);
}

