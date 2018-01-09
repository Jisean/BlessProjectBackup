#include "stdafx.h"
#include "Units.h"
#include "DynamicMesh.h"
#include "CubeColor.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Stage.h"

CUnits::CUnits(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_pMesh(NULL)
, m_pShader(NULL)
, m_pCalculatorCom(NULL)
, m_pMtrlCom(NULL)
, m_pStatus(NULL)
, m_vLook(1.f, 0.f, 0.f)
{
	m_bAttPositionFull = false;
	m_bLong_AttPositionFull = false;
	m_eUnitType = TYPEID_PLAYER;
	m_vecAttPositionInfo.reserve(15);
	m_vecLong_AttPositionInfo.reserve(15);

	AttackPositionInit();
}

CUnits::~CUnits(void)
{	

}

_int CUnits::Update(const _float& fTimeDelta)
{
	D3DXVECTOR3 vMin, vMax;
	m_pMesh->GetMinMax(&vMin, &vMax);
	D3DXVec3TransformCoord(&vMin, &vMin, &m_pInfo->m_matWorld);
	D3DXVec3TransformCoord(&vMax, &vMax, &m_pInfo->m_matWorld);

	Culling_Frustum_Object(vMin, vMax);

	return 0;
}

void CUnits::SetAniInfo(vector<Engine::ANI_INFO*> pAniInfo)
{
	m_vecAniInfo = pAniInfo;
}

void CUnits::SetBoxInfo(vector<Engine::BOX_INFO*>* pData)
{
	vector<Engine::BOX_INFO*>::iterator iter = pData->begin();
	vector<Engine::BOX_INFO*>::iterator iter_end = pData->end();

	for(; iter != iter_end; ++iter)
	{
		if((*iter)->fRadius == 0)
		{
			// 최종 TCHAR[] -> const char* 변환과정
			// wstring -> string-> const char*
			wstring wstrName = (*iter)->tBoneName;
			string str;
			str.assign(wstrName.begin(), wstrName.end());
			const char* szBoneName = str.c_str();

			// 뼈 행렬 찾음
			D3DXMATRIX* pmatBoxTemp = new D3DXMATRIX;
			D3DXMATRIX* pmatBox = const_cast<D3DXMATRIX*>(m_pMesh->FindFrame(szBoneName));	

			m_vecOriMat.push_back(pmatBox);

			memcpy(pmatBoxTemp, pmatBox, sizeof(D3DXMATRIX));

			// 박스 생성
			Engine::CVIBuffer* BoundingBox;
			BoundingBox = Engine::CCubeColor::Create(m_pDevice, (*iter)->vMin, (*iter)->vMax, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

			m_vecBoundingBox.push_back(BoundingBox);
			m_vecMatrixBox.push_back(pmatBoxTemp);
		}
		else
		{
			// 최종 TCHAR[] -> const char* 변환과정
			// wstring -> string-> const char*
			wstring wstrName = (*iter)->tBoneName;
			string str;
			str.assign(wstrName.begin(), wstrName.end());
			const char* szBoneName = str.c_str();

			// 뼈 행렬 찾음
			D3DXMATRIX* pmatSpher = const_cast<D3DXMATRIX*>(m_pMesh->FindFrame(szBoneName));
			// 구 생성
			LPD3DXMESH	pMesh;
			D3DXCreateSphere(m_pDevice, (*iter)->fRadius, 16, 8, &pMesh, NULL);

			SphereColor(&m_pDevice, &pMesh);

			m_vecMesh.push_back(pMesh);
			m_vecMatrixSphere.push_back(pmatSpher);
		}
	}
}

void CUnits::SphereColor(LPDIRECT3DDEVICE9* ppDevice, LPD3DXMESH* ppMesh)
{
	(*ppMesh)->CloneMeshFVF(0, Engine::VTXFVF_COL, *ppDevice, (ppMesh));

	LPDIRECT3DVERTEXBUFFER9 pTempBuffer;
	(*ppMesh)->GetVertexBuffer(&pTempBuffer);

	int num = (*ppMesh)->GetNumVertices();

	Engine::VTXCOL* pVercol = NULL;

	DWORD color = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	pTempBuffer->Lock(0,0,(void**)&pVercol, 0);

	for(int i = 0; i < num; ++i)
		pVercol[i].dwColor = color;

	pTempBuffer->Unlock();
}


void CUnits::BoxRender(void)
{
	vector<D3DXMATRIX*>::iterator iter		= m_vecMatrixBox.begin();
	vector<D3DXMATRIX*>::iterator iter_end  = m_vecMatrixBox.end();

	for(; iter != iter_end; ++iter)
	{
		*(*iter) = (*(*iter)) * m_pInfo->m_matWorld;
	}

	iter		= m_vecMatrixBox.begin();
	iter_end  = m_vecMatrixBox.end();

	vector<Engine::CVIBuffer*>::iterator iter2 = m_vecBoundingBox.begin();
	vector<Engine::CVIBuffer*>::iterator iter2_end = m_vecBoundingBox.end();

	int iSize = m_vecBoundingBox.size();

	for(int i = 0; i < iSize; ++i)
	{
		m_pDevice->SetTexture(0, NULL);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		(*iter2)->Render((*iter));
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		++iter;
		++iter2;
	}


	vector<D3DXMATRIX*>::iterator itermatSphere = m_vecMatrixSphere.begin();

	vector<LPD3DXMESH>::iterator iterMesh = m_vecMesh.begin();
	vector<LPD3DXMESH>::iterator iterMesh_end = m_vecMesh.end();

	for(; iterMesh != iterMesh_end; ++iterMesh)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, &(*(*itermatSphere) * m_pInfo->m_matWorld));		

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		(*iterMesh)->DrawSubset(0);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		++(itermatSphere);
	}
}

