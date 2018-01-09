#include "Material.h"


Engine::CMaterial::CMaterial(void)
{

}

Engine::CMaterial::~CMaterial(void)
{

}

const D3DMATERIAL9* Engine::CMaterial::Get_Material(void)
{
	return &m_MtrlInfo;
}

HRESULT Engine::CMaterial::Ready_Material(const D3DMATERIAL9* pMaterial)
{
	memcpy(&m_MtrlInfo, pMaterial, sizeof(D3DMATERIAL9));

	return S_OK;
}

HRESULT Engine::CMaterial::Ready_Material(const D3DXCOLOR& Diffuse, const D3DXCOLOR& Ambient , const D3DXCOLOR& Specular, const D3DXCOLOR& Emissive, const _float& fPower)
{
	m_MtrlInfo.Diffuse = Diffuse;
	m_MtrlInfo.Ambient = Ambient;
	m_MtrlInfo.Specular = Specular;
	m_MtrlInfo.Emissive = Emissive;
	m_MtrlInfo.Power = fPower;

	return S_OK;
}

Engine::CMaterial* Engine::CMaterial::Create(const D3DMATERIAL9* pMaterial)
{
	CMaterial*		pInstance = new CMaterial;

	if(FAILED(pInstance->Ready_Material(pMaterial)))
	{
		MSG_BOX(L"CMaterial Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CMaterial* Engine::CMaterial::Create(const D3DXCOLOR& Diffuse, const D3DXCOLOR& Ambient , const D3DXCOLOR& Specular, const D3DXCOLOR& Emissive, const _float& fPower)
{
	CMaterial*		pInstance = new CMaterial;

	if(FAILED(pInstance->Ready_Material(Diffuse, Ambient, Specular, Emissive, fPower)))
	{
		MSG_BOX(L"CMaterial Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CMaterial::Free(void)
{
	
}

