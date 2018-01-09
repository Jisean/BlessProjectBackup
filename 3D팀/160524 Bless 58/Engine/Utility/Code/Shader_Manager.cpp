#include "Shader_Manager.h"
#include "Shader.h"

USING(Engine)
IMPLEMENT_SINGLETON(CShader_Manager)

Engine::CShader_Manager::CShader_Manager(void)
{
	m_htShader.Ready_Table(/*HASH_SINGLE,*/ 29);
}

Engine::CShader_Manager::~CShader_Manager(void)
{

}

CShader* Engine::CShader_Manager::Clone_Shader(const TCHAR* pShaderTag)
{
	CShader* pShader = Find_Shader(pShaderTag);

	NULL_CHECK_RETURN(pShader, NULL);

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

	m_htShader.Insert_Table(pShaderTag, pShader);

	return S_OK;
}

CShader* Engine::CShader_Manager::Find_Shader(const TCHAR* pShaderTag)
{
	CShader* pShader = (CShader*)(m_htShader.Search_TableData(pShaderTag));

	return pShader;
}

void Engine::CShader_Manager::Free(void)
{
	size_t iTableSize =	m_htShader.Get_TableSize();
	for(size_t j = 0; j < iTableSize; ++j)
	{
		CShader*		pResource = (CShader*)m_htShader.Erase_Slot(j);
		if(NULL != pResource)
			Safe_Release(pResource);
	}
	m_htShader.Erase_Table();
}


