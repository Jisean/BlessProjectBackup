#include "Mesh_Effect.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Transform.h"
#include "StaticMesh.h"
#include "Shader.h"
#include <tchar.h>

Engine::CMesh_Effect::CMesh_Effect(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pShader(NULL)
, m_iComMeshCount(0)
, m_iComTexCount(0)
, m_iComMaskTexCount(0)
, m_iComNormalTexCount(0)
, m_fTimeDelta(0.f)
, m_fUnLimitedTime(0.f)
, m_vStartPos(0.f, 0.f, 0.f)
, m_eState(STATE_IDLE)
, m_bCalling(false)
, m_vTargetPos(NULL)
, m_fGetTime(0)
, m_iNumber(0)
, m_vScale(-1.f, -1.f, -1.f)
, m_bShield(false)
{
	m_htComponent.Ready_Table(661);
}

Engine::CMesh_Effect::~CMesh_Effect(void)
{

}

HRESULT Engine::CMesh_Effect::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_eState = STATE_READY;

	m_pAngle[ANGLE_X] = NULL;
	m_pAngle[ANGLE_Y] = NULL;
	m_pAngle[ANGLE_Z] = NULL;


	return S_OK;
}


int Engine::CMesh_Effect::Update(const _float& fTimeDelta)
{
	_int iResult = Engine::CGameObject::Update(fTimeDelta);

	Compute_ViewZ(&m_vStartPos);


	if(m_eState == STATE_START)
	{	

		if(m_bShield == true)
		{
			bool bShieldCheck = false;
			for(int i = 0; i < 54; ++i)
			{
				if(m_vecMeshInfoList[i]->bShield == false)
					bShieldCheck = true;

			}

			if(bShieldCheck == false)
			{
				EffectReset();
				m_eState = STATE_READY;
			}
		}

		m_fGetTime += fTimeDelta;
		for(unsigned int i = 0; i < m_vecMeshInfoList.size(); ++i)
		{
			m_vecMeshInfoList[i]->fGetTime += fTimeDelta;
			for(int iAct = 0; iAct < ACT_END; ++iAct)
			{
				if(m_vecMeshInfoList[i]->bUnLimited[iAct] == true)
					m_vecMeshInfoList[i]->fUnLimitedTime[iAct] += fTimeDelta;
			}

			if(m_vecMeshInfoList[i]->fGetTime > m_vecMeshInfoList[i]->fAliveEnd_Time)
			{
				m_vecMeshInfoList[i]->bAliveOn = false;

				--m_vecMeshInfoList[i]->iCycle;

				if(m_vecMeshInfoList[i]->iCycle <= 0)
					m_vecMeshInfoList[i]->bEnd = true;
				else
					SingleReset(i);
			}
			else if(m_vecMeshInfoList[i]->fGetTime > m_vecMeshInfoList[i]->fAliveStart_Time)
			{
				m_vecMeshInfoList[i]->bAliveOn = true;				

			}
			else if(m_vecMeshInfoList[i]->fGetTime < m_vecMeshInfoList[i]->fAliveStart_Time)
			{
				m_vecMeshInfoList[i]->bAliveOn = false;
			}

			if(m_vecMeshInfoList[i]->bAliveOn == true)
				ActionUpdate(i, fTimeDelta, m_vecMeshInfoList[i]->fGetTime);
		}

		bool bAliveCheck = true;

		for(unsigned int i = 0; i < m_vecMeshInfoList.size(); ++i)
		{
			if(m_vecMeshInfoList[i]->bEnd == false)
				bAliveCheck = false;
		}		

		if(bAliveCheck == true)
		{	
			EffectReset();
			m_eState = STATE_READY;
		}

		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_ALPHA, this);

	}

	if(m_eState == STATE_READY)
	{
		m_bCalling = false;

	}
	if(m_eState == STATE_IDLE)
	{
		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_ALPHA, this);
		for(unsigned int i = 0; i < m_vecMeshInfoList.size(); ++i)
		{			
			m_vecMeshInfoList[i]->bAliveOn = true;			
		}	
	}

	return iResult;
}

