

//======================== For.Management =======================//
//--Getter--------------------------------------------------------//
const CComponent* Engine::Get_Component(const WORD LayerID, const TCHAR* pObjKey, const TCHAR* pComponentKey)
{
	return CManagement::GetInstance()->GetComponent(LayerID, pObjKey, pComponentKey);
}
Engine::CScene*	Engine::Get_Scene(void)
{
	return CManagement::GetInstance()->GetScene();
}
inline vector<CGameObject*>* Engine::Get_ObjList(const WORD LayerID, const TCHAR* pObjKey)
{
	return CManagement::GetInstance()->GetObjList(LayerID, pObjKey);
}

//--Setter--------------------------------------------------------//
void Engine::Add_RenderGroup(Engine::CRenderer::RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	CManagement::GetInstance()->AddRenderGroup(eRenderGroup, pGameObject);
}
float*	Engine::Set_RendererLightPower(void)
{
	return CManagement::GetInstance()->SetRendererLightPower();
}
float* SetRendererBloomPower(void)
{
	return CManagement::GetInstance()->SetRendererBloomPower();
}
float* SetRendererBloomRange(void)
{
	return CManagement::GetInstance()->SetRendererBloomRange();
}
void Switch_DebugBuffer(bool bOnOff)
{
	CManagement::GetInstance()->OnOffDebugBuffer(bOnOff);
}
void Set_DOFRender(bool bDOF)
{
	CManagement::GetInstance()->SetDOFRender(bDOF);
}
void Set_RadialRender(bool bRadial, float fMaxTime/* = 5.f*/)
{
	CManagement::GetInstance()->SetRadialRender(bRadial, fMaxTime);
}

void Set_ShadowRender(bool bShadow)
{
	CManagement::GetInstance()->SetShadowRender(bShadow);
}

void Set_VolumetricRender(bool bVolumetric)
{
	CManagement::GetInstance()->SetVolumetricRender(bVolumetric);
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
const D3DLIGHT9* Engine::Get_LightInfo(const _ulong& dwIndex/* = 0*/, TCHAR*	pKeyName)
{
	return CLightMgr::GetInstance()->Get_LightInfo(dwIndex, pKeyName);
}

Engine::CLight* Engine::Get_Light(TCHAR*	pKeyName, const DWORD& dwLightIdx)
{
	return CLightMgr::GetInstance()->Get_Light(pKeyName, dwLightIdx);
}

int Engine::Get_LightCnt(TCHAR*	pKeyName)
{
	return CLightMgr::GetInstance()->Get_LightCnt(pKeyName);
}

size_t Get_LightKeyCnt(void)
{
	return CLightMgr::GetInstance()->Get_LightKeyCnt();
}

//--Setter--------------------------------------------------------//
HRESULT Engine::Add_Light(LPDIRECT3DDEVICE9 pDevice, const D3DLIGHT9* pLightInfo, TCHAR*	pKeyName)
{
	return CLightMgr::GetInstance()->AddLight(pDevice, pLightInfo, pKeyName);
}

void Set_LightRender(TCHAR* pKeyName, bool bRender)
{
	CLightMgr::GetInstance()->Set_LightRender(pKeyName, bRender);
}

void Set_LightRender(UINT iLightKey, bool bRender)
{
	CLightMgr::GetInstance()->Set_LightRender(iLightKey, bRender);
}

//--General-------------------------------------------------------//

// =========================== For.Optimazation =========================== //
// For.Getter----------------------------------------------------------------------------------------------------------------------------------------------------------------

// For.Setter----------------------------------------------------------------------------------------------------------------------------------------------------------------

// For.General---------------------------------------------------------------------------------------------------------------------------------------------------------------
_bool Engine::Decision_InFrustum(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius)
{
	return Engine::CFrustum::GetInstance()->Decision_In(pGraphicDev, pPosition, fRadius);
}

_bool Engine::Decision_InFrustum_Obj(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition, const _float& fRadius/* = 0.f*/)
{
	return Engine::CFrustum::GetInstance()->Decision_In_Object(pGraphicDev, pPosition, fRadius);
}

_bool Engine::Decision_InFrustum(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPosition)
{
	return Engine::CFrustum::GetInstance()->Decision_In(pGraphicDev, pPosition);
}
HRESULT Engine::Initialize_QuadTree(const _vec3* pVtxPos, const _ulong& dwVtxCntX, const _ulong& dwVtxCntZ)
{
	return Engine::CQuadTree::GetInstance()->Initialize_QuadTree(pVtxPos, dwVtxCntX, dwVtxCntZ);
}

void Engine::Decision_InQuadTree(LPDIRECT3DDEVICE9 pGraphicDev, INDEX32* pIndex, _int* pTriCnt)
{
	Engine::CQuadTree::GetInstance()->Decision_In(pGraphicDev, pIndex, pTriCnt);
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
bool Set_InOccBox(CGameObject* pObject)
{
	return COcclusionMgr::GetInstance()->SetInOccBox(pObject);
}
void Set_OccVtxPos(const _uint iIndex)
{
	COcclusionMgr::GetInstance()->Set_VtxPos(iIndex);
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
int Test_InOccBox(D3DXVECTOR3 vPos)
{
	return COcclusionMgr::GetInstance()->TestInOccBox(vPos);
}

//======================== For.NaviMgr =======================//
//--Getter--------------------------------------------------------//
DWORD Get_CellIndex(D3DXVECTOR3* pPos)
{
	return CNaviMgr::GetInstance()->GetCellIndex(pPos);
}
//--Setter--------------------------------------------------------//
void Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize)
{
	CNaviMgr::GetInstance()->Reserve_CellContainerSize(pDevice, dwSize);
}

HRESULT Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwType)
{
	return CNaviMgr::GetInstance()->Add_Cell(pPointA, pPointB, pPointC, dwType);
}

//--General-------------------------------------------------------//
void Link_Cell(void)
{
	CNaviMgr::GetInstance()->Link_Cell();
}

DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx, float fHeight)
{
	return CNaviMgr::GetInstance()->MoveOnNaviMesh(pPos, pDir, dwCurrentIdx, fHeight);
}

