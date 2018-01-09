#include "stdafx.h"
#include "Trail_Effect.h"

#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "Export_Function.h"
#include "Sword.h"
#include "Scene.h"
#include "Sphere.h"
#include "Stage.h"


CTrail_Effect::CTrail_Effect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pShader(NULL)
, m_fFrame(0.f)
, m_fTime(0.f)
, m_fTimeLast(0.f)
, m_eType(TRAIL_NONE)
, m_bFree(false)
, m_fFreeTime(0.f)
, m_fTimeDelta(0.f)
{
	m_htComponent.Ready_Table(19);
	ZeroMemory(&m_tMtrl, sizeof(D3DMATERIAL9));
}

CTrail_Effect::~CTrail_Effect(void)
{

}

HRESULT CTrail_Effect::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	/*m_Mtrl.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	m_Mtrl.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);*/

	for(int i = 0; i < LINECOUNT + 1; ++i)
	{
		m_vTopPoint[i] = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vBotPoint[i] = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	m_bFree = true;
	

	return S_OK;
}

_int CTrail_Effect::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if(m_eTrailState == STATE_START)
	{
		Compute_ViewZ(&m_pInfo->m_vPos);

		Engine::Add_RenderGroup(Engine::CRenderer::TYPE_ALPHA, this);

		m_fTime += fTimeDelta;
		m_fTimeLast += fTimeDelta;

		if(m_fTimeLast > 0.01f)
		{
			m_fTimeLast = 0.f;
			//	if(m_eType == TRAIL_SWORD)
			//	{
			m_vTopPoint[LINECOUNT] = ((CEquipment*)m_pTarget)->GetTrailPos(0);
			m_vBotPoint[LINECOUNT] = ((CEquipment*)m_pTarget)->GetTrailPos(1);
			//	}
			/*	else if(m_eType == TRAIL_MACE)
			{
			m_vTopPoint[LINECOUNT] = ((CMace*)m_pTarget)->GetTrailPos(0);
			m_vBotPoint[LINECOUNT] = ((CMace*)m_pTarget)->GetTrailPos(1);
			}*/
		}

		if(m_fTime > 0.001f)
		{
			m_fTime = 0.f;
			SetCatmullRom();
		}

		
	}
	_int iResult = Engine::CGameObject::Update(fTimeDelta);

	return iResult;
}

void CTrail_Effect::Render(void)
{
	if(m_bFree)
	{
		if(m_fFreeTime > 0.35f)
		{
			m_bFree = false;
			m_fFreeTime = 0.f;
		}
		else
		{
			m_fFreeTime += m_fTimeDelta;
			return;
		}
	}

	if(m_eTrailState == STATE_START)
	{


		LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		pEffect->AddRef();

		pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

		_matrix			matView, matProj;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);	

		m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", 0);
		//m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", _ushort(m_fFrame));
		Engine::SetUp_TargetOnShader(pEffect, L"Target_Depth", "g_DepthTexture");

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
	}
}

HRESULT CTrail_Effect::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer

	if(m_eType == TRAIL_SWORD)
		pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Trail_Sword");
	else if(m_eType == TRAIL_MACE)
		pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Trail_Mace");

	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_REMAIN, L"Texture_Trail");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Effect");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	return S_OK;
}

