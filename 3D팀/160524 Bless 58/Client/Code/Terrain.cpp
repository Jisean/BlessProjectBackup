#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "Material.h"
#include "Stage.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pTerrainVtx(NULL)
, m_pIndex(NULL)
, m_dwTriCnt(0)
, m_fUVCnt(0.f)
, pVtxPos(NULL)
{
	m_htComponent.Ready_Table(19);	
}

CTerrain::~CTerrain(void)
{
	
}

HRESULT CTerrain::Initialize(WORD& wStageId)
{
	FAILED_CHECK(AddComponent());

	m_pTerrainVtx = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	m_pIndex = new Engine::INDEX32[(VTXCNTX - 1) * (VTXCNTZ - 1) * 2];
	m_pBuffer->GetVtxInfo(m_pTerrainVtx);
	m_pBuffer->GetIdxInfo(m_pIndex, m_dwTriCnt);

	if(ST_STAGE1 == wStageId)
		LoadTerrain();
	else if(ST_STAGE2 == wStageId)
		LoadTerrain2();
	
	return S_OK;
}

_int CTerrain::Update(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(Engine::CRenderer::TYPE_STATIC, this);
	
	return Engine::CGameObject::Update(fTimeDelta);
}

void CTerrain::Render(void)
{	
	if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		return;

	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	Set_ShadowCheck(pEffect, &m_pInfo->m_matWorld);

	pEffect->AddRef();

	Set_ConstantTable(pEffect);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);


	m_pBuffer->Render(&m_pInfo->m_matWorld);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Transform", pComponent);

	//Buffer
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Buffer", pComponent);

	//Texture
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Terrain");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"Texture", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_TerrainNormal");
	m_pNormTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"NormalTexture", pComponent);

	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Filter");
	m_pFilterTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_htComponent.Insert_Table(L"FilterTexture", pComponent);

	// For.Material Component
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)
		, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 20.f);
	m_htComponent.Insert_Table(L"Com_Material", pComponent);	

	//Shader
	pComponent = m_pShader = Engine::Clone_Shader(L"Shader_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_htComponent.Insert_Table(L"Com_Shader", pComponent);

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice, WORD wStageId)
{
	CTerrain*		pGameObject = new CTerrain(pDevice);
	if(FAILED(pGameObject->Initialize(wStageId)))
		Engine::Safe_Release(pGameObject);

	return pGameObject;
}

void CTerrain::Culling_Frustum(void)
{
	pVtxPos = m_pBuffer->Get_VtxPosArrayPointer();

	_ulong dwVtxCntX = ((Engine::CTerrainTex*)m_pBuffer)->Get_VtxCntX();
	_ulong dwVtxCntZ = ((Engine::CTerrainTex*)m_pBuffer)->Get_VtxCntZ();

	_bool		bIn[3] = {false};
	_uint		iTriCnt = 0;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong iIndex = i * dwVtxCntX + j;

			bIn[0] = Engine::Decision_InFrustum(m_pDevice, &pVtxPos[iIndex + dwVtxCntX]);
			bIn[1] = Engine::Decision_InFrustum(m_pDevice, &pVtxPos[iIndex + dwVtxCntX + 1]);
			bIn[2] = Engine::Decision_InFrustum(m_pDevice, &pVtxPos[iIndex + 1]);

			if(bIn[0] || bIn[1] || bIn[2])
			{
				m_pIndex[iTriCnt]._1 = iIndex + dwVtxCntX;
				m_pIndex[iTriCnt]._2 = iIndex + dwVtxCntX + 1;
				m_pIndex[iTriCnt]._3 = iIndex + 1;
				++iTriCnt;
			}

			bIn[0] = Engine::Decision_InFrustum(m_pDevice, &pVtxPos[iIndex + dwVtxCntX]);
			bIn[1] = Engine::Decision_InFrustum(m_pDevice, &pVtxPos[iIndex + 1]);
			bIn[2] = Engine::Decision_InFrustum(m_pDevice, &pVtxPos[iIndex]);

			if(bIn[0] || bIn[1] || bIn[2])
			{
				m_pIndex[iTriCnt]._1 = iIndex + dwVtxCntX;
				m_pIndex[iTriCnt]._2 = iIndex + 1;
				m_pIndex[iTriCnt]._3 = iIndex;
				++iTriCnt;
			}
		}
	}

	m_pBuffer->SetIdxInfo(m_pIndex, iTriCnt);
}

void CTerrain::Culling_QuadTree(void)
{
	_int	iTriCnt = 0;

	Engine::Decision_InQuadTree(m_pDevice, m_pIndex, &iTriCnt);

	m_pBuffer->SetIdxInfo(m_pIndex, iTriCnt);
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Delete_Array(m_pTerrainVtx);
	Engine::Safe_Delete_Array(m_pIndex);
}

void CTerrain::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fUVMount", m_fUVCnt);
	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture",0);
	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture2", 1);
	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture3", 2);
	m_pFilterTexture->Set_ConstantTable(pEffect, "g_FilterTexture4", 3);
	m_pTexture->Set_ConstantTable(pEffect, "g_BaseTexture",0);
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture", 1);
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture2", 2);
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture3", 3);
	m_pTexture->Set_ConstantTable(pEffect, "g_DestTexture4", 4);
	m_pNormTexture->Set_ConstantTable(pEffect, "g_BaseNormTexture",0);
	m_pNormTexture->Set_ConstantTable(pEffect, "g_DestNormTexture", 1);
	m_pNormTexture->Set_ConstantTable(pEffect, "g_DestNormTexture2", 2);
	m_pNormTexture->Set_ConstantTable(pEffect, "g_DestNormTexture3", 3);
	m_pNormTexture->Set_ConstantTable(pEffect, "g_DestNormTexture4", 4);



	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightPos", (_vec4*)&pLightInfo->Position);

	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_Material();
	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&pMtrlInfo->Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pMtrlInfo->Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pMtrlInfo->Specular);

	_matrix			matViewInv;

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matViewInv.m[3][0]);	
	pEffect->SetFloat("g_fPower", pMtrlInfo->Power);
}