void Engine::CMesh_Effect::Render(void)
{	
	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	int iCount = 0;	

	for(unsigned int i = 0; i < m_vecMeshList.size(); ++i)
	{
		if( m_vecMeshInfoList[iCount]->bAliveOn == true)
		{
			const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
			if(NULL == pLightInfo)
				return;

			pEffect->SetVector("g_vLightDir", &_vec4(D3DXVECTOR3(1.f, -1.f, 1.f), 0.f));

			pEffect->SetMatrix("g_matWorld", &m_vecMeshInfoList[iCount]->matWorld);	

			D3DXVECTOR3 vColor = m_vecMeshInfoList[iCount]->vRGB;
			vColor /= 255.f;

			if(m_vecMeshInfoList[iCount]->bColorOn == true)
				pEffect->SetVector("g_vSetColor", &_vec4(vColor, 1.f));
			else
				pEffect->SetVector("g_vSetColor", &_vec4(vColor, 0.f));		

			_mymat			matView, matProj;
			m_pDevice->GetTransform(D3DTS_VIEW, &matView);
			m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

			pEffect->SetMatrix("g_matView", &matView);
			pEffect->SetMatrix("g_matProj", &matProj);				

			_matrix matCamWorld;
			D3DXMatrixInverse(&matCamWorld, NULL, &matView);
			pEffect->SetVector("g_vCamPosition", (_vec4*)&matCamWorld.m[3][0]);			

			pEffect->SetInt("g_iPass", m_vecMeshInfoList[iCount]->iPass);

			pEffect->SetFloat("g_fU", m_vecMeshInfoList[iCount]->fUPos);
			pEffect->SetFloat("g_fV",  m_vecMeshInfoList[iCount]->fVPos);		

			if(m_vecMeshInfoList[iCount]->bHoldOn == true)
			{
				pEffect->SetFloat("g_fMaskU", 0.f);
				pEffect->SetFloat("g_fMaskV", 0.f);			
			}
			else
			{
				pEffect->SetFloat("g_fMaskU",  m_vecMeshInfoList[iCount]->fUPos);
				pEffect->SetFloat("g_fMaskV",   m_vecMeshInfoList[iCount]->fVPos);
			}

			pEffect->SetFloat("g_fUSize", m_vecMeshInfoList[iCount]->fUSize);
			pEffect->SetFloat("g_fVSize", m_vecMeshInfoList[iCount]->fVSize);

			pEffect->SetFloat("g_fFade", m_vecMeshInfoList[iCount]->fFade);	


			if(m_vecMeshInfoList[iCount]->iSpriteX != 0 )
			{
				float iSpriteCountX = float(m_vecMeshInfoList[iCount]->iSpriteCount % m_vecMeshInfoList[iCount]->iSpriteX);
				float iSpriteCountY = float(m_vecMeshInfoList[iCount]->iSpriteCount / m_vecMeshInfoList[iCount]->iSpriteX);
		
				pEffect->SetVector("g_vUVSprite", &_vec4((float)m_vecMeshInfoList[iCount]->iSpriteX, (float)m_vecMeshInfoList[iCount]->iSpriteY, iSpriteCountX, iSpriteCountY));
			}
			else
				pEffect->SetVector("g_vUVSprite", &_vec4(0.f, 0.f, 0.f, 0.f));			

			m_vecMeshList[i]->RenderMesh_ForShader(m_vecTexList[iCount], m_vecMaskTexList[iCount], m_vecNormalTexList[iCount], pEffect, &m_vecMeshInfoList[iCount]->matWorld, 0, m_vecMeshInfoList[iCount]->iPass);
		}
		++iCount;
	}	
	
	Engine::Safe_Release(pEffect);
}

HRESULT Engine::CMesh_Effect::AddComponent(void)
{
	CComponent*		pComponent = NULL;	

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_MeshEffect");
	if(NULL == pComponent)
		return E_FAIL;

	m_htComponent.Insert_Table(L"Com_Shader", pComponent);	

	return S_OK;
}


Engine::CMesh_Effect* Engine::CMesh_Effect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMesh_Effect*		pGameObject = new CMesh_Effect(pDevice);		

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void Engine::CMesh_Effect::Free(void)
{
	Engine::CGameObject::Free();

	/*int iCount = 0;
	for(VECTEXLIST::iterator iter = m_vecTexList.begin(); iter != m_vecTexList.end(); ++iter)
	{
		for(int i = 0; i < m_vecMeshInfoList[iCount]->iTextureCount; ++i)
		{
			Safe_Release((*iter)[i]);
		}

		Safe_Delete_Array((*iter));

		++iCount;
	}*/

	//m_vecTexList.clear();

	for(VECMESHINFOLIST::iterator iter = m_vecMeshInfoList.begin(); iter != m_vecMeshInfoList.end(); ++iter)
	{
		Safe_Delete(*iter);
	}
	m_vecMeshInfoList.clear();

	for(vector<const TCHAR*>::iterator iter = m_vecComKey.begin(); iter != m_vecComKey.end(); ++iter)
	{
		Safe_Delete(*iter);
	}

	m_vecMeshList.clear();
	m_vecTexList.clear();
	m_vecMaskTexList.clear();
	m_vecNormalTexList.clear();
}

