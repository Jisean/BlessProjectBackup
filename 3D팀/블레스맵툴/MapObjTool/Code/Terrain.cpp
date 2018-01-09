#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "MapTool.h"
#include "resource.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Back.h"
#include "SplatingTool.h"


//.alpha가 스플래팅될 텍스쳐 .ptexture가 일반적인 타일텍스쳐

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pTerrainVtx(NULL)
, m_pIndex(NULL)
, m_dwTriCnt(0)
, m_pFilterTexture(NULL)
, m_SaveAlphaTexture(NULL)
, m_iFilterCnt(0)
, m_iFilterViewCnt(0)
{
	ZeroMemory(&m_tMtrl, sizeof(D3DMATERIAL9));
}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Initialize(int iCntX, int iCntZ, float fUVCnt)
{
	FAILED_CHECK(AddComponent());

	m_iCntX = iCntX;
	m_iCntZ = iCntZ;
	m_fUV	= fUVCnt;

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	m_tMtrl.Power = 20.f;

	m_pTerrainVtx = new Engine::VTXTEX[iCntX * iCntZ];
	m_pBuffer->GetVtxInfo(m_pTerrainVtx);

	m_pIndex = new Engine::INDEX32[(iCntX - 1) * (iCntZ - 1) * 2];
	m_pBuffer->GetIdxInfo(m_pIndex, m_dwTriCnt);



	return S_OK;
}

int CTerrain::Update(const float& fTimeDelta)
{
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_NONEALPHA, this);
	return Engine::CGameObject::Update(Engine::Get_Time());
}

void CTerrain::Render(void)
{	

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	D3DXMATRIX			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fUVMount", m_fUV);

	if(m_pTexture != NULL)
	{
		m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture",0);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture", 1);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture2", 2);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture3", 3);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture4", 4);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture5", 5);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture6", 6);
		m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture7", 7);

	}

	if(m_pFilterTexture != NULL)
	{
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture",0);
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture2", 1);
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture3", 2);
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture4", 3);
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture5", 4);
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture6", 5);
		m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture7", 6);
	}
	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)&pLightInfo->Specular);

	pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)&m_tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (D3DXVECTOR4*)&m_tMtrl.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (D3DXVECTOR4*)&m_tMtrl.Specular);


	D3DXMATRIX			matViewInv;

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (D3DXVECTOR4*)&matViewInv.m[3][0]);	
	pEffect->SetFloat("g_fPower", m_tMtrl.Power);

	pEffect->SetVector("g_vBrushPos", &D3DXVECTOR4(5.f, 0.f, 5.f, 1.f));
	pEffect->SetFloat("g_fRange", 3.f);

	CMapTool* pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	CSplatingTool* pSplatTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab4;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	if(pMapTool->m_WireFrame.GetCheck() == TRUE)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else
	{
		m_pBuffer->Render(&m_pInfo->m_matWorld);
	}

	pEffect->EndPass();
	pEffect->End();

	if(pMapTool->IsWindowVisible() == TRUE)
	{
		DrawPickCircle(30, (float)pMapTool->m_dPickSize, D3DXCOLOR(1.f,1.f,1.f,1.f));
	}


	if(pSplatTool->IsWindowVisible() == TRUE)
	{
		DrawPickCircle(30, pSplatTool->m_fAlphaRadius, D3DXCOLOR(1.f,1.f,1.f,1.f));
		DrawPickCircle(30, pSplatTool->m_fBrushSize, D3DXCOLOR(1.f,1.f,1.f,1.f));
		if(m_pFilterTexture != NULL && m_iFilterViewCnt > 0)
			MiniAlphaTextureRender();
	}


	Engine::Safe_Release(pEffect);
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Buffer_Terrain");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	/*pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Terrain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Filter");
	m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"FilterTexture", pComponent));*/

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}


CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice, int iCntX/* = VTXCNTX*/, int iCntZ/* = VTXCNTZ*/, float fUVCnt)
{
	CTerrain*		pGameObject = new CTerrain(pDevice);
	if(FAILED(pGameObject->Initialize(iCntX, iCntZ, fUVCnt)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Delete_Array(m_pTerrainVtx);
	Engine::Safe_Delete_Array(m_pIndex);	
}
Engine::VTXTEX* CTerrain::GetTerrainVtx(void)
{
	return m_pTerrainVtx;
}

HRESULT CTerrain::SetTexture(void)
{
	Engine::CComponent*		pComponent = NULL;
	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Terrain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = Engine::Clone_Resource(RESOURCE_TOOL, L"Texture_Filter");
	m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"FilterTexture", pComponent));



	return S_OK;
}


HRESULT CTerrain::SetHeight(void)
{
	int		iIndex = 0;
	for(int z = 0; z < m_iCntZ; ++z)
	{
		for(int x = 0; x < m_iCntX; ++x)
		{
			iIndex = z * m_iCntX + x;

			m_pTerrainVtx[iIndex].vPos.y = (m_pdwPixel[iIndex] & 0x000000ff) / 100.f;
		}
	}

	SetNormal();

	return S_OK;
}

HRESULT CTerrain::ResetHeight(void)
{
	int		iIndex = 0;
	for(int z = 0; z < m_iCntZ; ++z)
	{
		for(int x = 0; x < m_iCntX; ++x)
		{
			iIndex = z * m_iCntX + x;

			m_pTerrainVtx[iIndex].vPos.y = 0.f;
			m_pTerrainVtx[iIndex].vNormal = D3DXVECTOR3(0.f, 1.f, 0.f);
		}
	}
	m_pBuffer->SetVtxInfo(m_pTerrainVtx);

	return S_OK;
}

HRESULT CTerrain::ChangeHeight(D3DXVECTOR3 vPoint, float fHeight, DOUBLE dChangeSize)
{
	int		iIndex = 0;
	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;
	for(int z = 0; z < m_iCntZ; ++z)
	{
		for(int x = 0; x < m_iCntX; ++x)
		{
			iIndex = z * m_iCntX + x;

			if(pMapTool->m_MainHeightMode[0].GetCheck() == TRUE)
			{
				if(m_pTerrainVtx[iIndex].vPos == vPoint)
				{
					m_pTerrainVtx[iIndex].vPos.y = fHeight;
					ChangeNearHeight(vPoint, fHeight, dChangeSize);
				}
			}
			else
			{
				D3DXVECTOR3 vLength = m_pTerrainVtx[iIndex].vPos - vPoint;
				float fLength = D3DXVec3Length(&vLength);

				if(pMapTool->m_SoftHeightMode[0].GetCheck() == TRUE)
				{
					if((DOUBLE)fLength < dChangeSize)
					{	
						float Height = fHeight * cosf(D3DXToRadian(fLength / dChangeSize) * 90);
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight)
							m_pTerrainVtx[iIndex].vPos.y += 0.06f;
						else
							m_pTerrainVtx[iIndex].vPos.y -= 0.06f;
					}
				}
				else
				{
					if((DOUBLE)fLength < dChangeSize)
					{	
						float fReverseLength = dChangeSize;

						fReverseLength -= fLength;

						if(pMapTool->m_SmoothUpDown[0].GetCheck() == TRUE)
							m_pTerrainVtx[iIndex].vPos.y += (fReverseLength * pMapTool->m_fSmoothPower);

						else
							m_pTerrainVtx[iIndex].vPos.y -= (fReverseLength * pMapTool->m_fSmoothPower);


						/*if(m_pTerrainVtx[iIndex].vPos.y < fHeight)
						m_pTerrainVtx[iIndex].vPos.y += 0.06f;
						else
						m_pTerrainVtx[iIndex].vPos.y -= 0.06f;*/
					}
				}


			/*	D3DXVECTOR3 vCheckPos = *pOut - pVertex[iIndex].vPos;

				float fLength = D3DXVec3Length(&vCheckPos);

				if(fLength < fRange)
				{
					float fReverseLength = fRange;

					fReverseLength -= fLength;

					if(bUpDown == true)
						pVertex[iIndex].vPos.y += (fReverseLength * 1.f)  * Engine::Get_Time();

					if(bUpDown == false)
						pVertex[iIndex].vPos.y -= (fReverseLength * 1.f)  * Engine::Get_Time();*/

			}
		}
	}

	return S_OK;
}

