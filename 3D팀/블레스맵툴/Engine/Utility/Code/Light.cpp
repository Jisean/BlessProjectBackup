#include "Light.h"

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pVB(NULL)
, m_pIB(NULL)
{

	ZeroMemory(&m_LightInfo, sizeof(D3DLIGHT9));
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Init_Light(const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx)
{
	memcpy(&m_LightInfo, pLightInfo, sizeof(D3DLIGHT9));

	D3DVIEWPORT9			ViewPort;	
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pDevice->GetViewport(&ViewPort);

	if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXFVF_SCREEN, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR4((float)ViewPort.Width, 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR4((float)ViewPort.Width, (float)ViewPort.Height, 0.f, 1.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR4(0.f, (float)ViewPort.Height, 0.f, 1.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);	

	m_pVB->Unlock();


	if(FAILED(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void Engine::CLight::Render_Light(LPD3DXEFFECT pEffect)
{
	_uint	iPassIdx = 0;

	if(m_LightInfo.Type == D3DLIGHT_DIRECTIONAL)
	{
		iPassIdx = 0;
		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.f));
	}

	if(m_LightInfo.Type == D3DLIGHT_POINT)
	{
		iPassIdx = 1;
		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
	}

	pEffect->SetVector("g_vLightDiffuse", ((_vec4*)&m_LightInfo.Diffuse));
	pEffect->SetVector("g_vLightAmbient", ((_vec4*)&m_LightInfo.Ambient));	

	_vec3		vCamPos;
	_matrix		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetVector("g_vCamPosition", &_vec4(vCamPos, 1.f));
	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);
	pEffect->SetFloat("g_fPower", 20.f);

	pEffect->BeginPass(iPassIdx);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pDevice->SetFVF(VTXFVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pDevice , const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx)
{
	CLight*			pLight = new CLight(pDevice);
	if(FAILED(pLight->Init_Light(pLightInfo, dwLightIdx)))
		Engine::Safe_Release(pLight);

	return pLight;
}

void Engine::CLight::Free(void)
{
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}