void	Render_Navi(void)
{
	CNaviMgr::GetInstance()->Render();
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


//======================== For.Effect_Manager =======================//

//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
void Engine::Add_GuardianFX(GUARDIAN_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetGuardianFX(eType)->push_back((CMesh_Effect*)pGameObject);
}

void Engine::Add_RangerFX(RANGER_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetRangerFX(eType)->push_back((CMesh_Effect*)pGameObject);
}

void Engine::Add_PaladinFX(PALADIN_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetPaladinFX(eType)->push_back((CMesh_Effect*)pGameObject);
}

void Engine::Add_MagicianFX(MAGICIAN_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetMagicianFX(eType)->push_back((CMesh_Effect*)pGameObject);
}

void Engine::Add_MonsterFX(MONSTER_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetMonsterFX(eType)->push_back((CMesh_Effect*)pGameObject);
}

void Engine::Add_EtcFX(ETC_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetEtcFX(eType)->push_back((CMesh_Effect*)pGameObject);
}

void Engine::Add_TrailFX(TRAIL_FX eType, CGameObject* pGameObject)
{
	Engine::CEffect_Manager::GetInstance()->GetTrailFX(eType)->push_back(pGameObject);
}



//--General-------------------------------------------------------//

CParticle_System* Engine::CreateParticle(LPDIRECT3DDEVICE9 pGraphicDev, const PARTICLEINFO* pParticleInfo)
{
	return	Engine::CEffect_Manager::GetInstance()->CreateParticle(pGraphicDev, pParticleInfo);
}

CMesh_Effect* Engine::CreateMeshEffect(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return	Engine::CEffect_Manager::GetInstance()->CreateMeshEffect(pGraphicDev);
}

int Engine::Call_GuardianFX(GUARDIAN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	return Engine::CEffect_Manager::GetInstance()->Call_GuardianFX(eType, vPos, bChase, pAngleX, pAngleY, pAngleZ);
}

int Engine::Call_RangerFX(RANGER_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	return Engine::CEffect_Manager::GetInstance()->Call_RangerFX(eType, vPos, bChase, pAngleX, pAngleY, pAngleZ);
}

int Engine::Call_PaladinFX(PALADIN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	return Engine::CEffect_Manager::GetInstance()->Call_PaladinFX(eType, vPos, bChase, pAngleX, pAngleY, pAngleZ);
}

int Engine::Call_MagicianFX(MAGICIAN_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	return Engine::CEffect_Manager::GetInstance()->Call_MagicianFX(eType, vPos, bChase, pAngleX, pAngleY, pAngleZ);
}

int Engine::Call_MonsterFX(MONSTER_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ)
{
	return Engine::CEffect_Manager::GetInstance()->Call_MonsterFX(eType, vPos, bChase, pAngleX, pAngleY, pAngleZ);
}

int Engine::Call_EtcFX(ETC_FX eType, D3DXVECTOR3* vPos, bool bChase, float* pAngleX, float* pAngleY, float* pAngleZ, D3DXVECTOR3 vScale)
{
	return Engine::CEffect_Manager::GetInstance()->Call_EtcFX(eType, vPos, bChase, pAngleX, pAngleY, pAngleZ, vScale);
}

int Engine::Call_TrailFX(TRAIL_FX eType)
{
	return Engine::CEffect_Manager::GetInstance()->Call_TrailFX(eType);
}

void Engine::ShieldCounting()
{
	Engine::CEffect_Manager::GetInstance()->ShieldCounting();
}




//======================== For.Release_Utility =======================//
void Engine::Release_Utility(void)
{
	CManagement::GetInstance()->DestroyInstance();
	CShader_Manager::GetInstance()->DestroyInstance();
	CRenderTarget_Manager::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	//CQuadTree::GetInstance()->DestroyInstance();
	CFrustum::GetInstance()->DestroyInstance();
	CNaviMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	COcclusionMgr::GetInstance()->DestroyInstance();
	CEffect_Manager::GetInstance()->DestroyInstance();
}

void Release_Scene_Uitility(void)
{
	CNaviMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	COcclusionMgr::GetInstance()->DestroyInstance();
	CEffect_Manager::GetInstance()->DestroyInstance();
}