HRESULT Engine::CMesh_Effect::Add_Data(const TCHAR* szMeshKey, const TCHAR* szTextureKey, const TCHAR* szMaskKey, const TCHAR* szNormalKey, MESHEFFECTINFO* pEffectInfo)
{	
	CComponent*		pComponent = NULL;

	//StaticMesh
	CStaticMesh*	pMesh = NULL;
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, szMeshKey);
	pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN_MSG(pMesh, E_FAIL, L"Mesh Clone Fail");

	m_vecMeshList.push_back(pMesh);

	TCHAR*	szName = new TCHAR[128];	

	wsprintf(szName, _T("Mesh_%d"), m_iComMeshCount);

	m_vecComKey.push_back(szName);

	m_htComponent.Insert_Table(szName, pComponent);	

	++m_iComMeshCount;

	Engine::CTexture*	pTexture = NULL;
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, szTextureKey);
	pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_vecTexList.push_back(pTexture);

	TCHAR*	szTexName = new TCHAR[128];		

	wsprintf(szTexName, _T("Texture_%d"), m_iComTexCount);

	m_vecComKey.push_back(szTexName);

	m_htComponent.Insert_Table(szTexName, pComponent);

	++m_iComTexCount;

	Engine::CTexture*	pMaskTexture = NULL;
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, szMaskKey);
	pMaskTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_vecMaskTexList.push_back(pMaskTexture);

	TCHAR*	szMaskTexName = new TCHAR[128];		

	wsprintf(szMaskTexName, _T("Mask_%d"), m_iComMaskTexCount);

	m_vecComKey.push_back(szMaskTexName);

	m_htComponent.Insert_Table(szMaskTexName, pComponent);

	++m_iComMaskTexCount;


	Engine::CTexture*	pNormalTexture = NULL;
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, szNormalKey);
	pNormalTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_vecNormalTexList.push_back(pNormalTexture);

	TCHAR*	szNormalTexName = new TCHAR[128];		

	wsprintf(szNormalTexName, _T("Normal_%d"), m_iComNormalTexCount);

	m_vecComKey.push_back(szNormalTexName);

	m_htComponent.Insert_Table(szNormalTexName, pComponent);

	++m_iComNormalTexCount;


	m_vecMeshInfoList.push_back(pEffectInfo);



	return S_OK;
}

HRESULT Engine::CMesh_Effect::Add_Texture(const TCHAR* szTextureKey, int iSelect)
{
	size_t iTableSize =	m_htComponent.Get_TableSize();
	for(size_t i = 0; i < iTableSize; ++i)
	{
		CComponent*		pDelComponent = (Engine::CComponent*)m_htComponent.Search_TableData(i);
		if(m_vecTexList[iSelect] == dynamic_cast<Engine::CTexture*>(pDelComponent))
		{
			pDelComponent = (Engine::CComponent*)m_htComponent.Erase_Slot(i);
			Safe_Release(pDelComponent);
		}
	}

	CComponent*		pComponent = NULL;		

	Engine::CTexture*	pTexture = NULL;
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, szTextureKey);
	pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_vecTexList[iSelect] = pTexture;	
	
	TCHAR*	szTexName = new TCHAR[128];		

	wsprintf(szTexName, _T("Texture_%d"), m_iComMeshCount);

	m_vecComKey.push_back(szTexName);

	m_htComponent.Insert_Table(szTexName, pComponent);

	++m_iComTexCount;

	return S_OK;
}

vector<Engine::MESHEFFECTINFO*>* Engine::CMesh_Effect::GetMeshInfoList()
{
	return	&m_vecMeshInfoList;
}

vector<Engine::CTexture*>* Engine::CMesh_Effect::GetTexList()
{
	return &m_vecTexList;
}

void Engine::CMesh_Effect::TextureDelete(int iSelect)
{		
}

vector<Engine::CStaticMesh*>* Engine::CMesh_Effect::GetMeshList()
{
	return &m_vecMeshList;
}

int* Engine::CMesh_Effect::GetComMeshCount()
{
	return	&m_iComMeshCount;
}

int* Engine::CMesh_Effect::GetComTexCount()
{
	return	&m_iComTexCount;
}


