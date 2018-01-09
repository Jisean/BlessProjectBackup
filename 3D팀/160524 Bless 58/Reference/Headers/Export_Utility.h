/*!
 * \file Export_Utility.h
 * \date 2015/04/03 1:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "LightMgr.h"
#include "Frustum.h"
#include "QuadTree.h"
#include "NaviMgr.h"
#include "CollisionMgr.h"
#include "Shader_Manager.h"
#include "RenderTarget_Manager.h"
#include "Particle_System.h"
#include "Mesh_Effect.h"
#include "Effect_Manager.h"
#include "OcclusionMgr.h"

BEGIN(Engine)


//======================== For.Management =======================//
//--Getter--------------------------------------------------------//
inline const CComponent* Get_Component(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);
inline CScene*	Get_Scene(void);
inline vector<CGameObject*>* Get_ObjList(const WORD LayerID, const TCHAR* pObjKey);

//--Setter--------------------------------------------------------//
inline void Add_RenderGroup(Engine::CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);
inline void Switch_DebugBuffer(bool bOnOff);
inline float*	Set_RendererLightPower(void);
inline float*	SetRendererBloomPower(void);
inline float*	SetRendererBloomRange(void);
inline void		Set_DOFRender(bool bDOF);
inline void		Set_RadialRender(bool bRadial, float fMaxTime = 5.f);
inline void		Set_ShadowRender(bool bShadow);	
inline void		Set_VolumetricRender(bool bVolumetric);
//--General-------------------------------------------------------//
inline HRESULT Ready_Management(LPDIRECT3DDEVICE9 pDevice);
inline _int Update_Management(const _float& fTimeDelta);
inline void Render_Management(void);
inline void Clear_RenderGroup(void);
template <typename T> HRESULT SceneChange(T& Functor);

//======================== For.LightMgr =======================//
//--Getter--------------------------------------------------------//
inline const D3DLIGHT9* Get_LightInfo(const _ulong& dwIndex = 0, TCHAR*	pKeyName = L"Directional");
inline CLight*			Get_Light(TCHAR*	pKeyName, const DWORD& dwLightIdx);
inline int				Get_LightCnt(TCHAR*	pKeyName);
inline size_t			Get_LightKeyCnt(void);
//--Setter--------------------------------------------------------//
inline HRESULT Add_Light(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, TCHAR*	pKeyName);
inline void Set_LightRender(TCHAR*	pKeyName, bool bRender);
inline void	Set_LightRender(UINT	iLightKey, bool bRender);
//--General-------------------------------------------------------//

// =========================== For.Optimazation =========================== //
// For.Getter----------------------------------------------------------------------------------------------------------------------------------------------------------------

// For.Setter----------------------------------------------------------------------------------------------------------------------------------------------------------------

// For.General---------------------------------------------------------------------------------------------------------------------------------------------------------------
inline _bool Decision_InFrustum(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius);
inline _bool Decision_InFrustum_Obj(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius = 0.f);
inline _bool Decision_InFrustum(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition);

inline HRESULT Initialize_QuadTree(const _vec3* pVtxPos, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ);
inline void Decision_InQuadTree(LPDIRECT3DDEVICE9 pGraphicDev, INDEX32* pIndex, _int* pTriCnt);

//======================== For.Occlusion =======================//
//--Getter--------------------------------------------------------//
inline CTransform* Get_OccBox(const _uint iIndex);
inline CCubeColor* Get_OccBoxVtx(const _uint iIndex);
//--Setter--------------------------------------------------------//
inline void Set_OccBox(const _uint iIndex, CTransform* pInfo);
inline HRESULT Add_OccBox(const D3DXMATRIX* pmatWorld);
inline bool Set_InOccBox(CGameObject* pObject);
inline void Set_OccVtxPos(const _uint iIndex);
//--General-------------------------------------------------------//
inline HRESULT Ready_Occlusion(LPDIRECT3DDEVICE9 pDevice);
inline void Render_OccBox(void);
inline int	Test_InOccBox(D3DXVECTOR3 vPos);

//======================== For.NaviMgr =======================//
//--Getter--------------------------------------------------------//
inline DWORD Get_CellIndex(D3DXVECTOR3* pPos);
//--Setter--------------------------------------------------------//
inline void Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize);
inline HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD &dwType);

//--General-------------------------------------------------------//
inline void Link_Cell(void);
inline DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx, float fHeight);
inline void	Render_Navi(void);

//======================== For.CollisionMgr =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline bool Collision_OBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld
						  , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld);

inline bool Collision_AABB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax
						   , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax);

//======================== For.Shader_Manager =======================//
//--Getter--------------------------------------------------------//
inline CShader* Clone_Shader(const TCHAR* pShaderTag);
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//


//======================== For.RenderTarget_Manager =======================//
//--Getter--------------------------------------------------------//
inline void SetUp_TargetOnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, const char* pConstantName);
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//


//======================== For.Effect_Manager =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline Engine::CParticle_System* CreateParticle(LPDIRECT3DDEVICE9 pGraphicDev, const PARTICLEINFO* pParticleInfo);
inline Engine::CMesh_Effect* CreateMeshEffect(LPDIRECT3DDEVICE9 pGraphicDev);

inline int Call_GuardianFX(GUARDIAN_FX eType, D3DXVECTOR3* vPos, bool bChase = false, float* pAngleX = NULL, float* pAngleY = NULL, float* pAngleZ = NULL);
inline int Call_MagicianFX(MAGICIAN_FX eType, D3DXVECTOR3* vPos, bool bChase = false, float* pAngleX = NULL, float* pAngleY = NULL, float* pAngleZ = NULL);
inline int Call_RangerFX(RANGER_FX eType, D3DXVECTOR3* vPos, bool bChase = false, float* pAngleX = NULL, float* pAngleY = NULL, float* pAngleZ = NULL);
inline int Call_PaladinFX(PALADIN_FX eType, D3DXVECTOR3* vPos, bool bChase = false, float* pAngleX = NULL, float* pAngleY = NULL, float* pAngleZ = NULL);
inline int Call_MonsterFX(MONSTER_FX eType, D3DXVECTOR3* vPos, bool bChase = false, float* pAngleX = NULL, float* pAngleY = NULL, float* pAngleZ = NULL);
inline int Call_EtcFX(ETC_FX eType, D3DXVECTOR3* vPos, bool bChase = false, float* pAngleX = NULL, float* pAngleY = NULL, float* pAngleZ = NULL, D3DXVECTOR3 vScale = D3DXVECTOR3(-1.f, -1.f, -1.f));
inline int Call_TrailFX(TRAIL_FX eType);

inline void ShieldCounting();


inline void Add_GuardianFX(GUARDIAN_FX eType,	Engine::CGameObject* pGameObject);
inline void Add_MagicianFX(MAGICIAN_FX eType,	Engine::CGameObject* pGameObject);
inline void Add_RangerFX(RANGER_FX eType,		Engine::CGameObject* pGameObject);
inline void Add_PaladinFX(PALADIN_FX eType,		Engine::CGameObject* pGameObject);
inline void Add_MonsterFX(MONSTER_FX eType,		Engine::CGameObject* pGameObject);
inline void Add_EtcFX(ETC_FX eType,				Engine::CGameObject* pGameObject);
inline void Add_TrailFX(TRAIL_FX eType,				Engine::CGameObject* pGameObject);

//======================== For.Release_Utility =======================//
inline void Release_Utility(void);
inline void Release_Scene_Uitility(void);

#include "Export_Utility.inl"

END


#endif // Export_Utility_h__