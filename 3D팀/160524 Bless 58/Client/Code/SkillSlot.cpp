#include "stdafx.h"
#include "SkillSlot.h"
#include "Include.h"
#include "UI.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer.h"
#include "Export_Function.h"
#include "Texture.h"
#include "KeyMgr.h"
#include "FontMgr.h"
#include "Stage.h"
#include "MainPlayer.h"
#include "Players.h"


CSkillSlot::CSkillSlot(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_bActivate(false)
, m_pMainPlayer(NULL)
, m_MoveX(0.f)
, m_MoveY(0.f)
, m_MoveSizeX(0.f)
, m_MoveSizeY(0.f)
{
	for(UINT i = 0; i < SK_END; ++i)
	{
		m_vecSkill_Img[i].clear();
		m_vecSkill_ImgInfo[i].clear();
	}
}

CSkillSlot::~CSkillSlot(void)
{

}

HRESULT CSkillSlot::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	//140.f
	//315.f
	for (_uint i = 0; i < 4; ++i)
	{
		m_vecUI_Info[i].fX += ((WINCX / 2.f) + (60.f * (((float)i + 1.f))) - ((WINCX * 0.1f) + 200.f));
		m_vecUI_Info[i].fY += ((WINCY)) - 50.f;
	
		m_vecUI_Info[i].fSizeX = 23.f;
		m_vecUI_Info[i].fSizeY = 23.f;

		m_vecUI_Info[i].fStateIndex = 33.f;
	}

	for (_uint i = 4; i < 8; ++i)
	{
		m_vecUI_Info[i].fX = ((WINCX / 2.f) + (60.f * (((float)i + 1.f))) - ((WINCX * 0.1f)));
		m_vecUI_Info[i].fY += ((WINCY)) - 50.f;

		m_vecUI_Info[i].fSizeX = 23.f;
		m_vecUI_Info[i].fSizeY = 23.f;

		m_vecUI_Info[i].fStateIndex = 33.f;

	}

	m_bStart = false;

	for(_uint i = 0; i < 4; ++i)
	{
		vector<UI_INFO>::iterator iter_SK_Info = m_vecSkill_ImgInfo[i].begin();
		vector<UI_INFO>::iterator iter_SK_Info_end = m_vecSkill_ImgInfo[i].end();
		
		for(; iter_SK_Info != iter_SK_Info_end; ++iter_SK_Info)
		{
			(*iter_SK_Info).fSizeX -= 100.f;
			(*iter_SK_Info).fSizeY -= 100.f;
		}
	}
	
	return S_OK;
}

_int CSkillSlot::Update(const _float& fTimeDelta)
{
	if(g_bMainQuest)
		return 0;

	if(true == m_bDead)
		return 0;

	/*if(g_bMainQuest)
		return 0;*/

	if(0 >= m_vecUI_Info.size())
		return 0;

	if(m_bStart == false)
	{
		m_pMainPlayer = ((CMainPlayer*)Engine::Get_ObjList(LAYER_GAMELOGIC,L"Player")->front());
		m_bStart = true;
	}

	//Move_Tex();
	SetUp();
	CollisionUI();

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	return Engine::CGameObject::Update(fTimeDelta);
}

