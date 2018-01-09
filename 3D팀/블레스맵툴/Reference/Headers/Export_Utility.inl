

//======================== For.Management =======================//
//--Getter--------------------------------------------------------//
const CComponent* Engine::Get_Component(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	return CManagement::GetInstance()->GetComponent(LayerID, pObjKey, pComponentKey);
}
CScene* Engine::Get_Scene(void)
{
	return CManagement::GetInstance()->Get_Scene();
}

//--Setter--------------------------------------------------------//
void Engine::Add_RenderGroup(Engine::CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	CManagement::GetInstance()->AddRenderGroup(eRenderGroup, pGameObject);
}

//--General-------------------------------------------------------//
HRESULT Engine::Ready_Management(LPDIRECT3DDEVICE9 pDevice)
{
	return CManagement::GetInstance()->InitManagement(pDevice);
}
_int Engine::Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update(fTimeDelta);
}
void Engine::Render_Management(void)
{
	CManagement::GetInstance()->Render();
}

void Engine::Clear_RenderGroup(void)
{
	CManagement::GetInstance()->Clear_RenderGroup();
}

template <typename T> HRESULT Engine::SceneChange(T& Functor)
{
	return CManagement::GetInstance()->SceneChange(Functor);
}

//======================== For.LightMgr =======================//
//--Getter--------------------------------------------------------//
const D3DLIGHT9* Engine::Get_LightInfo(const _ulong& dwIndex/* = 0*/)
{
	return CLightMgr::GetInstance()->Get_LightInfo(dwIndex);
}
//--Setter--------------------------------------------------------//
HRESULT Engine::Add_Light(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx)
{
	return CLightMgr::GetInstance()->AddLight(pDevice, pLightInfo, dwLightIdx);
}
//--General-------------------------------------------------------//

//======================== For.Frustum =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
HRESULT Ready_Frustum(LPDIRECT3DDEVICE9 pDevice)
{
	return CFrustum::GetInstance()->InitFrustum(pDevice);
}

void Transform_Frustum_ToWorld(void)
{
	CFrustum::GetInstance()->Transform_Frustum_ToWorld();
}

void Transform_Frustum_ToLocal(const D3DXMATRIX* pWorld)
{
	CFrustum::GetInstance()->Transform_Frustum_ToLocal(pWorld);
}

void CullingForTerrain(INDEX32* pIndex, DWORD* pTriCnt, const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ)
{
	CFrustum::GetInstance()->CullingForTerrain(pIndex, pTriCnt, pTerrainVtx, wCntX, wCntZ);
}

bool VertexInFrustum(const D3DXVECTOR3* pPoint)
{
	return CFrustum::GetInstance()->VertexInFrustum(pPoint);
}

bool SphereInFrustum(const D3DXVECTOR3* pCenter, const float& fRadius)
{
	return CFrustum::GetInstance()->SphereInFrustum(pCenter, fRadius);
}

//======================== For.QuadTree =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
HRESULT Ready_Neighbor(const VTXTEX* pTerrainVtx, CQuadTree* pParentQuad, const WORD& wCntX, const WORD& wCntZ)
{
	return CQuadTree::GetInstance()->InitNeighbor(pTerrainVtx, pParentQuad, wCntX, wCntZ);
}

HRESULT Ready_QuadTree(const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ)
{
	return CQuadTree::GetInstance()->InitQuadTree(pTerrainVtx, wCntX, wCntZ);
}

void IsinFrustum(const VTXTEX* pTerrainVtx)
{
	CQuadTree::GetInstance()->IsinFrustum(pTerrainVtx);
}

void CullingToQuadTree(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt)
{
	CQuadTree::GetInstance()->CullingToQuadTree(pTerrainVtx, pIndex, pTriCnt);
}

void Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt)
{
	CQuadTree::GetInstance()->Culling_LOD(pTerrainVtx, pIndex, pTriCnt);
}

//======================== For.Occlusion =======================//
//--Getter--------------------------------------------------------//
CTransform* Get_OccBox(const _uint iIndex)
{
	return COcclusionMgr::GetInstance()->Get_OccBox(iIndex);
}
CCubeColor* Get_OccBoxVtx(const _uint iIndex)
{
	return COcclusionMgr::GetInstance()->Get_OccBoxVtx(iIndex);
}
//--Setter--------------------------------------------------------//
void Set_OccBox(const _uint iIndex, Engine::CTransform* pInfo)
{
	COcclusionMgr::GetInstance()->Set_OccBox(iIndex, pInfo);
}
HRESULT Add_OccBox(const D3DXMATRIX* pmatWorld)
{
	return COcclusionMgr::GetInstance()->Add_OccBox(pmatWorld);
}