void CUnits::SetAniSpeed(float fAniSpeed/* = 1.f*/)
{
	m_pMesh->SetAniSpeed(fAniSpeed);
}

void CUnits::Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix)
{
	if(m_eShadowRender == Engine::CGameObject::SHADOW_NONE_D)
		pEffect->SetTechnique("Default_Device");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_SHADOW)
		pEffect->SetTechnique("D_Draw_Shadow");
	else if(m_eShadowRender == Engine::CGameObject::SHADOW_OBJECT)
		pEffect->SetTechnique("D_Draw_Default");


	pEffect->SetFloat("g_fWeaponCheck", 0.f);
	// Dynamic Shaodw Set
	Engine::CTransform* pPlayerInfo = (Engine::CTransform*)Engine::Get_Component(LAYER_GAMELOGIC, L"Player", L"Transform");

	_myvec vDLightDir = SSHADOWEYE - SSHADOWAT;
	D3DXVec3Normalize(&vDLightDir, &vDLightDir);
	_myvec vLightPos = pPlayerInfo->m_vPos + (vDLightDir * 10.f);

	_mymat	matLightView, matLightProj;
	D3DXMatrixLookAtLH(&matLightView, &vLightPos, &pPlayerInfo->m_vPos, &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matLightProj, 3.f, 3.f, 1.f, 1000.f);

	_mymat matLightWVP;
	matLightWVP = (*pWorldMatrix) * matLightView * matLightProj;

	pEffect->SetMatrix("g_matDLightWVP", &matLightWVP);
	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Dynamic", "g_DShadowTexture");

	// Static Shadow Set
	_mymat	matSLightView, matSLightProj;
	D3DXMatrixLookAtLH(&matSLightView, &D3DXVECTOR3(-148.066f, 432.107f, -148.066f), &D3DXVECTOR3(-142.399f, 424.358f, -142.399f), &D3DXVECTOR3(0,1,0));
	D3DXMatrixOrthoLH(&matSLightProj, 250.f, 250.f, 1.f, 1000.f);

	_mymat matSLightWVP;
	matSLightWVP = (*pWorldMatrix) * matSLightView * matSLightProj;

	pEffect->SetMatrix("g_matSLightWVP", &matSLightWVP);
	Engine::SetUp_TargetOnShader(pEffect, L"Shadow_Static", "g_SShadowTexture");
}

