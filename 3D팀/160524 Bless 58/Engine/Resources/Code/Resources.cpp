#include "Resources.h"

Engine::CResources::CResources(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

Engine::CResources::CResources(const CResources& rhs)
: m_pDevice(rhs.m_pDevice)
{
	m_pDevice->AddRef();
}

Engine::CResources::~CResources(void)
{

}

void Engine::CResources::Free(void)
{
	Engine::Safe_Release(m_pDevice);
}