void Engine::CMesh_Effect::ActionUpdate(_int iCount, const _float& fTimeDelta, _float fGetTime)
{
	//============================ Pos Action ===============================================
	if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_POS] != -1)
	{
		if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_POS] == false)
		{
			if(fGetTime > m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fEnd_Time)
			{
				m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fSpeed = 1.f;
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_POS])
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS] = 0;
			}
			else if(fGetTime > m_vecMeshInfoList[iCount]->vecPosAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fSpeed += m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fAcc * fTimeDelta;
				m_vecMeshInfoList[iCount]->vPos += m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].vPosValue * fTimeDelta * m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fSpeed;
			}
		}
		else
		{
			if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_POS] > m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fEnd_Time)
			{
				m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fSpeed = 1.f;
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_POS])
				{
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS] = 0;
					m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_POS] = 0.f;
				}

			}
			else if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_POS] > m_vecMeshInfoList[iCount]->vecPosAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fSpeed += m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fAcc * fTimeDelta;
				m_vecMeshInfoList[iCount]->vPos += m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].vPosValue * fTimeDelta * m_vecMeshInfoList[iCount]->vecPosAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_POS]].fSpeed;
			}
		}
	}

	//============================ Rot Action ===============================================
	if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_ROT] != -1)
	{
		if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_ROT] == false)
		{
			if(fGetTime > m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fEnd_Time)
			{
				m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fSpeed = 1.f;
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_ROT])
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT] = 0;				
			}
			else if(fGetTime > m_vecMeshInfoList[iCount]->vecRotAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fSpeed += m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fAcc * fTimeDelta;
				m_vecMeshInfoList[iCount]->vRot += m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].vRotValue * fTimeDelta * m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fSpeed;
			}
		}
		else
		{
			if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_ROT] > m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fEnd_Time)
			{	
				m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fSpeed = 1.f;
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_ROT])
				{
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT] = 0;
					m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_ROT] = 0.f;
				}
			}
			else if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_ROT] > m_vecMeshInfoList[iCount]->vecRotAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fSpeed += m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fAcc * fTimeDelta;
				m_vecMeshInfoList[iCount]->vRot += m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].vRotValue * fTimeDelta * m_vecMeshInfoList[iCount]->vecRotAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_ROT]].fSpeed;
			}
		}
	}

	//============================ Scale Action ===============================================
	if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_SCALE] != -1)
	{
		if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_SCALE] == false)
		{
			if(fGetTime > m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fEnd_Time)
			{
				m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fSpeed = 1.f;
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_SCALE])
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE] = 0;		
			}
			else if(fGetTime > m_vecMeshInfoList[iCount]->vecScaleAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fSpeed += m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fAcc * fTimeDelta;
				m_vecMeshInfoList[iCount]->vScale += m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].vScaleValue * fTimeDelta * m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fSpeed;
			}				
		}
		else
		{
			if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_SCALE] > m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fEnd_Time)
			{
				m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fSpeed = 1.f;
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_SCALE])
				{
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE] = 0;	
					m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_SCALE] = 0.f;
				}
			}
			else if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_SCALE] > m_vecMeshInfoList[iCount]->vecScaleAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fSpeed += m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fAcc * fTimeDelta;				
				m_vecMeshInfoList[iCount]->vScale += m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].vScaleValue * fTimeDelta * m_vecMeshInfoList[iCount]->vecScaleAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_SCALE]].fSpeed;
			}
		}
	}

	//============================ Fade Action ===============================================
	if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_FADE] != -1)
	{
		if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_FADE] == false)
		{
			if(fGetTime > m_vecMeshInfoList[iCount]->vecFadeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE]].fEnd_Time)
			{				
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_FADE])
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE] = 0;		
			}
			else if(fGetTime > m_vecMeshInfoList[iCount]->vecFadeAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE] ].fStart_Time)
			{				
				m_vecMeshInfoList[iCount]->fFade += m_vecMeshInfoList[iCount]->vecFadeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE]].fFadeValue * fTimeDelta; //* m_vecMeshInfoList[iCount]->vecFadeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE]].fSpeed;

				if(m_vecMeshInfoList[iCount]->fFade >= 1.f)
					m_vecMeshInfoList[iCount]->fFade = 1.f;

				if(m_vecMeshInfoList[iCount]->fFade <= 0.f)
					m_vecMeshInfoList[iCount]->fFade = 0.f;
			}
		}
		else
		{
			if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_FADE] > m_vecMeshInfoList[iCount]->vecFadeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE]].fEnd_Time)
			{				
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_FADE])
				{
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE] = 0;
					m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_FADE] = 0.f;
				}
			}
			else if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_FADE] > m_vecMeshInfoList[iCount]->vecFadeAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE] ].fStart_Time)
			{			
				m_vecMeshInfoList[iCount]->fFade += m_vecMeshInfoList[iCount]->vecFadeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE]].fFadeValue * fTimeDelta; //* m_vecMeshInfoList[iCount]->vecFadeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_FADE]].fSpeed;

				if(m_vecMeshInfoList[iCount]->fFade >= 1.f)
					m_vecMeshInfoList[iCount]->fFade = 1.f;

				if(m_vecMeshInfoList[iCount]->fFade <= 0.f)
					m_vecMeshInfoList[iCount]->fFade = 0.f;
			}
		}
	}

	//============================ UV Action ===============================================

	if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_UV] != -1)
	{
		if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_UV] == false)
		{
			if(fGetTime > m_vecMeshInfoList[iCount]->vecUVAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV]].fEnd_Time)
			{		
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_UV])
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV] = 0;		
			}
			else if(fGetTime > m_vecMeshInfoList[iCount]->vecUVAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->fUPos += m_vecMeshInfoList[iCount]->vecUVAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV]].fU * fTimeDelta;
				m_vecMeshInfoList[iCount]->fVPos += m_vecMeshInfoList[iCount]->vecUVAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV]].fV * fTimeDelta;
			}
		}
		else
		{
			if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_UV] > m_vecMeshInfoList[iCount]->vecUVAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV]].fEnd_Time)
			{			
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_UV])
				{
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV] = 0;
					m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_UV] = 0.f;
				}
			}
			else if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_UV] > m_vecMeshInfoList[iCount]->vecUVAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->fUPos += m_vecMeshInfoList[iCount]->vecUVAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV]].fU * fTimeDelta;
				m_vecMeshInfoList[iCount]->fVPos += m_vecMeshInfoList[iCount]->vecUVAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UV]].fV * fTimeDelta;
			
			}
		}
	}	

	//================================= UVSize Action ==========================================

	if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_UVSIZE] != -1)
	{
		if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_UVSIZE] == false)
		{
			if(fGetTime > m_vecMeshInfoList[iCount]->vecUVSizeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE]].fEnd_Time)
			{				
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_UVSIZE])
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE] = 0;		
			}
			else if(fGetTime > m_vecMeshInfoList[iCount]->vecUVSizeAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE] ].fStart_Time)
			{
				m_vecMeshInfoList[iCount]->fUSize += m_vecMeshInfoList[iCount]->vecUVSizeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE]].fUSize * fTimeDelta;
				m_vecMeshInfoList[iCount]->fVSize += m_vecMeshInfoList[iCount]->vecUVSizeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE]].fVSize * fTimeDelta;
			}
		}
		else
		{
			if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_UVSIZE] > m_vecMeshInfoList[iCount]->vecUVSizeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE]].fEnd_Time)
			{			
				++m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE];

				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_UVSIZE])
				{
					m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE] = 0;
					m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_UVSIZE] = 0.f;
				}
			}
			else if(m_vecMeshInfoList[iCount]->fUnLimitedTime[ACT_UVSIZE] > m_vecMeshInfoList[iCount]->vecUVSizeAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE] ].fStart_Time)
			{			
				m_vecMeshInfoList[iCount]->fUSize += m_vecMeshInfoList[iCount]->vecUVSizeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE]].fUSize * fTimeDelta;
				m_vecMeshInfoList[iCount]->fVSize += m_vecMeshInfoList[iCount]->vecUVSizeAction[m_vecMeshInfoList[iCount]->iActionIndex[ACT_UVSIZE]].fVSize * fTimeDelta;
			}
		}
	}



	//============================ Sprite Action ===============================================



	if(m_vecMeshInfoList[iCount]->bAliveOn == true)
	{
		m_vecMeshInfoList[iCount]->fSpriteTime += fTimeDelta;			

		if( m_vecMeshInfoList[iCount]->fSpriteTime > m_vecMeshInfoList[iCount]->fSpriteTotalTime / m_vecMeshInfoList[iCount]->iSpriteMaxCount)
		{
			m_vecMeshInfoList[iCount]->fSpriteTime = 0.f;

			++m_vecMeshInfoList[iCount]->iSpriteCount;

			if( m_vecMeshInfoList[iCount]->iSpriteCount > m_vecMeshInfoList[iCount]->iSpriteMaxCount)
				m_vecMeshInfoList[iCount]->iSpriteCount = 0;
		}
	}

	//============================== Color Action ========================================

	if(m_vecMeshInfoList[iCount]->bAliveOn == true)
	{
		if(m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_COLOR] != -1)
		{
			m_vecMeshInfoList[iCount]->fColorTime += fTimeDelta;

			if(m_vecMeshInfoList[iCount]->fColorTime > m_vecMeshInfoList[iCount]->vecColorAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR]].fNext_Time)
			{
				m_vecMeshInfoList[iCount]->fColorChangeTime += fTimeDelta;
				if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR] + 1 < m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_COLOR])
				{
					m_vecMeshInfoList[iCount]->vRGBDifference = m_vecMeshInfoList[iCount]->vecColorAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR] + 1 ].vRGB -  m_vecMeshInfoList[iCount]->vecColorAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR]].vRGB;

					D3DXVECTOR3 vDifference = m_vecMeshInfoList[iCount]->vRGBDifference / m_vecMeshInfoList[iCount]->vecColorAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR]].fChange_Time;

					m_vecMeshInfoList[iCount]->vRGB += vDifference * fTimeDelta;

					if( m_vecMeshInfoList[iCount]->fColorChangeTime > m_vecMeshInfoList[iCount]->vecColorAction[ m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR]].fChange_Time)
					{
						m_vecMeshInfoList[iCount]->fColorTime = 0.f;
						m_vecMeshInfoList[iCount]->fColorChangeTime = 0.f;
						++m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR];

						if(m_vecMeshInfoList[iCount]->bUnLimited[ACT_COLOR] == false)
						{
							if(m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR] >= m_vecMeshInfoList[iCount]->iActionMaxIndex[ACT_COLOR])
							{
								m_vecMeshInfoList[iCount]->iActionIndex[ACT_COLOR] = 0;							
							}
						}
					}
				}
			}
		}
	}

	//=======================°æ°è
	_mymat matScale, matRotX, matRotY, matRotZ, matTrans, matParent, matParentTrans, matRov;

	if(m_vScale.x != -1.f && m_vScale.y != -1.f && m_vScale.z != -1.f)
	{
		matScale.Scale(m_vecMeshInfoList[iCount]->vScale + m_vScale);
	}
	else
		matScale.Scale(m_vecMeshInfoList[iCount]->vScale);

	matRotX.RotX(D3DXToRadian(m_vecMeshInfoList[iCount]->vRot.x));
	matRotY.RotY(D3DXToRadian(m_vecMeshInfoList[iCount]->vRot.y));
	matRotZ.RotZ(D3DXToRadian(m_vecMeshInfoList[iCount]->vRot.z));

	matTrans.Trans(m_vecMeshInfoList[iCount]->vPos);

	//matParent.Identity();

	if(m_vTargetPos != NULL)
		matParentTrans.Trans((*m_vTargetPos));
	else
		matParentTrans.Trans(m_vStartPos);


	if(m_pAngle[ANGLE_X] != NULL || m_pAngle[ANGLE_Y] != NULL || m_pAngle[ANGLE_Z] != NULL)
	{
		_mymat matParentRotX, matParentRotY, matParentRotZ;

		if(m_pAngle[ANGLE_X] != NULL)
			matParentRotX.RotX(*m_pAngle[ANGLE_X]);
		else
			matParentRotX.RotX(D3DXToRadian(0.f));

		if(m_pAngle[ANGLE_Y] != NULL)
			matParentRotY.RotY(*m_pAngle[ANGLE_Y]);
		else
			matParentRotY.RotY(D3DXToRadian(0.f));

		if(m_pAngle[ANGLE_Z] != NULL)
			matParentRotZ.RotZ(*m_pAngle[ANGLE_Z]);
		else
			matParentRotZ.RotZ(D3DXToRadian(0.f));


		matParent = matParentRotX * matParentRotY * matParentRotZ * matParentTrans;
	}
	else
		matParent = matParentTrans;

	


	if(m_vecMeshInfoList[iCount]->bBillOn == true)
   {
      D3DXMATRIX      matBill;
      D3DXMatrixIdentity(&matBill);

      m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
      ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

      D3DXMatrixInverse(&matBill, 0, &matBill);
      m_vecMeshInfoList[iCount]->matWorld *= matBill; //* m_matParent;

      matParent = matBill * matParent;

      m_vecMeshInfoList[iCount]->matWorld = matScale * matRotX * matRotY * matRotZ * matTrans * matParent;

   }
   else
   {
      if(m_vecMeshInfoList[iCount]->bSingleBillOn == true)
      {
         D3DXMATRIX      matBill;
         D3DXMatrixIdentity(&matBill);

         m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
         ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

         D3DXMatrixInverse(&matBill, 0, &matBill);

         matTrans.Trans(m_vecMeshInfoList[iCount]->vPos + m_vStartPos);

         m_vecMeshInfoList[iCount]->matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
         m_vecMeshInfoList[iCount]->matWorld = matBill * m_vecMeshInfoList[iCount]->matWorld;
		 m_vecMeshInfoList[iCount]->matWorld *= matParent;
      }
      else
	  {
		  if(m_vecMeshInfoList[iCount]->bParRot == true)
		  {

			  D3DXMATRIX		matBill;
			  matRov.RotY(D3DXToRadian(m_vecMeshInfoList[iCount]->vRot.y));

			  matRov *= matParent;
			  matBill = matRov;
			  ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
			  D3DXMatrixInverse(&matBill, 0, &matBill);

			  m_vecMeshInfoList[iCount]->matWorld = matScale * matRotX * matRotY * matRotZ * matTrans * matRov;
			  m_vecMeshInfoList[iCount]->matWorld = matBill * m_vecMeshInfoList[iCount]->matWorld;	

		  }
		  else
			  m_vecMeshInfoList[iCount]->matWorld = matScale * matRotX * matRotY * matRotZ * matTrans * matParent;
	  }

   }
	
} 