CTrail_Effect* CTrail_Effect::Create(LPDIRECT3DDEVICE9 pDevice, TRAIL_TYPE eType)
{
	CTrail_Effect*		pGameObject = new CTrail_Effect(pDevice);

	((CTrail_Effect*)pGameObject)->SetType(eType);

	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CTrail_Effect::Free(void)
{
	Engine::CGameObject::Free();
}

void CTrail_Effect::SetCatmullRom()
{
	//m_pInfo->m_vPos		= ((CPlayer*)m_pTarget)->GetPos();
	//m_pInfo->m_vPos		-= ((CPlayer*)m_pTarget)->GetInfo()->m_vDir * 4.f;
	//m_pInfo->m_vPos.y	= 1.f;	

	//D3DXVECTOR3		vRight;			

	//memcpy(&vRight, &((CPlayer*)m_pTarget)->GetInfo()->m_matWorld.m[2][0], sizeof(D3DXVECTOR3));
	//D3DXVec3Normalize(&vRight, &vRight);

	//// m_vPoint와 m_vSubPoint 에  받은 무기의 포스를 넣어준다	

	for(int i = 0; i < LINECOUNT; ++i)
	{
		if(i == LINECOUNT - 1)
		{
			m_vTopPoint[i] = m_vTopPoint[i + 1];
			m_vBotPoint[i] = m_vBotPoint[i + 1];
		
		}
		else
		{
			m_vTopPoint[i] = m_vTopPoint[i + 1];
			m_vBotPoint[i] = m_vBotPoint[i + 1];
		}
	}

	//// m_pBuffer에 버텍스버퍼를 락 언락 ^^

	

	////반복문을 돈다 총 점의 갯수 나누기 곡선의 갯수!!! 
	//// 가중치는 구해주는데 이제 한 라인에 나눠질 점의 갯수로 i를 나눠준다 

	//// 가중치는 0부터 1까지 구요

	//// 캣멀롬 함수를 통해 보간된 점을 구했으면 pVertex에 차곡 차곡 넣어준다!

	//// 마지막으로 언락을 하면 매무리!!!!

	////캬 ~~  스플라인 곡선 완료!!!


	



	
	Engine::VTXTEX* pVertex = NULL;

	(*m_pBuffer->GetVBInfo())->Lock(0, 0, (void**)&pVertex, 0);

	//(*m_pBuffer->GetVtxInfo())->Lock(0, 0, (void**)&pVertex, 0);

	for ( int iPoint = 0; iPoint < POINTCOUNT; iPoint++)
	{
		float S =  (float)iPoint / POINTCOUNT;

		for(int iLine = 0; iLine < LINECOUNT * 2; ++iLine)
		{			
			int iIndex = iLine;

			if(iIndex >= LINECOUNT)
				iIndex -= LINECOUNT;

			int iBeforeIndex = iIndex - 1;
			int	iNextIndex = iIndex + 1;
			int	iNNextIndex = iIndex + 2;

			if(iBeforeIndex < 0)
				iBeforeIndex = 0;

			if(iNNextIndex > LINECOUNT)
				iNNextIndex = LINECOUNT;
			
			if(iLine < LINECOUNT)
				D3DXVec3CatmullRom( &pVertex[iPoint + (POINTCOUNT * iLine)].vPos, &m_vTopPoint[iBeforeIndex], &m_vTopPoint[iIndex],& m_vTopPoint[iNextIndex], &m_vTopPoint[iNNextIndex], S );
			else
				D3DXVec3CatmullRom( &pVertex[iPoint + (POINTCOUNT * iLine)].vPos, &m_vBotPoint[iBeforeIndex], &m_vBotPoint[iIndex],& m_vBotPoint[iNextIndex], &m_vBotPoint[iNNextIndex], S );

			//pVertex[iPoint + (POINTCOUNT * iLine)].vPos = m_vCatPoint[i];
		}
	}

	(*m_pBuffer->GetVBInfo())->Unlock();



	//	pVertex[i].vPos		= ((Engine::CTrail*)m_pBuffer)->m_vRomLine[i];

	//	pVertex[i+(LINESIZE/3)].vPos		= ((Engine::CTrail*)m_pBuffer)->m_vRomLine[i+(LINESIZE/3)];	

	//	pVertex[i+(LINESIZE/3) * 2].vPos		= ((Engine::CTrail*)m_pBuffer)->m_vRomLine[i+(LINESIZE/3) * 2];	

	//	pVertex[i+(LINESIZE/3) * 3].vPos		= ((Engine::CTrail*)m_pBuffer)->m_vRomLine[i+(LINESIZE/3) * 3];

	//	pVertex[i+(LINESIZE/3) * 4].vPos		= ((Engine::CTrail*)m_pBuffer)->m_vRomLine[i+(LINESIZE/3) * 4];		

	//	pVertex[i+(LINESIZE/3) * 5].vPos		= ((Engine::CTrail*)m_pBuffer)->m_vRomLine[i+(LINESIZE/3) * 5];		
	//}	

	//(*m_pBuffer->GetVBInfo())->Unlock();
}

void CTrail_Effect::SetTarget(CEquipment* pTarget)
{
	m_pTarget = pTarget;
}

void CTrail_Effect::SetType(TRAIL_TYPE eType)
{
	m_eType = eType;
}

