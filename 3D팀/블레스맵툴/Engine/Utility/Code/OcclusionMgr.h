/*!
 * \file Occlusion.h
 * \date 2016/06/11 2:11
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

#ifndef OcclusionMgr_h__
#define OcclusionMgr_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CCubeColor;
class CTransform;
class COcclusionBox;
class ENGINE_DLL COcclusionMgr : public CBase
{
public:
	DECLARE_SINGLETON(COcclusionMgr)

private:
	COcclusionMgr(void);
	~COcclusionMgr(void);

public:
	HRESULT InitOcclusion(LPDIRECT3DDEVICE9 pDevice);
	void	Set_OccBox(const _uint iIndex, CTransform* pInfo);
	void	Get_MinMax(const _uint iIndex, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax, D3DXVECTOR3* pCenter = NULL);
	CTransform* Get_OccBox(const _uint iIndex);
	CCubeColor* Get_OccBoxVtx(const _uint iIndex);
	HRESULT Add_OccBox(const D3DXMATRIX* pmatWorld);
	void	Update(void);
	void	Render_OccBox(void);
	_uint	GetVecOccBoxSize(void){ return m_vecOccBox.size(); }
	void	Delete_OccBox(const _uint iIndex);
	void	SetInOccBox(CGameObject* pObject);

private:
	typedef vector<COcclusionBox*>		OCCBOXVEC;
	OCCBOXVEC				m_vecOccBox;
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	virtual void Free(void);
};

END

#endif // OcclusionMgr_h__