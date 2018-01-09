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

typedef struct _OCCLUSION
{
	D3DXVECTOR3	vPlane[4];
	float		MinZ;
	D3DXPLANE	FrontPlane;
	D3DXPLANE	FirstPlane;
	D3DXPLANE	SecondPlane;
	D3DXPLANE	ThirdPlane;
	D3DXPLANE	FourthPlane;
}OCCLUSION;

class CGameObject;
class CCubeColor;
class CTransform;
class COcclusionBox;
class ENGINE_DLL COcclusionMgr : public CBase
{
public:
	enum OCCANGLE{ OCCANGLE_1, OCCANGLE_2, OCCANGLE_3, OCCANGLE_4, OCCANGLE_END };
public:
	DECLARE_SINGLETON(COcclusionMgr)

private:
	COcclusionMgr(void);
	~COcclusionMgr(void);

public:
	HRESULT InitOcclusion(LPDIRECT3DDEVICE9 pDevice);
	void	Set_OccBox(const _uint iIndex, CTransform* pInfo);
	void	Set_VtxPos(const _uint iIndex);
	CTransform* Get_OccBox(const _uint iIndex);
	CCubeColor* Get_OccBoxVtx(const _uint iIndex);
	HRESULT Add_OccBox(const D3DXMATRIX* pmatWorld);
	void	Update(void);
	void	Render_OccBox(void);
	_uint	GetVecOccBoxSize(void){ return m_vecOccBox.size(); }
	void	Delete_OccBox(const _uint iIndex);
	bool	SetInOccBox(CGameObject* pObject);
	int		TestInOccBox(D3DXVECTOR3 vPos);
private:
	void	SetupOcclusion(void);
	void	OcclusionCulling(void);
	bool	InOcclusion(OCCLUSION* pOcc, D3DXVECTOR3 vPosition, float fRadius);
private:
	typedef vector<COcclusionBox*>		OCCBOXVEC;
	OCCBOXVEC				m_vecOccBox;
	typedef vector<OCCLUSION*>			OCCVEC;
	OCCVEC					m_vecOccluder;
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	virtual void Free(void);
};

END

#endif // OcclusionMgr_h__