void CUnits::AttackPositionInit(void)
{
	for(int i = 0; i < 15; ++i)
	{
		Engine::ATTPOSITION* AttPositionInfo = new Engine::ATTPOSITION;

		AttPositionInfo->bPossession = false;
		//ZeroMemory(&AttPositionInfo->vPos, sizeof(_vec3));
		AttPositionInfo->vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		AttPositionInfo->fAttDist = 0.5;

		m_vecAttPositionInfo.push_back(AttPositionInfo);
	} 

	for(int i = 0; i < 15; ++i)
	{
		Engine::ATTPOSITION* pLong_AttPositionInfo = new Engine::ATTPOSITION;

		pLong_AttPositionInfo->bPossession = false;
		//ZeroMemory(&pLong_AttPositionInfo->vPos, sizeof(_vec3));
		pLong_AttPositionInfo->vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pLong_AttPositionInfo->fAttDist = 1.5f;

		m_vecLong_AttPositionInfo.push_back(pLong_AttPositionInfo);
	}
}

void CUnits::AttackPositionSetting(void)
{
	// 근거리 좌표
	vector<Engine::ATTPOSITION*>::iterator iter = m_vecAttPositionInfo.begin();
	vector<Engine::ATTPOSITION*>::iterator iter_end = m_vecAttPositionInfo.end();

	_vec3 vTempPos = _vec3(m_pInfo->m_vPos.x + 5.f, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	_vec3 vTempDir = vTempPos - m_pInfo->m_vPos;
	D3DXVec3Normalize(&vTempDir, &vTempDir);

	int i = 0;

	for(iter; iter != iter_end; ++iter)
	{
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(15 * i));
		D3DXVec3TransformNormal(&vTempDir, &vTempDir, &matRotY);

		(*iter)->vPos = m_pInfo->m_vPos + vTempDir * (*iter)->fAttDist;
		++i;
	}


	// 원거리 좌표
	vector<Engine::ATTPOSITION*>::iterator iter_Long = m_vecLong_AttPositionInfo.begin();
	vector<Engine::ATTPOSITION*>::iterator iter_Long_end = m_vecLong_AttPositionInfo.end();

	_vec3 vTempPos2 = _vec3(m_pInfo->m_vPos.x + 5.f, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	_vec3 vTempDir2 = vTempPos2 - m_pInfo->m_vPos;
	D3DXVec3Normalize(&vTempDir2, &vTempDir2);

	i = 0;
	for(iter_Long; iter_Long != iter_Long_end; ++iter_Long)
	{
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(15 * i));
		D3DXVec3TransformNormal(&vTempDir2, &vTempDir2, &matRotY);

		(*iter_Long)->vPos = m_pInfo->m_vPos + vTempDir2 * (*iter_Long)->fAttDist;
		++i;
	}
}


void CUnits::Free(void)
{
	vector<Engine::CVIBuffer*>::iterator iterBuffer = m_vecBoundingBox.begin();
	vector<Engine::CVIBuffer*>::iterator iterBuffer_end = m_vecBoundingBox.end();

	for(iterBuffer; iterBuffer != iterBuffer_end; ++iterBuffer)		
		Engine::Safe_Release((*iterBuffer));

	m_vecBoundingBox.clear();
	m_vecMatrixBox.clear();
	m_vecOriMat.clear();
	m_vecMatrixSphere.clear();
	

	vector<Engine::ATTPOSITION*>::iterator iter = m_vecAttPositionInfo.begin();
	vector<Engine::ATTPOSITION*>::iterator iter_end = m_vecAttPositionInfo.end();

	for(iter; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete(*iter);
	}
	m_vecAttPositionInfo.clear();

	vector<Engine::ATTPOSITION*>::iterator iterLong = m_vecLong_AttPositionInfo.begin();
	vector<Engine::ATTPOSITION*>::iterator iterLong_end = m_vecLong_AttPositionInfo.end();

	for(iterLong; iterLong != iterLong_end; ++iterLong)
	{
		Engine::Safe_Delete(*iterLong);
	}
	m_vecLong_AttPositionInfo.clear();
}