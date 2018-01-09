/*!
 * \file Water.h
 * \date 2016/05/26 16:31
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

#ifndef Water_h__
#define Water_h__

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

class CWater
	: public Engine::CGameObject
{
private:
	CWater(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CWater(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pMeshKey = NULL);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CTexture*		m_pTextureNRM;
	Engine::CShader*		m_pShader;
	Engine::CMaterial*		m_pMtrlCom;

private:
	float					m_fWaveTime;

private:
	_float	m_fSpecPower;
	_float	m_fSpecRange;


private:
	void Set_ConstantTable(LPD3DXEFFECT		pEffect);

private:
	virtual void Free(void);
};

#endif // Water_h__