void CTerrain::SetNormal(void)
{
	int		iIndex = 0;
	int iTriCnt = 0;
	for(int z = 0; z < m_iCntZ - 1; ++z)
	{
		for(int x = 0; x < m_iCntX - 1; ++x)
		{
			iIndex = z * m_iCntX + x;

			m_pIndex[iTriCnt]._1 = iIndex + m_iCntX;
			m_pIndex[iTriCnt]._2 = iIndex + m_iCntX + 1;
			m_pIndex[iTriCnt]._3 = iIndex + 1;

			D3DXVECTOR3		vDest, vSour, vNormal;
			vDest = m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._1].vPos;
			vSour = m_pTerrainVtx[m_pIndex[iTriCnt]._3].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			m_pTerrainVtx[m_pIndex[iTriCnt]._1].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._2].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;

			m_pIndex[iTriCnt]._1 = iIndex + m_iCntX;
			m_pIndex[iTriCnt]._2 = iIndex + 1;
			m_pIndex[iTriCnt]._3 = iIndex;
			vDest = m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._1].vPos;
			vSour = m_pTerrainVtx[m_pIndex[iTriCnt]._3].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			m_pTerrainVtx[m_pIndex[iTriCnt]._1].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._2].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;
		}
	}
	m_pBuffer->SetVtxInfo(m_pTerrainVtx);
}

void CTerrain::ChangeNearHeight(D3DXVECTOR3 vPoint, float fHeight, DOUBLE dChangeSize)
{
	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab1;

	int		iIndex = 0;
	for(int z = 0; z < m_iCntZ; ++z)
	{
		for(int x = 0; x < m_iCntX; ++x)
		{
			iIndex = z * m_iCntX + x;
			D3DXVECTOR3 vLength = m_pTerrainVtx[iIndex].vPos - vPoint;
			float fLength = D3DXVec3Length(&vLength);

			if(TRUE == pMapTool->m_ModifyMode[2].GetCheck())
			{
				if((DOUBLE)fLength < dChangeSize)
				{
					m_pTerrainVtx[iIndex].vPos.y = fHeight;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.1f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.9f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.9f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.9f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.2f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.8f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.8f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.8f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.3f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.7f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.7f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.7f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.4f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.6f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.6f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.6f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.5f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.5f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.5f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.5f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.6f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.4f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.4f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.4f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.7f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.3f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.3f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.3f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.8f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.2f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.2f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.2f;
				}
				else if((DOUBLE)fLength < dChangeSize * 1.9f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.1f)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f * 0.1f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f * 0.1f;
				}

				/*if((DOUBLE)fLength < dChangeSize * 2.f)
				{
					if(m_pTerrainVtx[iIndex].vPos.y < fHeight)
						m_pTerrainVtx[iIndex].vPos.y += 0.05f;
					else
						m_pTerrainVtx[iIndex].vPos.y -= 0.05f;
				}*/
			}
			else
			{
				if(pMapTool->m_Gradient[0].GetCheck())
				{
					if((DOUBLE)fLength < dChangeSize)
					{
						m_pTerrainVtx[iIndex].vPos.y = fHeight;
					}
				}
				else if(pMapTool->m_Gradient[1].GetCheck())
				{
					if((DOUBLE)fLength < dChangeSize)
					{
						m_pTerrainVtx[iIndex].vPos.y = fHeight;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.1f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.9f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.9f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.2f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.8f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.8f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.3f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.7f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.7f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.4f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.6f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.6f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.5f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.5f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.5f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.6f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.4f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.4f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.7f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.3f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.3f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.8f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.2f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.2f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.9f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.1f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.1f;
					}
				}
				else if(pMapTool->m_Gradient[2].GetCheck())
				{
					if((DOUBLE)fLength < dChangeSize)
					{
						m_pTerrainVtx[iIndex].vPos.y = fHeight;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.1f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.8f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.8f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.2f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.6f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.6f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.3f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.4f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.4f;
					}
					else if((DOUBLE)fLength < dChangeSize * 1.4f)
					{
						if(m_pTerrainVtx[iIndex].vPos.y < fHeight * 0.2f)
							m_pTerrainVtx[iIndex].vPos.y = fHeight * 0.2f;
					}
				}
			}
		}
	}
}