void CSkillSlot::Render(void)
{
	if(true == m_bDead)
		return;

	if(0 >= m_vecUI_Info.size())
		return;

	if(NULL == m_pMainPlayer)
		return;

	int iType = m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob();

	vec_Texture::iterator iter = m_vecTexture.begin();
	vec_Texture::iterator iter_end = m_vecTexture.end();

	vector<UI_INFO>::iterator iter_SKImg_Info = m_vecSkill_ImgInfo[iType].begin();
	
	for(_uint i = 0; i < m_vecTexture.size(); ++i)
	{
		m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);      //소스의 블랜딩값은 소스의 알파값을 블렌딩한다
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//데스트블렌딩 값은 소스의 알파값의 반대값을 블렌딩한다.

		_matrix matOldView, matOldProj;
		_matrix matOldView_SK,matOldeProj_SK;

		m_pDevice->GetTransform(D3DTS_VIEW,&matOldView);
		m_pDevice->GetTransform(D3DTS_PROJECTION,&matOldProj);

		m_pDevice->GetTransform(D3DTS_VIEW,&matOldView_SK);
		m_pDevice->GetTransform(D3DTS_PROJECTION,&matOldeProj_SK);

		m_pDevice->SetTransform(D3DTS_VIEW, &m_vecUI_Info[i].matView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_vecUI_Info[i].matProj);

		m_pDevice->SetTransform(D3DTS_VIEW, &(*iter_SKImg_Info).matView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &(*iter_SKImg_Info).matProj);


		if((*iter) != NULL)
		{
			LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
			if(NULL == pEffect)
				return;

			pEffect->SetTechnique("Skill");
			pEffect->AddRef();

			pEffect->SetMatrix("g_matWorld",&m_pInfo->m_matWorld);
			pEffect->SetMatrix("g_matView",&m_vecUI_Info[i].matView);
			pEffect->SetMatrix("g_matProj",&m_vecUI_Info[i].matProj);
			pEffect->SetFloat("g_fAlpha", 1.f);
			pEffect->SetFloat("g_fRGB", 1.f);
			pEffect->SetFloat("g_fPointPower", 1.f);


			if(m_pMainPlayer->Get_TargetPlayer()->Get_TargetMonster() == NULL)
			{
				if(m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob() == CPlayers::JOB_GUARDIAN)
				{
					if(i == 0 || i == 5 || i == 6 || i == 7)
						pEffect->SetFloat("g_fColor", SkillCoolTime(i));
					else
						pEffect->SetFloat("g_fColor", 1.f);
					
				}
				if(m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob() == CPlayers::JOB_PALADIN)
				{
					if(i == 0 || i == 1 || i == 2 || i == 3 || i == 5 || i == 6 || i == 7)
						pEffect->SetFloat("g_fColor", SkillCoolTime(i));
					else
						pEffect->SetFloat("g_fColor", 1.f);
				}
				if(m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob() == CPlayers::JOB_MAGICIAN)
				{
					if(i == 5 || i == 6)
						pEffect->SetFloat("g_fColor", SkillCoolTime(i));
					else
						pEffect->SetFloat("g_fColor", 1.f);
				}
				if(m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob() == CPlayers::JOB_RANGER)
				{
					if(i == 5 || i == 6)
						pEffect->SetFloat("g_fColor", SkillCoolTime(i));
					else
						pEffect->SetFloat("g_fColor", 1.f);
				}

			///	pEffect->SetFloat("g_fColor", 1.f);
			}
			else
			{
				pEffect->SetFloat("g_fColor", SkillCoolTime(i));
			}

			(*iter)->Set_ConstantTable(pEffect,"g_BaseTexture");
			(*(Engine::CTexture*)(m_vecSkill_Img[iType])[i]).Set_ConstantTable(pEffect,"g_BaseTexture");
			m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture");
			
			UINT iIdx = 0;
			pEffect->Begin(&iIdx,0);

			for(_uint i = 0; i < iIdx; ++i)
			{
				pEffect->BeginPass(i);

				m_pBuffer->Render(&m_pInfo->m_matWorld);

				pEffect->EndPass();
			}
			
			pEffect->End();

			Engine::Safe_Release(pEffect);
			++iter;
			++iter_SKImg_Info;
		}

		m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

		m_pDevice->SetTransform(D3DTS_VIEW, &matOldView_SK);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldeProj_SK);

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}


}