void Engine::CMesh_Effect::EffectReset()
{
	for(unsigned int i = 0; i < m_vecMeshInfoList.size(); ++i)
	{
		m_vecMeshInfoList[i]->vPos			= m_vecMeshInfoList[i]->vOriPos;
		m_vecMeshInfoList[i]->vRot			= m_vecMeshInfoList[i]->vOriRot;
		m_vecMeshInfoList[i]->vScale		= m_vecMeshInfoList[i]->vOriScale;
		m_vecMeshInfoList[i]->fFade			= m_vecMeshInfoList[i]->fOriFade;			
		m_vecMeshInfoList[i]->iSpriteCount	= 0;
		m_vecMeshInfoList[i]->fUSize		= m_vecMeshInfoList[i]->fOriUSize;
		m_vecMeshInfoList[i]->fVSize		= m_vecMeshInfoList[i]->fOriVSize;
		m_vecMeshInfoList[i]->fUPos			= m_vecMeshInfoList[i]->fOriUPos;
		m_vecMeshInfoList[i]->fVPos			= m_vecMeshInfoList[i]->fOriVPos;

		for( int iActCount = 0; iActCount < ACT_END; ++iActCount)
		{
			if(m_vecMeshInfoList[i]->iActionMaxIndex[iActCount] != -1)
				m_vecMeshInfoList[i]->iActionIndex[iActCount] = 0;

			m_vecMeshInfoList[i]->fUnLimitedTime[iActCount] = 0.f;
			
		}

		m_vecMeshInfoList[i]->fColorChangeTime = 0.f;
		m_vecMeshInfoList[i]->fColorTime = 0.f;

		if(m_vecMeshInfoList[i]->iActionMaxIndex[ACT_COLOR] != -1)
			m_vecMeshInfoList[i]->vRGB = m_vecMeshInfoList[i]->vecColorAction.front().vRGB;


		for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[i]->vecPosAction.size(); ++iSize)
			m_vecMeshInfoList[i]->vecPosAction[iSize].fSpeed = 1.f;

		for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[i]->vecRotAction.size(); ++iSize)
			m_vecMeshInfoList[i]->vecRotAction[iSize].fSpeed = 1.f;

		for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[i]->vecScaleAction.size(); ++iSize)
			m_vecMeshInfoList[i]->vecScaleAction[iSize].fSpeed = 1.f;

	/*	for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[i]->vecUVAction.size(); ++iSize)
		{
			m_vecMeshInfoList[i]->vecUVAction[iSize].fTransU = m_vecMeshInfoList[i]->vecUVAction[iSize].fOriU;
			m_vecMeshInfoList[i]->vecUVAction[iSize].fTransV = m_vecMeshInfoList[i]->vecUVAction[iSize].fOriV;
			}*/

		for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[i]->vecFadeAction.size(); ++iSize)
			m_vecMeshInfoList[i]->vecFadeAction[iSize].fSpeed = 1.f;

		m_vecMeshInfoList[i]->iCycle = m_vecMeshInfoList[i]->iOriCycle;

		m_vecMeshInfoList[i]->bEnd = false;

		m_vecMeshInfoList[i]->fGetTime = 0.f;

		m_vTargetPos = NULL;
		m_pAngle[ANGLE_X] = NULL;
		m_pAngle[ANGLE_Y] = NULL;
		m_pAngle[ANGLE_Z] = NULL;

		m_fGetTime = 0.f;

		if(m_eState == STATE_READY)
			m_bCalling = false;

		if(m_bShield == true)
			ShieldInit();
	}
}

