#include "ComSphere.h"
#include "GraphicDev.h"

USING(Engine)

Engine::CComSphere::CComSphere(void)
: m_pDevice(CGraphicDev::GetInstance()->GetDevice())
, m_pSphereMesh(NULL)
, m_fRadius(0.f)
, m_vPos(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matCombinedMatrix);
}	

Engine::CComSphere::~CComSphere(void)
{

}

HRESULT Engine::CComSphere::Initialize( const D3DXMATRIX& matCombinedMatrix, float fRadius, const D3DXVECTOR3& vPos )
{
	// ±¸ »ý¼º
	
	m_vPos = vPos;
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	D3DXMatrixTranslation(&matIdentity, m_vPos.x, m_vPos.y, m_vPos.z);
	m_matCombinedMatrix = matCombinedMatrix * matIdentity;

	LPD3DXMESH	pMesh;
	D3DXCreateSphere(m_pDevice, fRadius, 16, 8, &pMesh, NULL);

	pMesh->CloneMeshFVF(0, Engine::VTXFVF_COL, m_pDevice, &pMesh);

	LPDIRECT3DVERTEXBUFFER9 pTempBuffer;
	pMesh->GetVertexBuffer(&pTempBuffer);

	int num = pMesh->GetNumVertices();

	Engine::VTXCOL* pVercol = NULL;

	DWORD color = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pTempBuffer->Lock(0,0,(void**)&pVercol, 0);

	for(int i = 0; i < num; ++i)
		pVercol[i].dwColor = color;

	pTempBuffer->Unlock();

	m_pSphereMesh = pMesh;

	return S_OK;
}

void Engine::CComSphere::Update(void)
{
	
}

void Engine::CComSphere::Render(const D3DXMATRIX* pmatWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_matCombinedMatrix * (*pmatWorld)));

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pSphereMesh->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

Engine::CComSphere* Engine::CComSphere::Create( const D3DXMATRIX& matCombinedMatrix, float fRadius, const D3DXVECTOR3& vPos )
{
	CComSphere* pComShere = new CComSphere();

	if(FAILED(pComShere->Initialize(matCombinedMatrix, fRadius, vPos)))
		Engine::Safe_Delete(pComShere);

	return pComShere;
}

void Engine::CComSphere::Free(void)
{
	
}