void CTerrain::Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix)
{
	if(m_eShadowRender == Engine::CGameObject::SHADOW_NONE_S)
		pEffect->SetTechnique("Default");
	if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		pEffect->SetTechnique("S_Draw_Shadow");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_OBJECT)
		pEffect->SetTechnique("S_Draw_Default");
	

	// Static
	_mymat	matLightView, matLightProj;
	D3DXMatrixLookAtLH(&matLightView, &SSHADOWEYE, &SSHADOWAT, &D3DXVECTOR3(0,1,0));

	float fFov = D3DX_PI/4;
	D3DXMatrixOrthoLH(&matLightProj, 250.f, 250.f, 1.f, 1000.f);

	_mymat matLightWVP;
	matLightWVP = (*pWorldMatrix) * matLightView * matLightProj;

	pEffect->SetMatrix("g_matSLightWVP", &matLightWVP);

	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Static", "g_SShadowTexture");

	// Dynamic
	Engine::CTransform* pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");

	_myvec vDLightDir = SSHADOWEYE - SSHADOWAT;
	D3DXVec3Normalize(&vDLightDir, &vDLightDir);
	_myvec vDLightPos = pPlayerInfo->m_vPos + (vDLightDir * 10.f);

	_mymat	matDLightView, matDLightProj;
	D3DXMatrixLookAtLH(&matDLightView, &vDLightPos, &pPlayerInfo->m_vPos, &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matDLightProj, 3.f, 3.f, 1.f, 1000.f);

	_mymat matDLightWVP;
	matDLightWVP = (*pWorldMatrix) * matDLightView * matDLightProj;

	pEffect->SetMatrix("g_matDLightWVP", &matDLightWVP);

	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Dynamic", "g_DShadowTexture");

}

void CTerrain::LoadTerrain(void)
{
	int iCntX = VTXCNTX;
	int iCntZ = VTXCNTZ;
	int iItv = VTXITV;

	HANDLE	hFile = CreateFile(L"..\\bin\\Resources\\Data\\City\\SampleTerrain.dat", GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	ReadFile(hFile, &iCntX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iCntZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iItv, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &m_fUVCnt, sizeof(float), &dwByte, NULL);

	int		iIndex = 0;
	for(int z = 0; z < iCntZ; ++z)
	{
		for(int x = 0; x < iCntX; ++x)
		{
			iIndex = z * iCntX + x;

			ReadFile(hFile, &m_pTerrainVtx[iIndex].vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		}
	}

	m_pBuffer->SetVtxInfo(m_pTerrainVtx);
	SetNormal();
	// 높이 바꿨을때 호출 해줘야함
	m_pBuffer->Set_VertexPositionArray(true);

	CloseHandle(hFile);
}

void CTerrain::LoadTerrain2(void)
{
	int iCntX = VTXCNTX;
	int iCntZ = VTXCNTZ;
	int iItv = VTXITV;

	HANDLE	hFile = CreateFile(L"..\\bin\\Resources\\Data\\Dungeon\\SampleDungeonTerrain.dat", GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;

	ReadFile(hFile, &iCntX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iCntZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iItv, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &m_fUVCnt, sizeof(float), &dwByte, NULL);

	int		iIndex = 0;
	for(int z = 0; z < iCntZ; ++z)
	{
		for(int x = 0; x < iCntX; ++x)
		{
			iIndex = z * iCntX + x;

			ReadFile(hFile, &m_pTerrainVtx[iIndex].vPos, sizeof(D3DXVECTOR3), &dwByte, NULL);
		}
	}

	m_pBuffer->SetVtxInfo(m_pTerrainVtx);
	SetNormal();
	// 높이 바꿨을때 호출 해줘야함
	m_pBuffer->Set_VertexPositionArray(true);

	CloseHandle(hFile);
}

void CTerrain::SetNormal(void)
{
	int		iIndex = 0;
	int iTriCnt = 0;
	for(int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for(int x = 0; x < VTXCNTX - 1; ++x)
		{
			iIndex = z * VTXCNTX + x;

			m_pIndex[iTriCnt]._1 = iIndex + VTXCNTX;
			m_pIndex[iTriCnt]._2 = iIndex + VTXCNTX + 1;
			m_pIndex[iTriCnt]._3 = iIndex + 1;

			D3DXVECTOR3		vDest, vSour, vNormal;
			vDest = m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._1].vPos;
			vSour = m_pTerrainVtx[m_pIndex[iTriCnt]._3].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			m_pTerrainVtx[m_pIndex[iTriCnt]._1].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._2].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;

			m_pIndex[iTriCnt]._1 = iIndex + VTXCNTX;
			m_pIndex[iTriCnt]._2 = iIndex + 1;
			m_pIndex[iTriCnt]._3 = iIndex;
			vDest = m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._1].vPos;
			vSour = m_pTerrainVtx[m_pIndex[iTriCnt]._3].vPos - m_pTerrainVtx[m_pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			m_pTerrainVtx[m_pIndex[iTriCnt]._1].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._2].vNormal += vNormal;
			m_pTerrainVtx[m_pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;
		}
	}
	m_pBuffer->SetVtxInfo(m_pTerrainVtx);
}