void Engine::CMesh_Effect::SingleReset(int iNum)
{
	m_vecMeshInfoList[iNum]->vPos			= m_vecMeshInfoList[iNum]->vOriPos;
	m_vecMeshInfoList[iNum]->vRot			= m_vecMeshInfoList[iNum]->vOriRot;
	m_vecMeshInfoList[iNum]->vScale			= m_vecMeshInfoList[iNum]->vOriScale;
	m_vecMeshInfoList[iNum]->fFade			= m_vecMeshInfoList[iNum]->fOriFade;			
	m_vecMeshInfoList[iNum]->iSpriteCount	= 0;
	m_vecMeshInfoList[iNum]->fUSize			= m_vecMeshInfoList[iNum]->fOriUSize;
	m_vecMeshInfoList[iNum]->fVSize			= m_vecMeshInfoList[iNum]->fOriVSize;
	m_vecMeshInfoList[iNum]->fUPos			= m_vecMeshInfoList[iNum]->fOriUPos;
	m_vecMeshInfoList[iNum]->fVPos			= m_vecMeshInfoList[iNum]->fOriVPos;

	for( int iActCount = 0; iActCount < ACT_END; ++iActCount)
	{
		if(m_vecMeshInfoList[iNum]->iActionMaxIndex[iActCount] != -1)
			m_vecMeshInfoList[iNum]->iActionIndex[iActCount] = 0;

		m_vecMeshInfoList[iNum]->fUnLimitedTime[iActCount] = 0.f;
	}

	m_vecMeshInfoList[iNum]->fColorChangeTime = 0.f;
	m_vecMeshInfoList[iNum]->fColorTime = 0.f;

	if(m_vecMeshInfoList[iNum]->iActionMaxIndex[ACT_COLOR] != -1)
		m_vecMeshInfoList[iNum]->vRGB = m_vecMeshInfoList[iNum]->vecColorAction.front().vRGB;

	for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[iNum]->vecPosAction.size(); ++iSize)
		m_vecMeshInfoList[iNum]->vecPosAction[iSize].fSpeed = 1.f;

	for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[iNum]->vecRotAction.size(); ++iSize)
		m_vecMeshInfoList[iNum]->vecRotAction[iSize].fSpeed = 1.f;

	for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[iNum]->vecScaleAction.size(); ++iSize)
		m_vecMeshInfoList[iNum]->vecScaleAction[iSize].fSpeed = 1.f;

	for(unsigned int iSize = 0; iSize < m_vecMeshInfoList[iNum]->vecFadeAction.size(); ++iSize)
		m_vecMeshInfoList[iNum]->vecFadeAction[iSize].fSpeed = 1.f;

	m_vecMeshInfoList[iNum]->fGetTime = 0.f;
}

