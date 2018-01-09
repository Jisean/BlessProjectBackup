#include "Shader_Manager.h"
#include "Shader.h"

USING(Engine)
IMPLEMENT_SINGLETON(CShader_Manager)

Engine::CShader_Manager::CShader_Manager(void)
{

}

Engine::CShader_Manager::~CShader_Manager(void)
{

}

CShader* Engine::CShader_Manager::Clone_Shader(const TCHAR* pShaderTag)
{
	CShader* pShader = Find_Shader(pShaderTag);

	if(NULL == pShader)
		return NULL;


	return pShader->Clone_Shader();
}

LPD3DXEFFECT Engine::CShader_Manager::Get_EffectHandle(const TCHAR* pShaderTag)
{
	CShader* pShader = Find_Shader(pShaderTag);

	if(NULL == pShader)
		return NULL;
	
	return pShader->Get_EffectHandle();
}

HRESULT Engine::CShader_Manager::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFileName)
{
	CShader*		pShader = Find_Shader(pShaderTag);

	if(NULL != pShader)
		return E_FAIL;

	pShader = CShader::Create(pGraphicDev, pFileName);
	if(NULL == pShader)
		return E_FAIL;

	m_mapShader.insert(MAPSHADER::value_type(pShaderTag, pShader));

	return S_OK;
}

CShader* Engine::CShader_Manager::Find_Shader(const TCHAR* pShaderTag)
{
	MAPSHADER::iterator	iter = find_if(m_mapShader.begin(), m_mapShader.end(), CTagFinder(pShaderTag));

	if(iter == m_mapShader.end())
		return NULL;


	return iter->second;
}

void Engine::CShader_Manager::Free(void)
{
	for_each(m_mapShader.begin(), m_mapShader.end(), CRelease_Pair());
	m_mapShader.clear();
}

