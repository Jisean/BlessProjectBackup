/*!
 * \file Terrain.h
 * \date 2016/02/23 13:24
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

#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};

class CTerrain
	: public Engine::CGameObject
{
private:
	CTerrain(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTerrain(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void Free(void);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CTexture*		m_pFilterTexture;
	Engine::CTexture*		m_pBrushTexture;
	Engine::CShader*		m_pShader;

private:
	Engine::VTXTEX*			m_pTerrainVtx;
	Engine::INDEX32*		m_pIndex;
	LPDIRECT3DTEXTURE9		m_pMyFilterTex;

	DWORD					m_dwTriCnt;

private:
	D3DMATERIAL9		m_tMtrl;
};

#endif // Terrain_h__