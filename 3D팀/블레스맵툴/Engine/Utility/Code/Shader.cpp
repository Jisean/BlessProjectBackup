#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pEffect(NULL)
, m_pErrBuffer(NULL)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& Instance)
: m_pGraphicDev(Instance.m_pGraphicDev)
, m_pEffect(Instance.m_pEffect)
, m_pErrBuffer(Instance.m_pErrBuffer)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
	if(m_pErrBuffer != NULL)
		m_pErrBuffer->AddRef();
}

Engine::CShader::~CShader(void)
{

}



HRESULT Engine::CShader::Ready_Shader(const TCHAR* pFileName)
{
	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFileName, NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_pEffect, &m_pErrBuffer)))
	{
		MessageBoxA(NULL, (char*)m_pErrBuffer->GetBufferPointer(), NULL, MB_OK);
		return E_FAIL;
	}

	if(NULL != m_pErrBuffer)	
		MessageBoxA(NULL, (char*)m_pErrBuffer->GetBufferPointer(), NULL, MB_OK);
		
	

	return S_OK;
}

CShader* Engine::CShader::Clone_Shader(void)
{
	return new CShader(*this);
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFileName)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if(FAILED(pInstance->Ready_Shader(pFileName)))
	{
		MSG_BOX(L"CShader Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CShader::Free(void)
{	
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pEffect);
	Engine::Safe_Release(m_pErrBuffer);
}