void Engine::CMesh_Effect::Call_Effect(D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ, D3DXVECTOR3 vScale)
{
	if(bChase == true)
		m_vTargetPos = vPos;
	else
		m_vStartPos = (*vPos);

	m_pAngle[ANGLE_X] = pAngleX;
	m_pAngle[ANGLE_Y] = pAngleY;
	m_pAngle[ANGLE_Z] = pAngleZ;

	m_vScale = vScale;
	m_eState = STATE_START;

	m_bCalling = true;
}

void Engine::CMesh_Effect::Call_Effect(D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	if(bChase == true)
		m_vTargetPos = vPos;
	else
		m_vStartPos = (*vPos);

	m_pAngle[ANGLE_X] = pAngleX;
	m_pAngle[ANGLE_Y] = pAngleY;
	m_pAngle[ANGLE_Z] = pAngleZ;

	m_eState = STATE_START;

	m_bCalling = true;
}

bool Engine::CMesh_Effect::GetCalling()
{
	return m_bCalling;
}

void Engine::CMesh_Effect::Off_FX()
{
	m_eState = STATE_READY;
	EffectReset();
}

float Engine::CMesh_Effect::GetTime()
{
	return m_fGetTime;
}

void Engine::CMesh_Effect::SetNumber(int iNumber)
{
	m_iNumber = iNumber;
}

int Engine::CMesh_Effect::GetNumber()
{
	return m_iNumber;
}

void Engine::CMesh_Effect::ShieldCounting()
{
	
	for(unsigned int i = 0; i < m_vecMeshInfoList.size(); ++i)
	{
		
		if(m_vecMeshInfoList[i]->bShield == false)
		{
			m_vecMeshInfoList[i]->bShield = true;
			m_vecMeshInfoList[i]->fGetTime = 999999.f;
			m_vecMeshInfoList[i]->bAliveOn = false;
			
			return;
		}
	}	
}

void Engine::CMesh_Effect::SetShield()
{
	for(unsigned int i = 0; i < 54; ++i)
	{
		m_vecMeshInfoList[i]->bParRot = true;			
	}	
}

void Engine::CMesh_Effect::ShieldInit()
{
	m_bShield = true;
	for(unsigned int i = 0; i < 54; ++i)
	{
		m_vecMeshInfoList[i]->bShield = false;		
	}	
}