void CTerrain::DrawPickCircle(int iCount, float fSize, D3DXCOLOR Color)
{
	if ( iCount < 4 ) iCount = 4;
	float Step = (D3DX_PI * 2) / iCount;

	Line3D          S_Line[2];
	D3DXVECTOR3     NewPos, Pos, MousePos;
	D3DXMATRIXA16   matRot;

	((CBack*)Engine::Get_Scene())->m_pMouseCol->PickTerrain(&MousePos,m_pTerrainVtx);

	Pos = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );

	//size도 원의 지름이 넘어오므로 반지름으로 만들어줘야한다
	fSize = fSize/2.0f;

	S_Line[1].Pos = Pos * fSize + MousePos;
	S_Line[1].Col = S_Line[0].Col = Color;
	S_Line[1].Pos.y = MousePos.y;
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	for ( int i = 1; i < iCount + 1; ++i) {
		S_Line[0].Pos = S_Line[1].Pos;

		D3DXMatrixRotationY( &matRot, i * Step );
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot );
		D3DXVec3Normalize( &NewPos, &NewPos );

		S_Line[1].Pos = (NewPos * fSize) + MousePos;
		S_Line[1].Pos.y = MousePos.y;



		m_pDevice->SetFVF( Engine::VTXFVF_COL );
		m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, S_Line, sizeof( Line3D ) );

		//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}
}