//--General-------------------------------------------------------//
HRESULT Ready_Occlusion(LPDIRECT3DDEVICE9 pDevice)
{
	return COcclusionMgr::GetInstance()->InitOcclusion(pDevice);
}

void Render_OccBox(void)
{
	COcclusionMgr::GetInstance()->Render_OccBox();
}

//======================== For.NaviMgr =======================//
//--Getter--------------------------------------------------------//
int Get_NaviMeshSize(void)
{
	return CNaviMgr::GetInstance()->GetSize();
}
void Get_NearPoint(D3DXVECTOR3& vPoint, float fRange)
{
	CNaviMgr::GetInstance()->GetNearPoint(vPoint, fRange);
}
vector<CNaviCell*> Get_Cell(void)
{
	return *(CNaviMgr::GetInstance()->GetCell());
}
//--Setter--------------------------------------------------------//
HRESULT Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize)
{
	return CNaviMgr::GetInstance()->Reserve_CellContainerSize(pDevice, dwSize);
}

HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwType)
{
	return CNaviMgr::GetInstance()->Add_Cell(pPointA, pPointB, pPointC, dwType);
}
bool Set_OnePoint(D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2, D3DXVECTOR3 vPoint3)
{
	return CNaviMgr::GetInstance()->SetOnePoint(vPoint1, vPoint2, vPoint3);
}
bool Set_TwoPoint(D3DXVECTOR2 vDir, CNaviCell* pNaviClose, D3DXVECTOR3& vPoint1, D3DXVECTOR3& vPoint2)
{
	return CNaviMgr::GetInstance()->SetTwoPoint(vDir, pNaviClose, vPoint1, vPoint2);
}
//--General-------------------------------------------------------//
void Link_Cell(void)
{
	CNaviMgr::GetInstance()->Link_Cell();
}
void Unlink_Cell(void)
{
	CNaviMgr::GetInstance()->Unlink_Cell();
}
DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx)
{
	return CNaviMgr::GetInstance()->MoveOnNaviMesh(pPos, pDir, dwCurrentIdx);
}
void	Render_Navi(void)
{
	CNaviMgr::GetInstance()->Render();
}
void Change_Point(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour)
{
	CNaviMgr::GetInstance()->ChangePoint(vDest, vSour);
}
void Delete_Point(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour)
{
	CNaviMgr::GetInstance()->DeletePoint(vDest, vSour);
}
void Delete_Triangle(D3DXVECTOR3 vDest, D3DXVECTOR3 vSour)
{
	CNaviMgr::GetInstance()->DeleteTriangle(vDest, vSour);
}
void Reset_Index(void)
{
	CNaviMgr::GetInstance()->ResetIndex();
}
void Reset_NaviMesh(void)
{
	CNaviMgr::GetInstance()->ResetNavimesh();
}
//======================== For.CollisionMgr =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
bool Collision_OBB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax, const D3DXMATRIX* pDestWorld
						  , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax, const D3DXMATRIX* pSourWorld)
{
	return CCollisionMgr::GetInstance()->Collision_OBB(pDestMin, pDestMax, pDestWorld, pSourMin, pSourMax, pSourWorld);
}

bool Collision_AABB(const D3DXVECTOR3* pDestMin, const D3DXVECTOR3* pDestMax
						   , const D3DXVECTOR3* pSourMin, const D3DXVECTOR3* pSourMax)
{	
	return CCollisionMgr::GetInstance()->Collision_AABB(pDestMin, pDestMax, pSourMin, pSourMax);
}

//======================== For.Shader_Manager =======================//
//--Getter--------------------------------------------------------//
CShader* Engine::Clone_Shader(const TCHAR* pShaderTag)
{
	return Engine::CShader_Manager::GetInstance()->Clone_Shader(pShaderTag);
}
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//



//======================== For.RenderTarget_Manager =======================//
//--Getter--------------------------------------------------------//
void Engine::SetUp_TargetOnShader(LPD3DXEFFECT pEffect, const TCHAR* pTargetTag, const char* pConstantName)
{
	CRenderTarget_Manager::GetInstance()->SetUp_TargetOnShader(pEffect, pTargetTag, pConstantName);
}
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//


//======================== For.Release_Utility =======================//
void Engine::Release_Utility(void)
{
	CManagement::GetInstance()->DestroyInstance();
	CShader_Manager::GetInstance()->DestroyInstance();
	CRenderTarget_Manager::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CFrustum::GetInstance()->DestroyInstance();
	CQuadTree::GetInstance()->DestroyInstance();
	CNaviMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
}