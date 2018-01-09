/*!
 * \file Player.h
 * \date 2016/03/08 13:58
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

#ifndef Player_h__
#define Player_h__

#include "LandObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CShader;
};

class CPlayer
	: public CLandObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
private:
	Engine::CDynamicMesh*		m_pMesh;
	Engine::CShader*			m_pShader;
private:
	D3DMATERIAL9				m_tMtrl;

private:
	DWORD		m_dwIndex;
	bool		m_bPush;
	UINT		m_iAniIdx;
private:
	virtual HRESULT AddComponent(void);
	void KeyCheck(void);
	void Set_ContantTable(LPD3DXEFFECT	pEffect);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};


#endif // Player_h__
