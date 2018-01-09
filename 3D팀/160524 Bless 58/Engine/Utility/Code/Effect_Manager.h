/*!
 * \file Particle_Manager.h
 * \date 2016/05/07 14:45
 *
 * \author 세웅	
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note 파티클 통괄 관리 매니저
*/
#ifndef Effect_Manager_h__
#define Effect_Manager_h__

#include "Engine_Include.h"
#include "Particle_System.h"
#include "Mesh_Effect.h"
#include "Base.h"

// 생성자
// 소멸자
// public 변수
// public 함수
// private 변수
// private 함수
// Create()
// Free()


BEGIN(Engine)

class ENGINE_DLL CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

private:
	explicit CEffect_Manager(void);
	virtual ~CEffect_Manager(void);

public:
	int	Call_GuardianFX(GUARDIAN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ);
	int	Call_RangerFX(RANGER_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ);
	int	Call_PaladinFX(PALADIN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ);
	int	Call_MagicianFX(MAGICIAN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ);
	int	Call_MonsterFX(MONSTER_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ);
	int	Call_EtcFX(ETC_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ, D3DXVECTOR3 vScale);
	int	Call_TrailFX(TRAIL_FX eType);

	void	Off_GuardianFX(GUARDIAN_FX eType, int iNumber = 0);
	void	Off_RangerFX(RANGER_FX eType, int iNumber = 0);
	void	Off_PaladinFX(PALADIN_FX eType, int iNumber = 0);
	void	Off_MagicianFX(MAGICIAN_FX eType, int iNumber = 0);
	void	Off_MonsterFX(MONSTER_FX eType, int iNumber = 0);
	void	Off_EtcFX(ETC_FX eType, int iNumber = 0);
	void	Off_TrailFX(TRAIL_FX eType, int iNumber = 0);

	void	ShieldCounting();
public:
	CParticle_System* CreateParticle(LPDIRECT3DDEVICE9 pGraphicDev, const PARTICLEINFO* pParticleInfo);
	CMesh_Effect* CreateMeshEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	
	vector<CMesh_Effect*>*	GetGuardianFX(GUARDIAN_FX eType);
	vector<CMesh_Effect*>*	GetRangerFX(RANGER_FX eType);
	vector<CMesh_Effect*>*	GetPaladinFX(PALADIN_FX eType);
	vector<CMesh_Effect*>*	GetMagicianFX(MAGICIAN_FX eType);
	vector<CMesh_Effect*>*	GetMonsterFX(MONSTER_FX eType);
	vector<CMesh_Effect*>*	GetEtcFX(ETC_FX eType);
	vector<CGameObject*>*	GetTrailFX(TRAIL_FX eType);


	bool GetMagicianCalling(MAGICIAN_FX eType, int iIndex = 0);

private:
	vector<CMesh_Effect*>	m_GuardianFXList[G_END];
	vector<CMesh_Effect*>	m_RangerFXList[R_END];
	vector<CMesh_Effect*>	m_PaladinFXList[P_END];
	vector<CMesh_Effect*>	m_MagicianFXList[M_END];
	vector<CMesh_Effect*>	m_MonsterFXList[MONSTER_END];
	vector<CMesh_Effect*>	m_EtcFXList[ETC_END];

	vector<CGameObject*>	m_TrailFXList[TRAIL_END];

public:
	virtual void Free(void)	{};
};

END


#endif // Effect_Manager_h__