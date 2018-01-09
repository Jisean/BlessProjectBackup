#include "NaviMgr.h"
#include "NaviCell.h"
#include "Line2D.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr(void)
: m_pDevice(NULL)
, m_dwReserveSize(0)
, m_dwIdxCnt(0)
{

}

Engine::CNaviMgr::~CNaviMgr(void)
{
	
}

void Engine::CNaviMgr::Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_dwReserveSize = dwSize;
	m_vecNaviMesh.reserve(dwSize);
}

void Engine::CNaviMgr::Free(void)
{
	for_each(m_vecNaviMesh.begin(), m_vecNaviMesh.end(), CDeleteObj());
	m_vecNaviMesh.clear();
	Safe_Release(m_pDevice);
}

HRESULT Engine::CNaviMgr::Add_Cell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC, const DWORD& dwType)
{
	if(m_dwReserveSize == 0)
		return E_FAIL;

	CNaviCell*		pCell = CNaviCell::Create(m_pDevice, pPointA, pPointB, pPointC, m_dwIdxCnt, dwType);
	if(pCell == NULL)
		return E_FAIL;

	++m_dwIdxCnt;

	m_vecNaviMesh.push_back(pCell);
	return S_OK;
}

void Engine::CNaviMgr::Render(void)
{
	UINT		iSize = m_vecNaviMesh.size();

	for(UINT i = 0; i < iSize; ++i)
	{
		m_vecNaviMesh[i]->Render();
	}
}

void Engine::CNaviMgr::Link_Cell(void)
{
	VECCELL::iterator	iter = m_vecNaviMesh.begin();

	if(m_vecNaviMesh.end() == iter)
		return;

	for( ; iter != m_vecNaviMesh.end(); ++iter)
	{
		VECCELL::iterator	iter_Target = m_vecNaviMesh.begin();

		while(iter_Target != m_vecNaviMesh.end())
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if( (*iter_Target)->ComparePoint( (*iter)->GetPoint(CNaviCell::POINT_A)
				, (*iter)->GetPoint(CNaviCell::POINT_B), (*iter) ) )
			{
				(*iter)->SetNeighbor(CNaviCell::NEIGHBOR_AB, (*iter_Target));
			}
			else if( (*iter_Target)->ComparePoint( (*iter)->GetPoint(CNaviCell::POINT_B)
				, (*iter)->GetPoint(CNaviCell::POINT_C), (*iter) ) )
			{
				(*iter)->SetNeighbor(CNaviCell::NEIGHBOR_BC, (*iter_Target));
			}
			else if( (*iter_Target)->ComparePoint( (*iter)->GetPoint(CNaviCell::POINT_C)
				, (*iter)->GetPoint(CNaviCell::POINT_A), (*iter) ) )
			{
				(*iter)->SetNeighbor(CNaviCell::NEIGHBOR_CA, (*iter_Target));
			}

			++iter_Target;
		}
	}
}