void CTerrain::MiniAlphaTextureRender(void)
{
	CSplatingTool* pSplatTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab4;
	m_iFilterCnt = pSplatTool->m_iCurrentFilter;

	static float MiniAlphaTex[4][6] = 
	{
		{ WINCX - 128, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ WINCX,0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
		{ WINCX - 128, 128, 0.0f, 1.0f, 0.0f, 1.0f },
		{ WINCX,128, 0.0f, 1.0f, 1.0f, 1.0f },
	};

	m_pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );	
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	if(m_iFilterCnt > 0)
	{
		m_pFilterTexture->Render(0, m_iFilterCnt - 1);
	}
	m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, MiniAlphaTex, sizeof(MiniAlphaTex[0]) );	
}
HRESULT CTerrain::DrawAlphamaps(D3DXVECTOR3 MousePos, int iIndex, float fBrushSize, float fAlphaRadius ) 
{	
	// 알파맵의 한 픽셀이 월드 상의 한점의 크기를 구한다.
	// 1.0f <-현재 사각형1개만그렸으므로
	//float		PixSize		= 1.0f/(float)TEXALPHASIZE;
	float		fTotalSize = float(m_iCntX + m_iCntZ);
	float		PixSize		= (float)m_iCntX/(float)fTotalSize;
	CSplatingTool* pSplatTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab4;
	m_iFilterCnt = pSplatTool->m_iCurrentFilter - 1;


	//전역브러시 와 전역스무스는 현재 브러쉬할원의 지름을 넣기대문에 반지름을 사용해야한다.
	float 	    fHalfBrushSize	= fBrushSize/2.0f;
	float		fHalfSmoothSize = fAlphaRadius/2.0f;

	// 에디터의 원의 최대 크기에 한점의 크기를 나누워서
	// 몇픽셀을 에디터 하는지 크기를 구한다.
	float		EditSize = fHalfBrushSize/PixSize;

	float	PosU = MousePos.x / (float)(m_iCntX);
	float	PosV = MousePos.z / (float)(m_iCntZ);
	//float	PosV = m_vGetPos.z / (float)(MAPHEIGHT);
	

	m_iTexPosX = int(PosU  * fTotalSize);
	m_iTexPosY = int(PosV  * fTotalSize);

	int StartPosX = int(( (m_iTexPosX - EditSize) < 0 )? 0 : m_iTexPosX - EditSize);
	int StartPosY = int(( (m_iTexPosY - EditSize) < 0 )? 0 : m_iTexPosY - EditSize);
	int EndPosX   = int(( (m_iTexPosX + EditSize) >= fTotalSize )? fTotalSize - 1: m_iTexPosX + EditSize);
	int EndPosY   = int(( (m_iTexPosY + EditSize) >= fTotalSize )? fTotalSize - 1: m_iTexPosY + EditSize);

	DWORD dwChangeColor = 0x00;
	float Smooth		= 0.0f;
	DWORD dwA			= 0x00;

	D3DLOCKED_RECT		AlphaMap_Locked;

	memset( &AlphaMap_Locked, 0, sizeof(D3DLOCKED_RECT) );

	if( FAILED(m_pFilterTexture->LockRect(m_iFilterCnt, &AlphaMap_Locked, NULL, 0)) )
	{
		MessageBox(NULL, L"alphaTexture Lock error", L"Error", MB_OK);
		return E_FAIL;
	}


	LPBYTE pDataDST = (LPBYTE) AlphaMap_Locked.pBits;

	for( int j=StartPosY; j<=EndPosY; ++j )
	{
		LPDWORD pDWordDST = (LPDWORD) (pDataDST + j * AlphaMap_Locked.Pitch);

		for( int i=StartPosX; i<=EndPosX; ++i )
		{
			D3DXVECTOR3 Pix = D3DXVECTOR3( i * PixSize, 0.0f, j * PixSize ) -
				D3DXVECTOR3( m_iTexPosX * PixSize, 0.0f, m_iTexPosY * PixSize ); 

			float Len = D3DXVec3Length( &Pix );

			if( Len <= fHalfSmoothSize )
			{
				dwChangeColor = 0xFF000000;
			}
			else if(  Len <= fHalfBrushSize  )
			{
				// 최대사이즈에 포함될 우 최대사이즈까지의
				// 선영보간한 알파값을 구한다.
				Len  -= fHalfSmoothSize;
				Smooth = fHalfBrushSize - fHalfSmoothSize;
				dwChangeColor= DWORD(( Len - Smooth ) / Smooth  * 0xFFFFFFFF);
				D3DXCOLOR dwcc = dwChangeColor;
				dwA = (dwChangeColor & 0xFFFFFFFF) >> 24;
				D3DXCOLOR dwaa = dwA;
				dwChangeColor = (dwA << 24) | (dwA << 16) | (dwA << 8) | dwA;
				dwcc = dwChangeColor;
				if(dwcc.a < 1.f)
					dwcc.a = 1.f;
				dwChangeColor = dwcc;

			}
			else
			{
				continue;
			}

			*(pDWordDST + i) = ( *(pDWordDST + i) > dwChangeColor ) ? dwChangeColor : *(pDWordDST + i);
			D3DXCOLOR temp = *(pDWordDST + i);

			if (temp.a < 1.f)
				temp.a = 1.f;
			*(pDWordDST + i) = temp;
		}
	}

	if( FAILED(m_pFilterTexture->UnLockRect(m_iFilterCnt)) )	
	{
		MessageBox(NULL, L"alphaTexture UnLock error", L"Error", MB_OK);
		return E_FAIL;
	}




	m_SaveAlphaTexture = m_pFilterTexture->GetTexture(m_iFilterCnt);

	D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeightsave.png", D3DXIFF_PNG, *m_SaveAlphaTexture, NULL);


	return S_OK;
}

