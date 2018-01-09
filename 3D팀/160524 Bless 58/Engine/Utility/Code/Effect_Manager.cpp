#include "Effect_Manager.h"
#include <iostream>
USING(Engine)
IMPLEMENT_SINGLETON(CEffect_Manager)


Engine::CEffect_Manager::CEffect_Manager(void)
{

}


Engine::CEffect_Manager::~CEffect_Manager(void)
{

}



CParticle_System* Engine::CEffect_Manager::CreateParticle(LPDIRECT3DDEVICE9 pGraphicDev, const PARTICLEINFO* pParticleInfo)
{	
	CParticle_System* pParticle = CParticle_System::Create(pGraphicDev, pParticleInfo);

	//NULL_CHECK_RETURN(pParticle, NULL);	

	return pParticle;
}

CMesh_Effect* Engine::CEffect_Manager::CreateMeshEffect(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMesh_Effect* pMeshEffect = CMesh_Effect::Create(pGraphicDev);

	return pMeshEffect;
}

int Engine::CEffect_Manager::Call_GuardianFX(GUARDIAN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	vector<CMesh_Effect*>::iterator iter = m_GuardianFXList[eType].begin();
	vector<CMesh_Effect*>::iterator iter_end = m_GuardianFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(false == (*iter)->GetCalling())
		{
			(*iter)->Call_Effect(vPos, bChase, pAngleX, pAngleY, pAngleZ);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}

int Engine::CEffect_Manager::Call_RangerFX(RANGER_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	vector<CMesh_Effect*>::iterator iter = m_RangerFXList[eType].begin();
	vector<CMesh_Effect*>::iterator iter_end = m_RangerFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(false == (*iter)->GetCalling())
		{
			(*iter)->Call_Effect(vPos, bChase, pAngleX, pAngleY, pAngleZ);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}

int Engine::CEffect_Manager::Call_PaladinFX(PALADIN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	vector<CMesh_Effect*>::iterator iter = m_PaladinFXList[eType].begin();
	vector<CMesh_Effect*>::iterator iter_end = m_PaladinFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(false == (*iter)->GetCalling())
		{
			(*iter)->Call_Effect(vPos, bChase, pAngleX, pAngleY, pAngleZ);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}

int Engine::CEffect_Manager::Call_MagicianFX(MAGICIAN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	vector<CMesh_Effect*>::iterator iter = m_MagicianFXList[eType].begin();
	vector<CMesh_Effect*>::iterator iter_end = m_MagicianFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(false == (*iter)->GetCalling())
		{
			(*iter)->Call_Effect(vPos, bChase, pAngleX, pAngleY, pAngleZ);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}

int Engine::CEffect_Manager::Call_MonsterFX(MONSTER_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	vector<CMesh_Effect*>::iterator iter = m_MonsterFXList[eType].begin();
	vector<CMesh_Effect*>::iterator iter_end = m_MonsterFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(false == (*iter)->GetCalling())
		{
			(*iter)->Call_Effect(vPos, bChase, pAngleX, pAngleY, pAngleZ);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}


int Engine::CEffect_Manager::Call_EtcFX(ETC_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ, D3DXVECTOR3 vScale)
{
	vector<CMesh_Effect*>::iterator iter = m_EtcFXList[eType].begin();
	vector<CMesh_Effect*>::iterator iter_end = m_EtcFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(false == (*iter)->GetCalling())
		{
			(*iter)->Call_Effect(vPos, bChase, pAngleX, pAngleY, pAngleZ, vScale);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}

int Engine::CEffect_Manager::Call_TrailFX(TRAIL_FX eType)
{
	vector<CGameObject*>::iterator iter = m_TrailFXList[eType].begin();
	vector<CGameObject*>::iterator iter_end = m_TrailFXList[eType].end();

	for(; iter != iter_end; ++iter)
	{
		if(0 == (*iter)->GetTrailState())
		{
			(*iter)->SetTrailState(true);
			return (*iter)->GetNumber();
		}
	}
	return 0;
}

void Engine::CEffect_Manager::ShieldCounting()
{
	m_MonsterFXList[SKULL_SHIELD].front()->ShieldCounting();
}


void Engine::CEffect_Manager::Off_GuardianFX(GUARDIAN_FX eType, int iNumber)
{		
	(m_GuardianFXList[eType])[iNumber]->Off_FX();	
}

void Engine::CEffect_Manager::Off_RangerFX(RANGER_FX eType, int iNumber /*= 0*/)
{
	(m_RangerFXList[eType])[iNumber]->Off_FX();	
}

void Engine::CEffect_Manager::Off_PaladinFX(PALADIN_FX eType, int iNumber /*= 0*/)
{
	(m_PaladinFXList[eType])[iNumber]->Off_FX();	
}

void Engine::CEffect_Manager::Off_MagicianFX(MAGICIAN_FX eType, int iNumber /*= 0*/)
{
	(m_MagicianFXList[eType])[iNumber]->Off_FX();	
}

void Engine::CEffect_Manager::Off_MonsterFX(MONSTER_FX eType, int iNumber /*= 0*/)
{
	(m_MonsterFXList[eType])[iNumber]->Off_FX();
}

void Engine::CEffect_Manager::Off_EtcFX(ETC_FX eType, int iNumber /*= 0*/)
{
	(m_EtcFXList[eType])[iNumber]->Off_FX();
}

void Engine::CEffect_Manager::Off_TrailFX(TRAIL_FX eType, int iNumber /*= 0*/)
{
	(m_TrailFXList[eType])[iNumber]->SetTrailState(false);
}


vector<CMesh_Effect*>* Engine::CEffect_Manager::GetGuardianFX(GUARDIAN_FX eType)
{
	return &m_GuardianFXList[eType];
}

vector<CMesh_Effect*>* Engine::CEffect_Manager::GetRangerFX(RANGER_FX eType)
{
	return &m_RangerFXList[eType];
}

vector<CMesh_Effect*>* CEffect_Manager::GetPaladinFX(PALADIN_FX eType)
{
	return &m_PaladinFXList[eType];
}

vector<CMesh_Effect*>* CEffect_Manager::GetMagicianFX(MAGICIAN_FX eType)
{
	return &m_MagicianFXList[eType];
}

vector<CMesh_Effect*>* Engine::CEffect_Manager::GetMonsterFX(MONSTER_FX eType)
{
	return &m_MonsterFXList[eType];
}

vector<CMesh_Effect*>* Engine::CEffect_Manager::GetEtcFX(ETC_FX eType)
{
	return &m_EtcFXList[eType];
}

vector<CGameObject*>* Engine::CEffect_Manager::GetTrailFX(TRAIL_FX eType)
{
	return &m_TrailFXList[eType];
}

bool Engine::CEffect_Manager::GetMagicianCalling(MAGICIAN_FX eType, int iIndex)
{
	return (m_MagicianFXList[eType])[iIndex]->GetCalling();
}





