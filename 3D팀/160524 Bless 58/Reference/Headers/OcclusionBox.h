/*!
 * \file OcclusionBox.h
 * \date 2016/06/11 1:09
 *
 * \author ±«ø¿»∆
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef OcclusionBox_h__
#define OcclusionBox_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CTransform;
class CCubeColor;
class ENGINE_DLL COcclusionBox : public CBase
{
	enum RESOURCETYPE {RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_STATIC, RESOURCE_TOOL, RESOURCE_END};
private:
	explicit COcclusionBox(LPDIRECT3DDEVICE9 pDevice, const _uint iIndex);
	virtual ~COcclusionBox(void);

private:
	bool				m_bFrustumCulled;
	bool				m_bRender;
	unsigned int		m_iIndex;
	LPDIRECT3DDEVICE9	m_pDevice;
	D3DXVECTOR3			m_vMax;
	D3DXVECTOR3			m_vMin;
	CCubeColor*			m_pOccBox;
	CTransform*			m_pInfo;
	D3DXVECTOR3			m_vVtxPos[8];
	vector<CGameObject*>	m_vecObject;

public:
	void		Update(void);
	void		SetRender(bool bRender){ m_bRender = bRender; }
	bool		GetRender(void){ return m_bRender; }
	HRESULT		InitOcclusionBox(const D3DXMATRIX* pmatWorld);
	void		SetOccBox(CTransform* pInfo);
	CTransform* GetOccBox(void);
	CCubeColor*	GetOccBoxVtx(void);
	void		GetMinMax(D3DXVECTOR3* pMin, D3DXVECTOR3* pMax);
	void		RenderOccBox(void);
	void		SetInOccBox(CGameObject* pObject);
	void		SetVtxPos(void);
	void		GetVtxPos(D3DXVECTOR3* pVtxPos);
	void		GetOccVtxPos(WORD wAngle, D3DXVECTOR3* vOccPos1, D3DXVECTOR3* vOccPos2, D3DXVECTOR3* vOccPos3, D3DXVECTOR3* vOccPos4);
	static COcclusionBox* Create(LPDIRECT3DDEVICE9 pDevice, const _uint iIndex, const D3DXMATRIX* pmatWorld);
	virtual void	Free(void);
};

END

#endif // OcclusionBox_h__