HRESULT CTerrain::EraseAlphamaps(D3DXVECTOR3 MousePos, int iIndex, float fBrushSize, float fAlphaRadius ) 
{	
	// 알파맵의 한 픽셀이 월드 상의 한점의 크기를 구한다.
	// 1.0f <-현재 사각형1개만그렸으므로
	//float		PixSize		= 1.0f/(float)TEXALPHASIZE;
	float		fTotalSize = float(m_iCntX + m_iCntZ);
	float		PixSize		= (float)m_iCntX/(float)fTotalSize;
	CSplatingTool* pSplatTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab4;
	m_iFilterCnt = pSplatTool->m_iCurrentFilter - 1;


	//전역브러시 와 전역스무스는 현재 브러쉬할원의 지름을 넣기대문에 반지름을 사용해야한다.
	float 	    fHalfBrushSize	= fBrushSize/2.0f;
	float		fHalfSmoothSize = fAlphaRadius/2.0f;

	// 에디터의 원의 최대 크기에 한점의 크기를 나누워서
	// 몇픽셀을 에디터 하는지 크기를 구한다.
	float		EditSize = fHalfBrushSize/PixSize;

	float	PosU = MousePos.x / (float)(m_iCntX);
	float	PosV = MousePos.z / (float)(m_iCntZ);
	//float	PosV = m_vGetPos.z / (float)(MAPHEIGHT);
	

	m_iTexPosX = int(PosU  * fTotalSize);
	m_iTexPosY = int(PosV  * fTotalSize);

	int StartPosX = int(( (m_iTexPosX - EditSize) < 0 )? 0 : m_iTexPosX - EditSize);
	int StartPosY = int(( (m_iTexPosY - EditSize) < 0 )? 0 : m_iTexPosY - EditSize);
	int EndPosX   = int(( (m_iTexPosX + EditSize) >= fTotalSize )? fTotalSize - 1: m_iTexPosX + EditSize);
	int EndPosY   = int(( (m_iTexPosY + EditSize) >= fTotalSize )? fTotalSize - 1: m_iTexPosY + EditSize);

	DWORD dwChangeColor = 0x00;
	float Smooth		= 0.0f;
	DWORD dwA			= 0x00;

	D3DLOCKED_RECT		AlphaMap_Locked;

	memset( &AlphaMap_Locked, 0, sizeof(D3DLOCKED_RECT) );

	if( FAILED(m_pFilterTexture->LockRect(m_iFilterCnt, &AlphaMap_Locked, NULL, 0)) )
	{
		MessageBox(NULL, L"alphaTexture Lock error", L"Error", MB_OK);
		return E_FAIL;
	}


	LPBYTE pDataDST = (LPBYTE) AlphaMap_Locked.pBits;

	for( int j=StartPosY; j<=EndPosY; ++j )
	{
		LPDWORD pDWordDST = (LPDWORD) (pDataDST + j * AlphaMap_Locked.Pitch);

		for( int i=StartPosX; i<=EndPosX; ++i )
		{
			D3DXVECTOR3 Pix = D3DXVECTOR3( i * PixSize, 0.0f, j * PixSize ) -
				D3DXVECTOR3( m_iTexPosX * PixSize, 0.0f, m_iTexPosY * PixSize ); 

			float Len = D3DXVec3Length( &Pix );

			if( Len <= fHalfSmoothSize )
			{
				dwChangeColor = 0xFFFFFFFF;
			}
			else if(  Len <= fHalfBrushSize  )
			{
				// 최대사이즈에 포함될 우 최대사이즈까지의
				// 선영보간한 알파값을 구한다.
				Len  -= fHalfSmoothSize;
				Smooth = fHalfBrushSize - fHalfSmoothSize;
				dwChangeColor= DWORD(( Smooth - Len  ) / Smooth  * 0xFFFFFFFF);
				D3DXCOLOR dwcc = dwChangeColor;
				dwA = (dwChangeColor & 0xFF000000) >> 24;
				D3DXCOLOR dwaa = dwA;
				dwChangeColor = (dwA << 24) | (dwA << 16) | (dwA << 8) | dwA;
				dwcc = dwChangeColor;
				if(dwcc.a < 1.f)
					dwcc.a = 1.f;
				dwChangeColor = dwcc;
			}
			else
			{
				continue;
			}

			*(pDWordDST + i) = ( *(pDWordDST + i) < dwChangeColor ) ? dwChangeColor : *(pDWordDST + i);
			D3DXCOLOR temp = *(pDWordDST + i);
		}
	}

	if( FAILED(m_pFilterTexture->UnLockRect(m_iFilterCnt)) )	
	{
		MessageBox(NULL, L"alphaTexture UnLock error", L"Error", MB_OK);
		return E_FAIL;
	}




	m_SaveAlphaTexture = m_pFilterTexture->GetTexture(m_iFilterCnt);

	D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeightsave.png", D3DXIFF_PNG, *m_SaveAlphaTexture, NULL);



	return S_OK;
}