DWORD Engine::CNaviMgr::MoveOnNaviMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir , const DWORD& dwCurrentIdx, float fHeight)
{
	CNaviCell::NEIGHBOR		Neighbor;
	DWORD		dwNextIndex = dwCurrentIdx;

	bool		bPassNeighbor[CNaviCell::NEIGHBOR_END] = {false, false, false};
	for(int i = 0; i < CNaviCell::NEIGHBOR_END; ++i)
	{
		bPassNeighbor[i] = m_vecNaviMesh[dwCurrentIdx]->CheckPass(pPos, pDir, (DWORD)i);
	}
	int iCnt = 0;
	for(int i = 0; i < CNaviCell::NEIGHBOR_END; ++i)
	{
		if(bPassNeighbor[i])
			++iCnt;
	}

	if(m_vecNaviMesh[dwCurrentIdx]->CheckPass(pPos, pDir, &Neighbor))
	{
		CNaviCell*		pNeighbor = m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(Neighbor);
		if(pNeighbor == NULL)
		{
			// 슬라이딩 벡터
			if(iCnt == 1)
			{
				/*D3DXVECTOR3 vDir = *(const_cast<D3DXVECTOR3*>(pDir));

				vDir = m_vecNaviMesh[dwCurrentIdx]->SlidingVector(vDir, Neighbor);
				*pPos += vDir;*/

				D3DXVECTOR3 vDir = *(const_cast<D3DXVECTOR3*>(pDir));

				D3DXVECTOR3 vPoint[3];
				D3DXVECTOR3 vLine[2];
				D3DXVECTOR3 vCrossLine, vPlane;
				float fY;

				D3DXVec3Normalize(&vCrossLine, &vCrossLine);
				D3DXVec3Normalize(&vPlane, &vPlane);
				vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
				vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
				vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

				vLine[0] = vPoint[0] - vPoint[1];
				vLine[1] = vPoint[0] - vPoint[2];

				D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

				fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;

				vDir = m_vecNaviMesh[dwCurrentIdx]->SlidingVector(vDir, Neighbor);
				*pPos += vDir;
				pPos->y = fY;

			}
			else if(iCnt == 2)
			{
				D3DXVECTOR2 vPos = D3DXVECTOR2((*pPos).x, (*pPos).z);
				D3DXVECTOR3 vDir = *(const_cast<D3DXVECTOR3*>(pDir));
				vDir = m_vecNaviMesh[dwCurrentIdx]->SlidingVector(vDir, Neighbor);

				if(Neighbor == CNaviCell::NEIGHBOR_AB)
				{
					D3DXVECTOR2 vA = m_vecNaviMesh[dwCurrentIdx]->GetLine(CNaviCell::LINE_AB)->m_vStartPoint - vPos;
					D3DXVECTOR2 vB = m_vecNaviMesh[dwCurrentIdx]->GetLine(CNaviCell::LINE_AB)->m_vEndPoint - vPos;
					float fLengthA = D3DXVec2Length(&vA);
					float fLengthB = D3DXVec2Length(&vB);
					bool bCompare = true;
					if(fLengthA > fLengthB)
						bCompare = false;

					if(NULL != m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(CNaviCell::NEIGHBOR_BC) && bCompare == false)
					{
						D3DXVECTOR3 vPoint[3];
						D3DXVECTOR3 vLine[2];
						D3DXVECTOR3 vCrossLine;
						float fY;

						D3DXVec3Normalize(&vCrossLine, &vCrossLine);
						vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
						vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
						vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

						vLine[0] = vPoint[0] - vPoint[1];
						vLine[1] = vPoint[0] - vPoint[2];

						D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

						fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
						pPos->y = fY;
						*pPos += vDir;
					}
					else if(NULL != m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(CNaviCell::NEIGHBOR_CA) && bCompare == true)
					{
						D3DXVECTOR3 vPoint[3];
						D3DXVECTOR3 vLine[2];
						D3DXVECTOR3 vCrossLine;
						float fY;

						D3DXVec3Normalize(&vCrossLine, &vCrossLine);
						vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
						vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
						vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

						vLine[0] = vPoint[0] - vPoint[1];
						vLine[1] = vPoint[0] - vPoint[2];

						D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

						fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
						pPos->y = fY;
						*pPos += vDir;
					}
				}
				else if(Neighbor == CNaviCell::NEIGHBOR_BC)
				{
					D3DXVECTOR2 vB = m_vecNaviMesh[dwCurrentIdx]->GetLine(CNaviCell::LINE_BC)->m_vStartPoint - vPos;
					D3DXVECTOR2 vC = m_vecNaviMesh[dwCurrentIdx]->GetLine(CNaviCell::LINE_BC)->m_vEndPoint - vPos;
					float fLengthB = D3DXVec2Length(&vB);
					float fLengthC = D3DXVec2Length(&vC);
					bool bCompare = true;
					if(fLengthB > fLengthC)
						bCompare = false;

					if(NULL != m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(CNaviCell::NEIGHBOR_CA) && bCompare == false)
					{
						D3DXVECTOR3 vPoint[3];
						D3DXVECTOR3 vLine[2];
						D3DXVECTOR3 vCrossLine;
						float fY;

						D3DXVec3Normalize(&vCrossLine, &vCrossLine);
						vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
						vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
						vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

						vLine[0] = vPoint[0] - vPoint[1];
						vLine[1] = vPoint[0] - vPoint[2];

						D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

						fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
						pPos->y = fY;
						*pPos += vDir;
					}
					else if(NULL != m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(CNaviCell::NEIGHBOR_AB) && bCompare == true)
					{
						D3DXVECTOR3 vPoint[3];
						D3DXVECTOR3 vLine[2];
						D3DXVECTOR3 vCrossLine;
						float fY;

						D3DXVec3Normalize(&vCrossLine, &vCrossLine);
						vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
						vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
						vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

						vLine[0] = vPoint[0] - vPoint[1];
						vLine[1] = vPoint[0] - vPoint[2];

						D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

						fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
						pPos->y = fY;
						*pPos += vDir;
					}
				}
				else if(Neighbor == CNaviCell::NEIGHBOR_CA)
				{
					D3DXVECTOR2 vC = m_vecNaviMesh[dwCurrentIdx]->GetLine(CNaviCell::LINE_CA)->m_vStartPoint - vPos;
					D3DXVECTOR2 vA = m_vecNaviMesh[dwCurrentIdx]->GetLine(CNaviCell::LINE_CA)->m_vEndPoint - vPos;
					float fLengthC = D3DXVec2Length(&vC);
					float fLengthA = D3DXVec2Length(&vA);
					bool bCompare = true;
					if(fLengthC > fLengthA)
						bCompare = false;

					if(NULL != m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(CNaviCell::NEIGHBOR_AB) && bCompare == false)
					{
						D3DXVECTOR3 vPoint[3];
						D3DXVECTOR3 vLine[2];
						D3DXVECTOR3 vCrossLine;
						float fY;

						D3DXVec3Normalize(&vCrossLine, &vCrossLine);
						vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
						vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
						vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

						vLine[0] = vPoint[0] - vPoint[1];
						vLine[1] = vPoint[0] - vPoint[2];

						D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

						fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
						pPos->y = fY;
						*pPos += vDir;
					}
					else if(NULL != m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(CNaviCell::NEIGHBOR_BC) && bCompare == true)
					{
						D3DXVECTOR3 vPoint[3];
						D3DXVECTOR3 vLine[2];
						D3DXVECTOR3 vCrossLine;
						float fY;

						D3DXVec3Normalize(&vCrossLine, &vCrossLine);
						vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
						vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
						vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

						vLine[0] = vPoint[0] - vPoint[1];
						vLine[1] = vPoint[0] - vPoint[2];

						D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

						fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
						pPos->y = fY;
						*pPos += vDir;
					}
				}
			}
			return dwCurrentIdx;
		}
		else
		{
			dwNextIndex = pNeighbor->GetIndex();

			D3DXVECTOR3 vPoint[3];
			D3DXVECTOR3 vLine[2];
			D3DXVECTOR3 vCrossLine;
			//float fY;

			D3DXVec3Normalize(&vCrossLine, &vCrossLine);
			vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
			vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
			vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

			vLine[0] = vPoint[0] - vPoint[1];
			vLine[1] = vPoint[0] - vPoint[2];

			D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

			if(m_vecNaviMesh[dwCurrentIdx]->GetCellType() == TYPE_CELL)
			{
				pPos->y = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
			}
			else if(m_vecNaviMesh[dwCurrentIdx]->GetCellType() == TYPE_TERRAIN)
			{
				//pPos->y = fHeight;
			}
			*pPos += *pDir;
		}
	}
	else
	{
		D3DXVECTOR3 vPoint[3];
		D3DXVECTOR3 vLine[2];
		D3DXVECTOR3 vCrossLine;
		//float fY;

		D3DXVec3Normalize(&vCrossLine, &vCrossLine);
		vPoint[0] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_A);
		vPoint[1] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_B);
		vPoint[2] = *m_vecNaviMesh[dwCurrentIdx]->GetPoint(CNaviCell::POINT_C);

		vLine[0] = vPoint[0] - vPoint[1];
		vLine[1] = vPoint[0] - vPoint[2];
		D3DXVec3Cross(&vCrossLine, &vLine[0], &vLine[1]);

		

		if(m_vecNaviMesh[dwCurrentIdx]->GetCellType() == TYPE_CELL)
		{
			//fY = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
			pPos->y = ( (-(pPos->x * vCrossLine.x )) + (vPoint[0].x * vCrossLine.x) + (vPoint[0].y * vCrossLine.y) - (pPos->z * vCrossLine.z) + (vPoint[0].z * vCrossLine.z) ) / vCrossLine.y;
		}
		else if(m_vecNaviMesh[dwCurrentIdx]->GetCellType() == TYPE_TERRAIN)
		{
			//pPos->y = fHeight;
		}
		*pPos += *pDir;
	}

	return dwNextIndex;
}

DWORD Engine::CNaviMgr::GetCellIndex(D3DXVECTOR3* pPos)
{
	int iCellCnt = 0;

	VECCELL::iterator iter		= m_vecNaviMesh.begin();
	VECCELL::iterator iter_end	= m_vecNaviMesh.end();
	bool		bPassNeighbor[CNaviCell::NEIGHBOR_END] = {false, false, false};

	for(iter; iter != iter_end; ++iter)
	{
		for(int i = 0; i < CNaviCell::NEIGHBOR_END; ++i)
		{
			bPassNeighbor[i] = (*iter)->CheckPass(pPos, &(D3DXVECTOR3(0.f, 0.f, 0.f)), i);
			if(bPassNeighbor[i] == false)
				++iCellCnt;

			if(iCellCnt == 3)
				break;
		}

		if(iCellCnt == 3)
			break;
		else
			iCellCnt = 0;
	}
	if(iter == iter_end)
		return 0;

	return (*iter)->GetIndex();
}

DWORD Engine::CNaviMgr::GetCellType(DWORD dwIndex)
{
	return m_vecNaviMesh[dwIndex]->GetCellType();
}
