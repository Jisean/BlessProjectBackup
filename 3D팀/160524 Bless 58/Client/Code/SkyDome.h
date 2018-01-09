/*!
 * \file SkyDome.h
 * \date 2016/05/25 17:22
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

#ifndef SkyDome_h__
#define SkyDome_h__


#include "MapObjects.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
};

class CCalculator;
class CSkyDome
	: public CMapObjects
{
private:
	explicit CSkyDome(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkyDome(void);

public:
	virtual HRESULT Initialize(const TCHAR* pMeshKey);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(const TCHAR* pMeshKey);

public:
	static CSkyDome* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey = NULL);
private:
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);

private:
	virtual void Free(void);

public:
	void	Set_Top(_bool bTop){ m_bTop = bTop; };

private:
	Engine::CShader*		m_pVolumetricShader;
	_bool					m_bTop;
};

#endif // SkyDome_h__