HRESULT CSkillSlot::AddComponent(void)
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

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_UI, L"Texture_SkillFilter");
	m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pFilterTexture, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	for(int i = 0; i < 8; ++i)
	{
		{
			UI_INFO* pUI_Info = new UI_INFO;
			ZeroMemory(pUI_Info,sizeof(UI_INFO));

			Engine::CComponent* pComponent = NULL;
			Engine::CTexture*	pTexture = NULL;

			wsprintf(pUI_Info->TextureKey,L"slotsprite",i);

			pComponent = Engine::Clone_Resource(RESOURCE_UI,L"slotsprite");
			pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(pTexture,E_FAIL);
			m_vecTexture.push_back(pTexture);
			m_vecUI_Info.push_back(*pUI_Info);
			Engine::Safe_Delete(pUI_Info);
		}
		////////////////////////////////////////////////////////////////////////// 가디언
		{
			UI_INFO* pUI_SKill_Info = new UI_INFO;
			ZeroMemory(pUI_SKill_Info,sizeof(UI_INFO));

			Engine::CComponent* pComponent = NULL;
			Engine::CTexture*	pTexture = NULL;

			wsprintf(pUI_SKill_Info->TextureKey,L"Guardian_Skill_%d",i);
			pComponent = Engine::Clone_Resource(RESOURCE_UI,pUI_SKill_Info->TextureKey);
			pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(pTexture,E_FAIL);
			m_vecSkill_Img[SK_GUARDIAN].push_back(pTexture);
			m_vecSkill_ImgInfo[SK_GUARDIAN].push_back(*pUI_SKill_Info);
			Engine::Safe_Delete(pUI_SKill_Info);
		}
		
		//////////////////////////////////////////////////////////////////////////// 팔라딘
		{
			UI_INFO* pUI_SKill_Info = new UI_INFO;
			ZeroMemory(pUI_SKill_Info,sizeof(UI_INFO));

			Engine::CComponent* pComponent = NULL;
			Engine::CTexture*	pTexture = NULL;

			wsprintf(pUI_SKill_Info->TextureKey,L"Paladin_Skill_%d",i);
			pComponent = Engine::Clone_Resource(RESOURCE_UI,pUI_SKill_Info->TextureKey);
			pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(pTexture,E_FAIL);
			m_vecSkill_Img[SK_PALADIN].push_back(pTexture);
			m_vecSkill_ImgInfo[SK_PALADIN].push_back(*pUI_SKill_Info);
			Engine::Safe_Delete(pUI_SKill_Info);
		}
		////////////////////////////////////////////////////////////////////////// 레인저
		{
			UI_INFO* pUI_SKill_Info = new UI_INFO;
			ZeroMemory(pUI_SKill_Info,sizeof(UI_INFO));

			Engine::CComponent* pComponent = NULL;
			Engine::CTexture*	pTexture = NULL;

			wsprintf(pUI_SKill_Info->TextureKey,L"Ranger_Skill_%d",i);
			pComponent = Engine::Clone_Resource(RESOURCE_UI,pUI_SKill_Info->TextureKey);
			pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(pTexture,E_FAIL);
			m_vecSkill_Img[SK_RANGER].push_back(pTexture);
			m_vecSkill_ImgInfo[SK_RANGER].push_back(*pUI_SKill_Info);
			Engine::Safe_Delete(pUI_SKill_Info);
		}
		////////////////////////////////////////////////////////////////////////// 매지션
		{
			UI_INFO* pUI_SKill_Info = new UI_INFO;
			ZeroMemory(pUI_SKill_Info,sizeof(UI_INFO));

			Engine::CComponent* pComponent = NULL;
			Engine::CTexture*	pTexture = NULL;

			wsprintf(pUI_SKill_Info->TextureKey,L"Magician_Skill_%d",i);
			pComponent = Engine::Clone_Resource(RESOURCE_UI,pUI_SKill_Info->TextureKey);
			pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(pTexture,E_FAIL);
			m_vecSkill_Img[SK_MAGICIAN].push_back(pTexture);
			m_vecSkill_ImgInfo[SK_MAGICIAN].push_back(*pUI_SKill_Info);
			Engine::Safe_Delete(pUI_SKill_Info);
		}
	}
	return S_OK;
}
float CSkillSlot::SkillCoolTime(int iNum)
{
	float fMaxTime = m_pMainPlayer->Get_TargetPlayer()->Get_CoolTime_Max(iNum);
	float fCurTime = m_pMainPlayer->Get_TargetPlayer()->Get_CoolTime(iNum);

	return (fCurTime / fMaxTime);
}

HRESULT CSkillSlot::SetComponent(WORD wContainerID, const TCHAR* pResourcekey)
{
	Engine::CComponent* pComponent = NULL;
	Engine::CTexture*	pTexture = NULL;

	pComponent = Engine::Clone_Resource(RESOURCE_UI,pResourcekey);
	pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pTexture,E_FAIL);
	m_vecTexture.push_back(pTexture);

	return S_OK;
}

