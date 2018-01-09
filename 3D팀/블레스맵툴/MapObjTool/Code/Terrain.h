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
#include "resource.h"

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
	struct Line3D
	{
		D3DXVECTOR3     Pos;
		D3DCOLOR        Col;
	};
private:
	CTerrain(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTerrain(void);

public:
	virtual HRESULT Initialize(int iCntX, int iCntZ, float fUVCn);
	virtual int Update(const float& fTimeDelta);
	virtual void Render(void);
public:
	Engine::VTXTEX*	GetTerrainVtx(void);
	HRESULT		SetTexture(void);
	HRESULT		SetHeight(void);
	void		SetNormal(void);
	HRESULT		ResetHeight(void);
	HRESULT ChangeHeight(D3DXVECTOR3 vPoint, float fHeight, DOUBLE dChangeSize);
	void ChangeNearHeight(D3DXVECTOR3 vPoint, float fHeight, DOUBLE dChangeSize);
	HRESULT DrawAlphamaps(D3DXVECTOR3 MousePos,int iIndex, float fBrushSize, float fAlphaRadius);
	HRESULT EraseAlphamaps(D3DXVECTOR3 MousePos,int iIndex, float fBrushSize, float fAlphaRadius);
	HRESULT CreateAlphaMaps(void);
	HRESULT DeleteAlphaMaps(void);
	HRESULT ResetAlphaMaps(void);

private:
	virtual HRESULT AddComponent(void);
	void DrawPickCircle(int iCount, float fSize, D3DXCOLOR Color);
	void MiniAlphaTextureRender(void);

public:
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
	bool					m_bTextureShow;
	unsigned int			m_iFilterCnt;
	unsigned int			m_iFilterViewCnt;
	Engine::CTexture*		m_pFilterTexture;
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, int iCntX = VTXCNTX, int iCntZ = VTXCNTZ, float fUVCn = 30.f);

private:
	virtual void Free(void);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pBrushTexture;
	Engine::CShader*		m_pShader;

private:
	Engine::VTXTEX*			m_pTerrainVtx;
	Engine::INDEX32*		m_pIndex;
	DWORD					m_dwTriCnt;
	LPDIRECT3DTEXTURE9* m_SaveAlphaTexture;

private:
	DWORD*					m_pdwPixel;
	int						m_iCntX;
	int						m_iCntZ;
	WORD					m_wItv;
	float					m_fUV;
	int						m_iTexPosX;
	int						m_iTexPosY;


private:
	D3DMATERIAL9		m_tMtrl;
};

#endif // Terrain_h__