HRESULT CTerrain::CreateAlphaMaps()
{
	float		fTotalSize = float(m_iCntX +m_iCntZ);
	HRESULT hr = NULL;


	m_SaveAlphaTexture = m_pFilterTexture->GetTexture(m_iFilterCnt);
	for( int k=0 ; k< 1/*이거 타일리스트에서 받아오게끔 변경해야함*/ ; ++k)
	{
		if( hr =  FAILED(m_pDevice->CreateTexture( (UINT)fTotalSize, (UINT)fTotalSize, 1, 0, D3DFMT_A8R8G8B8, 
			D3DPOOL_MANAGED, ((LPDIRECT3DTEXTURE9*)(&m_SaveAlphaTexture)), NULL)))
			return MessageBox(NULL, L"alphaTexture make error", L"Error", MB_OK);

		D3DLOCKED_RECT		AlphaMap_Locked;

		memset( &AlphaMap_Locked, 0, sizeof(D3DLOCKED_RECT) );

		if( FAILED(m_pFilterTexture->LockRect(m_iFilterCnt, &AlphaMap_Locked, NULL, 0)) )
			return MessageBox(NULL, L"alphaTexture Lock error", L"Error", MB_OK);

		LPBYTE pDataDST = (LPBYTE) AlphaMap_Locked.pBits;

		for( int j=0; j<fTotalSize; ++j )
		{
			LPDWORD pDWordDST = (LPDWORD) (pDataDST + j * AlphaMap_Locked.Pitch);

			for( int i=0; i<fTotalSize; ++i )
			{
				*(pDWordDST + i) = 0xFFFFFFFF;
			}
		}

		if( FAILED(m_pFilterTexture->UnLockRect(m_iFilterCnt)) )	
			return MessageBox(NULL, L"alphaTexture UnLock error", L"Error", MB_OK);
	}


	return S_OK;
}

