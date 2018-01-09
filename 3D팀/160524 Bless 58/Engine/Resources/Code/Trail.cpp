#include "Trail.h"

Engine::CTrail::CTrail(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CTrail::~CTrail(void)
{

}

Engine::CResources* Engine::CTrail::CloneResource(void)
{
	/*CResources* pResource = new CTrail(*this);

	pResource->AddRef();

	return pResource;*/

	return new CTrail(*this);
}

HRESULT Engine::CTrail::CreateBuffer()
{
	// LINESIZE = iPointCount * iLineCount	

	for(int i = 0; i < LINECOUNT; ++i)
	{
		m_vPoint[i] = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_vSubPoint[i] = D3DXVECTOR3(0.f, 0.f, 0.f);
	}

	//m_vPoint[0]	= D3DXVECTOR3( 0.f, 0.f, 0.f );	
	//m_vPoint[1]	= D3DXVECTOR3( 0.f, 0.f, 0.f );
	//m_vPoint[2]	= D3DXVECTOR3( 0.f, 0.f, 0.f );
	//m_vPoint[3]	= D3DXVECTOR3( 0.f, 0.f, 0.f );

	//m_vSubPoint[0]	= D3DXVECTOR3( 0.f, 0.f, 0.f );
	//m_vSubPoint[1]	= D3DXVECTOR3( 0.f, 0.f, 0.f );
	//m_vSubPoint[2]	= D3DXVECTOR3( 0.f, 0.f, 0.f );
	//m_vSubPoint[3]	= D3DXVECTOR3( 0.f, 0.f, 0.f );

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = LINESIZE * 2;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = (LINESIZE - 1) * 2;
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*			pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	for ( int i = 0; i < LINESIZE * 2; i++ )
	{
		pVtxTex[i].vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		pVtxTex[i].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		//float S =  (float)i / (LINESIZE/3);
		//D3DXVec3CatmullRom( &m_vRomLine[i], &m_vPoint[0], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2], S );
		//D3DXVec3CatmullRom( &m_vRomLine[i+(LINESIZE/3)], &m_vPoint[0], &m_vPoint[1],& m_vPoint[2], &m_vPoint[3], S );
		//D3DXVec3CatmullRom( &m_vRomLine[i+(LINESIZE/3) * 2], &m_vPoint[1], &m_vPoint[2],& m_vPoint[3], &m_vPoint[3], S );

		//D3DXVec3CatmullRom( &m_vRomLine[i+(LINESIZE/3) * 3], &m_vSubPoint[0], &m_vSubPoint[0], &m_vSubPoint[1], &m_vSubPoint[2], S );
		//D3DXVec3CatmullRom( &m_vRomLine[i+(LINESIZE/3) * 4], &m_vSubPoint[0], &m_vSubPoint[1],& m_vSubPoint[2], &m_vSubPoint[3], S );
		//D3DXVec3CatmullRom( &m_vRomLine[i+(LINESIZE/3) * 5], &m_vSubPoint[1], &m_vSubPoint[2],& m_vSubPoint[3], &m_vSubPoint[3], S );
		
		/*pVtxTex[i].vPos		= m_vRomLine[i];
		pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);		

		pVtxTex[i+(LINESIZE/3)].vPos		= m_vRomLine[i+(LINESIZE/3)];
		pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

		pVtxTex[i+(LINESIZE/3) * 2].vPos		= m_vRomLine[i+(LINESIZE/3) * 2];
		pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

		pVtxTex[i+(LINESIZE/3) * 3].vPos		= m_vRomLine[i+(LINESIZE/3) * 3];
		pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

		pVtxTex[i+(LINESIZE/3) * 4].vPos		= m_vRomLine[i+(LINESIZE/3) * 4];
		pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

		pVtxTex[i+(LINESIZE/3) * 5].vPos		= m_vRomLine[i+(LINESIZE/3) * 5];
		pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
		pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);*/
	}

	for(int z = 0; z < 2; ++z)
	{
		for(int x = 0; x < LINESIZE; ++x)
		{	
			int iIndex = z * LINESIZE + x;
			pVtxTex[iIndex].vTexUV = D3DXVECTOR2( x / (LINESIZE - 1.f), z / (2 - 1.f) );
		}
	}

	m_pVB->Unlock();

	/*m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);
	for(int i = 0; i < LINESIZE; ++i)
	{
		S_RomLine[i + LINESIZE].color					= 0xffff8080;
		S_RomLine[i + LINESIZE].rhw					= 1.0f;

		float fY = (S_RomLine[i+1].vPos.y - S_RomLine[i].vPos.y);

		float fX = (S_RomLine[i+1].vPos.x - S_RomLine[i].vPos.x);		
		
		float fGiul = fY / fX;		
		
		float	fZul = S_RomLine[i].vPos.y - (fGiul * S_RomLine[i].vPos.x);
		
		fZul += 10;

		S_RomLine[i + LINESIZE].vPos.x =  S_RomLine[i].vPos.x;
		S_RomLine[i + LINESIZE].vPos.y =  (fGiul * S_RomLine[i].vPos.x) + fZul;

		pVtxCol[i + LINESIZE].vPos		= S_RomLine[i + LINESIZE].vPos;
		pVtxCol[i + LINESIZE].color	= 0xffff8080;
		pVtxCol[i + LINESIZE].rhw	=	1.0f;

	}*/

	/*pVtxTex[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxTex[1].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxTex[2].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxTex[3].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);*/

	//m_pVB->Unlock();

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int		iTriCnt = 0;
	int		iIndex = 0;

	for(int z = 0; z < 2 -1 ; ++z)
	{
		for(int x = 0; x < LINESIZE - 1; ++x)
		{
			iIndex = z * LINESIZE + x;

			pIndex[iTriCnt]._1 = iIndex + LINESIZE;
			pIndex[iTriCnt]._2 = iIndex + LINESIZE + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;

			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + LINESIZE;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			++iTriCnt;
		}
	}	

	m_pIB->Unlock();

	return S_OK;
}

Engine::CTrail* Engine::CTrail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTrail*		pTrail = new CTrail(pDevice);

	if(FAILED(pTrail->CreateBuffer()))
		Engine::Safe_Release(pTrail);		

	return pTrail;
}

void Engine::CTrail::Free(void)
{
	Engine::CVIBuffer::Free();
}