CSkillSlot* CSkillSlot::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkillSlot* pGameObject = new CSkillSlot(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CSkillSlot::Free(void)
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

	for(_uint i = 0; i < SK_END; ++i)
	{
		vector<Engine::CTexture*>::iterator iterSkillImg = m_vecSkill_Img[i].begin();
		vector<Engine::CTexture*>::iterator iterSkillImg_end = m_vecSkill_Img[i].end();

		for(iterSkillImg; iterSkillImg != iterSkillImg_end; ++iterSkillImg)
		{
			Engine::Safe_Release(*iterSkillImg);
		}
		m_vecSkill_Img[i].clear();
	}

	CUI::Free();
}

void CSkillSlot::SetUp()
{
	vector<UI_INFO>::iterator iter = m_vecSkill_ImgInfo[m_pMainPlayer->Get_TargetPlayer()->Get_PlayerJob()].begin();

	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		D3DXMatrixOrthoLH(&m_vecUI_Info[i].matProj, WINCX, WINCY, 0.f, 1.f);
		D3DXMatrixIdentity(&m_vecUI_Info[i].matView);
		D3DXMatrixOrthoLH(&(*iter).matProj,WINCX,WINCY,0.f,1.f);
		D3DXMatrixIdentity(&(*iter).matView);

		m_vecUI_Info[i].matView._11 = m_vecUI_Info[i].fSizeX;
		m_vecUI_Info[i].matView._22 = m_vecUI_Info[i].fSizeY;
		m_vecUI_Info[i].matView._33 = 1.f;

		(*iter).matView._11 = (*iter).fSizeX;
		(*iter).matView._22 = (*iter).fSizeY;
		(*iter).matView._33 = 1.f;


		// -0.5	-> -400		,	0.5	-> 400
		m_vecUI_Info[i].matView._41 = m_vecUI_Info[i].fX - (WINCX >> 1);
		m_vecUI_Info[i].matView._42 = -m_vecUI_Info[i].fY + (WINCY >> 1);

		(*iter).matView._41 = (*iter).fX - (WINCX >> 1);
		(*iter).matView._42 = (*iter).fY + (WINCX >> 1);

		++iter;
	}

}

void CSkillSlot::CollisionUI()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	////////////////////////////////////////////////////////////////////////// 백업

	_uint iIndex = 0;
	for(_uint i = 0; i < m_vecUI_Info.size(); ++i)
	{
		RECT rcUI = {long(m_vecUI_Info[i].fX - m_vecUI_Info[i].fSizeX * 0.5f)
			, long(m_vecUI_Info[i].fY  - m_vecUI_Info[i].fSizeY  * 0.5f)
			, long(m_vecUI_Info[i].fX  + m_vecUI_Info[i].fSizeX  * 0.5f)
			, long(m_vecUI_Info[i].fY  + m_vecUI_Info[i].fSizeY  * 0.5f)};

		m_Rect = rcUI;

		if(PtInRect(&m_Rect,ptMouse))
		{
			if(false == m_bActivate)
				m_bActivate = true;
		}
		else
			m_bActivate = false;

		if(true == m_bActivate)
		{
			iIndex = i;
			m_vecUI_Info[i].bCheck = true;
		}
		else
		{
			iIndex = i;
			m_vecUI_Info[i].bCheck = false;
		}
	}
	
	if(true == m_vecUI_Info[iIndex].bCheck)
		m_vecUI_Info[iIndex].fStateIndex = 34.f;
	else
		m_vecUI_Info[iIndex].fStateIndex = 33.f;
	
}

void CSkillSlot::LoadData(const wstring& wstrPath)
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

		SetComponent(pUI_Info->ContainerNumber,pUI_Info->TextureKey);
		m_vecUI_Info.push_back(*pUI_Info);

		Engine::Safe_Delete(pUI_Info);

		m_vecUI_Info;
	}

	CloseHandle(hFile);
}

void CSkillSlot::Move_Tex(void)
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

