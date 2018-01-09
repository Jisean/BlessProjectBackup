#include "stdafx.h"
#include "UI.h"
#include "DmgView.h"
#include "Renderer.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"

CDmgView::CDmgView(LPDIRECT3DDEVICE9 pGraphicDev)
: CUI(pGraphicDev)
, m_vPos(_vec3(0.f,0.f,0.f))
, m_iDmg(0)
, m_iCnt(0)
, m_fAddY(0.f)
, m_fTime(0.f)
, m_fScaleRatio(1.f)
, m_fUpSpeed(0.8f)
, m_fRightSpeed(0.f)
, m_fAlpha(1.f)
, m_fMoveSize(0.f)
{

}

CDmgView::~CDmgView(void)
{

}

HRESULT CDmgView::Initialize(const _vec3* pPos, const _float& fDmg)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 30.0f / 5.f;//2.8f;   
	m_fSizeY = 24.0f / 5.f;//2.8f;

	m_fX = 400.f;
	m_fY = 150.f;

	m_fViewZ = 1;

	m_vPos = *pPos;
	m_vPos.y += 0.1f;
	m_iDmg = _int(fDmg);
	m_fDrawRatio = 1.f;
	_uint		iRand = rand() % 2;
	if(0 == iRand)
		m_fRightSpeed = 0.2f;
	else
		m_fRightSpeed = -0.2f;

	srand(GetTickCount());
	
	// 50까지 소수
	float fTemp = (float)rand() / RAND_MAX * 1000.f;
	fTemp -= (int)fTemp;
	fTemp *= 50.f;
	m_fPosRandX = fTemp - 25.f;


	float fTemp2 = (float)rand() / RAND_MAX * 100000.f;
	fTemp2 -= (int)fTemp2;
	fTemp2 *= 50.f;
	m_fPosRandY = fTemp2 - 25.f;

	m_vPos.x += (m_fPosRandX * 0.01f);
	m_vPos.y += (m_fPosRandY * 0.002f);

	Compute_Cnt();
	return S_OK;
}

_int CDmgView::Update(const _float& fTimeDelta)
{
	CUI::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_UI, this);

	m_fTime += fTimeDelta;

	//m_fAddY = m_fTime * 50;

	if(m_fTime > 1.3f)
		m_bDead = true;

	m_fAlpha = 1.f - (m_fTime / 1.3f);
	//_vec3		vDir = _vec3(0.f, 0.f, 0.f);
	//vDir.x = m_fRightSpeed * fTimeDelta / 10.f;
	//vDir.y = m_fUpSpeed * fTimeDelta / 30.f;
	//m_vPos += vDir;

	m_pInfo->m_vPos = m_vPos;

	m_fUpSpeed -= 1.8f * fTimeDelta;
	
	m_fScaleRatio = 1.0f;
	
	return 0;
}

void CDmgView::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPD3DXEFFECT		pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	//CDmgView::Set_ConstantTable(pEffect);

	_matrix			matProj, matView2;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView2);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);


	_matrix			matWorld2;
	D3DXMatrixIdentity(&matWorld2);

	// 직교투영을 수행해주는 함수다.
	//D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

	//pEffect->SetMatrix("g_matWorld", &matWorld);
	//pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	pEffect->SetFloat("g_fPointPower", 1.f);

	_int	iDmg = m_iDmg;
	_uint	iCnt = m_iCnt / 10;
	while(true)
	{
		_int	iNum;
		if(0 == iDmg)
			iNum = 0;
		else
			iNum = iDmg / iCnt;

		_mymat matScale, matRotX, matRotY, matRotZ, matTrans, matParent;

		matScale.Scale(D3DXVECTOR3(0.03f, 0.03f , 0.03f));

		matRotX.RotX(D3DXToRadian(0.f));
		matRotY.RotY(D3DXToRadian(0.f));
		matRotZ.RotZ(D3DXToRadian(0.f));


		matTrans.Trans(D3DXVECTOR3(m_fMoveSize, 0.005f, 0.f));

		matParent.Trans(m_vPos);


		D3DXMATRIX      matBill;
		D3DXMatrixIdentity(&matBill);

		m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
		ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

		D3DXMatrixInverse(&matBill, 0, &matBill);

		matParent = matBill * matParent;

		matWorld2 = matScale * matRotX * matRotY * matRotZ * matTrans * matParent;

		m_fMoveSize += 0.04f;

		pEffect->SetMatrix("g_matWorld", &matWorld2);
		pEffect->SetMatrix("g_matView", &matView2);
		pEffect->SetMatrix("g_matProj", &matProj);

		//pEffect->SetFloat("g_fDrawRatio", m_fDrawRatio);
		m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture", iNum);	
		_uint		iPassCnt = 0;

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pBuffer->Render(&m_pInfo->m_matWorld);

		pEffect->EndPass();
		pEffect->End();	

		if(1 >= iCnt )
			break;

		iDmg = iDmg - iNum * iCnt;
		iCnt /= 10;

	}	

	m_fMoveSize = 0.f;
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CDmgView* CDmgView::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _float& fDmg)
{
	CDmgView*		pInstance = new CDmgView(pGraphicDev);

	if(FAILED(pInstance->Initialize(pPos, fDmg)))
	{
		//MSG_BOX("CDmgView Created Failed");
		//::safe_release(pInstance);
	}

	return pInstance;
}

HRESULT CDmgView::Add_Component(void)
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
	pComponent = m_pTexture = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_UI, L"Texture_Dmg"); 
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Com_Texture", pComponent);

	// For.Shader Component
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader",pComponent);

	return S_OK;
}

void CDmgView::Set_ConstantTable(LPD3DXEFFECT pEffect)
{

}

void CDmgView::Compute_Cnt(void)
{
	_int		iDmg = m_iDmg;
	_int		iCnt = 1;
	m_iCnt2 = 1;

	while(true)
	{
		if(0 == iDmg / iCnt)
			break;

		++m_iCnt2;
		iCnt *= 10;
	}	
	m_iCnt = iCnt;
	--m_iCnt2;
}

void CDmgView::Free(void)
{
	CUI::Free();
}