HRESULT CTerrain::ResetAlphaMaps()
{
	float		fTotalSize = float(m_iCntX +m_iCntZ);
	HRESULT hr = NULL;
	IDirect3DTexture9* m_ResetTexture;


	for( int i=0 ; i< 1/*이거 타일리스트에서 받아오게끔 변경해야함*/ ; ++i)
	{
		if( hr =  FAILED(m_pDevice->CreateTexture( (UINT)m_iCntX, (UINT)m_iCntZ, 1, 0, D3DFMT_A8R8G8B8, 
			D3DPOOL_MANAGED, &m_ResetTexture, NULL)))
			return MessageBox(NULL, L"emptytexture make error", L"Error", MB_OK);

		D3DLOCKED_RECT		AlphaMap_Locked;

		memset( &AlphaMap_Locked, 0, sizeof(D3DLOCKED_RECT) );

		if( FAILED(m_ResetTexture->LockRect(0, &AlphaMap_Locked, NULL, 0)) )
			return MessageBox(NULL, L"emptytexture Lock error", L"Error", MB_OK);

		LPBYTE pDataDST = (LPBYTE) AlphaMap_Locked.pBits;

		for( int j=0; j<m_iCntX; ++j )
		{
			LPDWORD pDWordDST = (LPDWORD) (pDataDST + j * AlphaMap_Locked.Pitch);

			for( int i=0; i<m_iCntZ; ++i )
			{
				*(pDWordDST + i) = 0xFFFFFFFF;
			}
		}

		if( FAILED(m_ResetTexture->UnlockRect(0)) )	
			return MessageBox(NULL, L"emptytexture UnLock error", L"Error", MB_OK);
	}

		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight0.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight1.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight2.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight3.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight4.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight5.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeight6.png", D3DXIFF_PNG, m_ResetTexture, NULL);
		D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/ColorHeightSave.png", D3DXIFF_PNG, m_ResetTexture, NULL);


		


		//for( int k=0 ; k< 7/*이거 타일리스트에서 받아오게끔 변경해야함*/ ; ++k)
		//{
		//	m_SaveAlphaTexture = m_pFilterTexture->GetTexture(k);
		//	if( hr =  FAILED(m_pDevice->CreateTexture( (UINT)fTotalSize, (UINT)fTotalSize, 1, 0, D3DFMT_A8R8G8B8, 
		//		D3DPOOL_MANAGED, ((LPDIRECT3DTEXTURE9*)(&m_SaveAlphaTexture)), NULL)))
		//		return MessageBox(NULL, L"alphaTexture make error", L"Error", MB_OK);

		//	D3DLOCKED_RECT		AlphaMap_Locked;

		//	memset( &AlphaMap_Locked, 0, sizeof(D3DLOCKED_RECT) );

		//	if( FAILED(m_pFilterTexture->LockRect(k, &AlphaMap_Locked, NULL, 0)) )
		//		return MessageBox(NULL, L"alphaTexture Lock error", L"Error", MB_OK);

		//	LPBYTE pDataDST = (LPBYTE) AlphaMap_Locked.pBits;

		//	for( int j=0; j<fTotalSize; ++j )
		//	{
		//		LPDWORD pDWordDST = (LPDWORD) (pDataDST + j * AlphaMap_Locked.Pitch);

		//		for( int i=0; i<fTotalSize; ++i )
		//		{
		//			*(pDWordDST + i) = 0xFFFFFFFF;
		//		}
		//	}

		//	if( FAILED(m_pFilterTexture->UnLockRect(k)) )	
		//		return MessageBox(NULL, L"alphaTexture UnLock error", L"Error", MB_OK);
		//}


	return S_OK;
}

HRESULT CTerrain::DeleteAlphaMaps()
{
	float		fTotalSize = float(m_iCntX +m_iCntZ);
	HRESULT hr = NULL;

	int iCnt = m_iFilterCnt - 1;

	m_SaveAlphaTexture = m_pFilterTexture->GetTexture(iCnt);
	for( int k=0 ; k< 1/*이거 타일리스트에서 받아오게끔 변경해야함*/ ; ++k)
	{
		if( hr =  FAILED(m_pDevice->CreateTexture( (UINT)fTotalSize, (UINT)fTotalSize, 1, 0, D3DFMT_A8R8G8B8, 
			D3DPOOL_MANAGED, ((LPDIRECT3DTEXTURE9*)(&m_SaveAlphaTexture)), NULL)))
			return MessageBox(NULL, L"alphaTexture make error", L"Error", MB_OK);

		D3DLOCKED_RECT		AlphaMap_Locked;

		memset( &AlphaMap_Locked, 0, sizeof(D3DLOCKED_RECT) );

		if( FAILED(m_pFilterTexture->LockRect(iCnt, &AlphaMap_Locked, NULL, 0)) )
			return MessageBox(NULL, L"alphaTexture Lock error", L"Error", MB_OK);

		LPBYTE pDataDST = (LPBYTE) AlphaMap_Locked.pBits;

		for( int j=0; j<fTotalSize; ++j )
		{
			LPDWORD pDWordDST = (LPDWORD) (pDataDST + j * AlphaMap_Locked.Pitch);

			for( int i=0; i<fTotalSize; ++i )
			{
				*(pDWordDST + i) = 0xFFFFFFFF;
			}
		}

		if( FAILED(m_pFilterTexture->UnLockRect(iCnt)) )	
			return MessageBox(NULL, L"alphaTexture UnLock error", L"Error", MB_OK);
	}


	return S_OK;
}

