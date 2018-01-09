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
	class CMaterial;
};

class CTerrain
	: public Engine::CGameObject
{
private:
	CTerrain(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTerrain(void);

public:
	virtual HRESULT Initialize(WORD& wStageId);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);
	void Culling_Frustum(void);
	void Culling_QuadTree(void);
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);
protected:
	void Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix);
	void LoadTerrain(void);
	void LoadTerrain2(void);
	void SetNormal(void);


public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, WORD wStageId);
private:
	virtual void Free(void);
	

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CTexture*		m_pFilterTexture;
	Engine::CTexture*		m_pBrushTexture;
	Engine::CTexture*		m_pNormTexture;
	Engine::CShader*		m_pShader;
	const _vec3*			pVtxPos;

private:
	Engine::VTXTEX*			m_pTerrainVtx;
	Engine::INDEX32*		m_pIndex;
	LPDIRECT3DTEXTURE9		m_pMyFilterTex;

	DWORD					m_dwTriCnt;
	Engine::CMaterial*		m_pMtrlCom;
	float					m_fUVCnt;
};

#endif // Terrain_h__