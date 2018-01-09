#include "stdafx.h"
#include "SaveQMark.h"
#include "Renderer.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "MainPlayer.h"
#include "NPCs.h"
#include "AIMgr.h"
#include "BossQStart.h"

CSaveQMark::CSaveQMark(LPDIRECT3DDEVICE9 pDevice)
: CUI(pDevice)
, m_fSpeed(0.05f)
{

}

CSaveQMark::~CSaveQMark(void)
{

}

HRESULT CSaveQMark::Initialize(void)
{
	for(int i = 0; i < QUEST_END; ++i)
	{
		m_fAlpha[i] = 1.f;
		m_bDown[i] = false;
		m_vPos[i] = _myvec(0.f, 0.f, 0.f);
	}

	if(FAILED(AddComponent()))
		return E_FAIL;

	m_fSizeX = 30.0f / 5.f;//2.8f;   
	m_fSizeY = 24.0f / 5.f;//2.8f;

	m_fX = 400.f;
	m_fY = 150.f;

	m_fViewZ = 1;

	m_fMoveY[QUEST_1] = 0.0f;
	m_fMoveY[QUEST_2] = 0.02f;
	m_fMoveY[QUEST_3] = 0.03f;
	m_fMoveY[QUEST_4] = 0.04f;
	m_fMoveY[QUEST_5] = 0.05f;

	return S_OK;
}

_int CSaveQMark::Update(const _float& fTimeDelta)
{
	if(((CBossQStart*)(Engine::Get_ObjList(LAYER_UI, L"BossQStart")->front()))->Get_NewQuest() == false)
		return 0;

	vector<CNPCs*>::iterator iternpc = CAIMgr::GetInstance()->Get_NPCInfo()->begin();
	vector<CNPCs*>::iterator iternpc_end = CAIMgr::GetInstance()->Get_NPCInfo()->end();

	int iNum = 0;
	for(iternpc; iternpc < iternpc_end; ++iternpc, ++iNum)
	{
		m_vPos[(eQuest)iNum] = (*iternpc)->GetInfo()->m_vPos;
	}



	float fTime = fTimeDelta * m_fSpeed;
	for(int i = 0; i < QUEST_END; ++i)
	{
		if(m_bDown[i])
		{
			if(m_fMoveY[i] - fTime < 0.0f)
				m_bDown[i] = false;
			else
				m_fMoveY[i] -= fTime;
		}
		else
		{
			if(m_fMoveY[i] + fTime > 0.05f)
				m_bDown[i] = true;
			else
				m_fMoveY[i] += fTime;
		}

	}

	CUI::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	//m_pInfo->m_vPos = m_vPos;


	return 0;
}

void CSaveQMark::Render(void)
{
	CMainPlayer* pMainPlayer = ((CMainPlayer*)(Engine::Get_ObjList(LAYER_GAMELOGIC, L"Player")->front()));
	if(pMainPlayer == NULL)
		return;

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	//CSaveQMark::Set_ConstantTable(pEffect);

	for(int i = 0; i < QUEST_END; ++i)
	{
		_myvec pPlayerPos = pMainPlayer->Get_TargetPlayer()->GetInfo()->m_vPos;
		float fLength = D3DXVec3Length(&(m_vPos[i] - pPlayerPos));
		if(fLength > 15.f)
			continue;


		pEffect->SetTechnique("Default_Device");

		_matrix			matProj, matView2, matWorld2;

		m_pDevice->GetTransform(D3DTS_VIEW, &matView2);
		m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		D3DXMatrixIdentity(&matWorld2);

		pEffect->SetFloat("g_fAlpha", m_fAlpha[i]);
		pEffect->SetFloat("g_fPointPower", 1.f);


		_mymat matScale, matRotX, matRotY, matRotZ, matTrans, matParent;

		matScale.Scale(D3DXVECTOR3(0.06f, 0.06f , 0.06f));

		matRotX.RotX(D3DXToRadian(0.f));
		matRotY.RotY(D3DXToRadian(0.f));
		matRotZ.RotZ(D3DXToRadian(0.f));


		matTrans.Trans(D3DXVECTOR3(0.f, 0.4f + m_fMoveY[i], 0.f));
		matParent.Trans(m_vPos[i]);

		D3DXMATRIX      matBill;
		D3DXMatrixIdentity(&matBill);

		m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
		ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

		D3DXMatrixInverse(&matBill, 0, &matBill);

		matParent = matBill * matParent;
		matWorld2 = matScale * matRotX * matRotY * matRotZ * matTrans * matParent;

		pEffect->SetMatrix("g_matWorld", &matWorld2);
		pEffect->SetMatrix("g_matView", &matView2);
		pEffect->SetMatrix("g_matProj", &matProj);

		//pEffect->SetFloat("g_fDrawRatio", m_fDrawRatio);
		m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture");	
		_uint		iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		pEffect->EndPass();
		pEffect->End();	
	}


	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CSaveQMark* CSaveQMark::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSaveQMark*		pInstance = new CSaveQMark(pDevice);

	if(FAILED(pInstance->Initialize()))
	{
		//MSG_BOX("CSaveQMark Created Failed");
		//::safe_release(pInstance);
	}

	return pInstance;
}

HRESULT CSaveQMark::AddComponent(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	// For.Buffer Component
	pComponent = m_pBuffer = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_REMAIN, L"Buffer_RcTex");
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Buffer", pComponent);

	// For.Texture Component
	pComponent = m_pTexture = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_UI, L"Texture_Quest_MainWidow_QuestMark"); 
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Texture", pComponent);

	// For.Shader Component
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader",pComponent);

	return S_OK;
}

void CSaveQMark::Free(void)
{
	CUI::Free();
}
