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
#include "OcclusionMgr.h"

BEGIN(Engine)


//======================== For.Management =======================//
//--Getter--------------------------------------------------------//
inline const CComponent* Get_Component(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey);
inline Engine::CScene* Get_Scene(void);

//--Setter--------------------------------------------------------//
inline void Add_RenderGroup(Engine::CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject);

//--General-------------------------------------------------------//
inline HRESULT Ready_Management(LPDIRECT3DDEVICE9 pDevice);
inline _int Update_Management(const _float& fTimeDelta);
inline void Render_Management(void);
inline void Clear_RenderGroup(void);
template <typename T> HRESULT SceneChange(T& Functor);

//======================== For.LightMgr =======================//
//--Getter--------------------------------------------------------//
inline const D3DLIGHT9* Get_LightInfo(const _ulong& dwIndex = 0);
//--Setter--------------------------------------------------------//
inline HRESULT Add_Light(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx);
//--General-------------------------------------------------------//

//======================== For.Frustum =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline HRESULT Ready_Frustum(LPDIRECT3DDEVICE9 pDevice);
inline void Transform_Frustum_ToWorld(void);
inline void Transform_Frustum_ToLocal(const D3DXMATRIX* pWorld);
inline void CullingForTerrain(INDEX32* pIndex, DWORD* pTriCnt, const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);
inline bool VertexInFrustum(const D3DXVECTOR3* pPoint);
inline bool SphereInFrustum(const D3DXVECTOR3* pCenter, const float& fRadius);

//======================== For.QuadTree =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline HRESULT Ready_Neighbor(const VTXTEX* pTerrainVtx, CQuadTree* pParentQuad, const WORD& wCntX, const WORD& wCntZ);
inline HRESULT Ready_QuadTree(const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);
inline void IsinFrustum(const VTXTEX* pTerrainVtx);
inline void CullingToQuadTree(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);
inline void Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);

//======================== For.Occlusion =======================//
//--Getter--------------------------------------------------------//
inline CTransform* Get_OccBox(const _uint iIndex);
inline CCubeColor* Get_OccBoxVtx(const _uint iIndex);
//--Setter--------------------------------------------------------//
inline void Set_OccBox(const _uint iIndex, CTransform* pInfo);
inline HRESULT Add_OccBox(const D3DXMATRIX* pmatWorld);
//--General-------------------------------------------------------//
inline HRESULT Ready_Occlusion(LPDIRECT3DDEVICE9 pDevice);
inline void Render_OccBox(void);


//======================== For.NaviMgr =======================//
//--Getter--------------------------------------------------------//
inline int Get_NaviMeshSize(void);
inline void Get_NearPoint(D3DXVECTOR3& vPoint, float fRange);
inline vector<CNaviCell*> Get_Cell(void);
//--Setter--------------------------------------------------------//
inline HRESULT Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize);
inline HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwType);
inline bool Set_OnePoint(D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2, D3DXVECTOR3 vPoint3);
inline bool Set_TwoPoint(D3DXVECTOR2 vDir, CNaviCell* pNaviClose, D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2);
//--General-------------------------------------------------------//
inline void Link_Cell(void);
inline void Unlink_Cell(void);
inline DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx);
inline void	Render_Navi(void);
inline void Change_Point(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour);
inline void Delete_Point(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour);
inline void Delete_Triangle(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour);
inline void Reset_Index(void);
inline void Reset_NaviMesh(void);

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





//======================== For.Release_Utility =======================//
inline void Release_Utility(void);



#include "Export_Utility.inl"

END


#endif // Export_Utility_h__