#include "NaviCell.h"
#include "Line2D.h"

Engine::CNaviCell::CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
: m_pDevice(NULL)
, m_pLine(NULL)
, m_dwIndex(0)
, m_dwType(0)
{
	ZeroMemory(m_pLine2D, sizeof(CLine2D*) * LINE_END);
	ZeroMemory(m_pNeighbor, sizeof(CNaviCell*) * NEIGHBOR_END);
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;
}

Engine::CNaviCell::~CNaviCell(void)
{
	
}

D3DXVECTOR3* Engine::CNaviCell::GetPoint(POINT ePointID)
{
	return &m_vPoint[ePointID];
}
DWORD Engine::CNaviCell::GetType(void)
{
	return m_dwType;
}

Engine::CNaviCell* Engine::CNaviCell::GetNeighbor(NEIGHBOR eNeighbor)
{
	return m_pNeighbor[eNeighbor];
}

DWORD Engine::CNaviCell::GetIndex(void)
{
	return m_dwIndex;
}

Engine::CLine2D* Engine::CNaviCell::GetLine(LINE eLineID)
{
	return m_pLine2D[eLineID];
}

void Engine::CNaviCell::SetPoint(D3DXVECTOR3 vDest, POINT ePointID)
{
	m_vPoint[ePointID] = vDest;
}

void Engine::CNaviCell::SetNeighbor(NEIGHBOR eNeighborID, CNaviCell* pNeighbor)
{
	m_pNeighbor[eNeighborID] = pNeighbor;
}

void Engine::CNaviCell::SetIndex(DWORD dwIndex)
{
	m_dwIndex = dwIndex;
}

HRESULT Engine::CNaviCell::Init_Cell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx, const DWORD& dwType)
{
	m_pDevice = pDevice;
	m_dwIndex = dwIdx;
	m_dwType = dwType;

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);

	FAILED_CHECK(D3DXCreateLine(m_pDevice, &m_pLine));
	return S_OK;
}

void Engine::CNaviCell::Render(void)
{
	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3			vPoint[4];
	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];

	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if(vPoint[i].z < 0.f)
			vPoint[i].z = 0.f;
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	m_pLine->SetWidth(2.f);

	m_pLine->Begin();
	if(m_dwType == TYPE_TERRAIN)
	{
		if(m_pNeighbor[NEIGHBOR_AB] != NULL && m_pNeighbor[NEIGHBOR_BC] != NULL && m_pNeighbor[NEIGHBOR_CA] != NULL)
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		else if(m_pNeighbor[NEIGHBOR_AB] == NULL && m_pNeighbor[NEIGHBOR_BC] == NULL && m_pNeighbor[NEIGHBOR_CA] == NULL)
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 0.5f, 0.f, 1.f));
		else
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if(m_dwType == TYPE_CELL)
	{
		if(m_pNeighbor[NEIGHBOR_AB] != NULL && m_pNeighbor[NEIGHBOR_BC] != NULL && m_pNeighbor[NEIGHBOR_CA] != NULL)
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(0.f, 1.f, 1.f, 1.f));
		else if(m_pNeighbor[NEIGHBOR_AB] == NULL && m_pNeighbor[NEIGHBOR_BC] == NULL && m_pNeighbor[NEIGHBOR_CA] == NULL)
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		else
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(0.f, 0.5f, 1.f, 1.f));
	}
	else if(m_dwType == TYPE_MESH)
	{
		if(m_pNeighbor[NEIGHBOR_AB] != NULL && m_pNeighbor[NEIGHBOR_BC] != NULL && m_pNeighbor[NEIGHBOR_CA] != NULL)
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
		else if(m_pNeighbor[NEIGHBOR_AB] == NULL && m_pNeighbor[NEIGHBOR_BC] == NULL && m_pNeighbor[NEIGHBOR_CA] == NULL)
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(1.f, 0.5f, 1.f, 1.f));
		else
			m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	}
	m_pLine->End();
}

Engine::CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pDevice 
											 , const D3DXVECTOR3* pPointA
											 , const D3DXVECTOR3* pPointB
											 , const D3DXVECTOR3* pPointC 
											 , const DWORD& dwIdx
											 , const DWORD& dwType)
{
	CNaviCell*	pCell = new CNaviCell(pPointA, pPointB, pPointC);
	if(FAILED(pCell->Init_Cell(pDevice, dwIdx, dwType)))
		Safe_Release(pCell);

	return pCell;
}

void Engine::CNaviCell::Free(void)
{
	for(int i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine2D[i]);

	Safe_Release(m_pLine);
}

bool Engine::CNaviCell::ComparePoint(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, CNaviCell* pNeighbor)
{
	if(*pPointA == m_vPoint[POINT_A])
	{
		if(*pPointB == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pPointB == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
	}

	if(*pPointA == m_vPoint[POINT_B])
	{
		if(*pPointB == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pPointB == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	if(*pPointA == m_vPoint[POINT_C])
	{
		if(*pPointB == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else if(*pPointB == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}
	return false;
}

bool Engine::CNaviCell::CheckPass(const D3DXVECTOR3* pPos 
								  , const D3DXVECTOR3* pDir 
								  , NEIGHBOR* peNeighborID)
{
	for(int i = 0; i < 3; ++i)
	{
		D3DXVECTOR2		vEndPoint = D3DXVECTOR2(pPos->x + pDir->x, pPos->z + pDir->z);
		if(CLine2D::DIR_LEFT == m_pLine2D[i]->Check_Dir(&vEndPoint))
		{
			*peNeighborID = NEIGHBOR(i);
			return true;
		}
	}

	return false;
}
bool Engine::CNaviCell::CheckPass(const D3DXVECTOR3* pPos , const D3DXVECTOR3* pDir , DWORD dwNeighborID)
{
	D3DXVECTOR2		vEndPoint = D3DXVECTOR2(pPos->x + pDir->x, pPos->z + pDir->z);
	if(CLine2D::DIR_LEFT == m_pLine2D[dwNeighborID]->Check_Dir(&vEndPoint))
	{
		return true;
	}

	return false;
}

D3DXVECTOR3 Engine::CNaviCell::SlidingVector(D3DXVECTOR3& vDir, DWORD dwNeighborID)
{
	D3DXVECTOR2 vTemp = D3DXVECTOR2(vDir.x, vDir.z);
	D3DXVECTOR2 vOut;
	vOut = vTemp - m_pLine2D[(LINE)dwNeighborID]->m_vNormal * D3DXVec2Dot(&m_pLine2D[(LINE)dwNeighborID]->m_vNormal, &vTemp);
	D3DXVECTOR3 vOut3 = D3DXVECTOR3(vOut.x, 0.f, vOut.y);
	return vOut3;
}

