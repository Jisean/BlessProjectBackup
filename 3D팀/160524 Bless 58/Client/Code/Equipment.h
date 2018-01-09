/*!
 * \file Bow.h
 * \date 2016/03/14 14:33
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

#ifndef Equipment_h__
#define Equipment_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CComSphere;
	class CShader;
	class CMaterial;
};
class CEquipment
	: public Engine::CGameObject
{
protected:
	explicit CEquipment(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEquipment(void);
public:
	_vec3 GetTrailPos(int iNum){ return m_vTrailPos[iNum]; }
protected:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta){return 0;}
	virtual void Render(void);
	void Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix = NULL);
	void Set_ConstantTable(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix, const D3DLIGHT9* pLightInfo, const D3DMATERIAL9* pMtrlInfo);
	_bool Clear_Render(void);
private:
	virtual HRESULT AddComponent(void) {return S_OK;}
public:
	virtual void Free(void);
protected:
	Engine::CStaticMesh*		m_pMesh;
	Engine::CTransform*			m_pInfo;
	Engine::CComSphere*			m_pComSphere;
	Engine::CShader*			m_pShader;
	Engine::CMaterial*			m_pMtrlCom;
	_vec3						m_vPoint[2];
	//트레일 이펙트
	_vec3						m_vTrailPos[2];
	float						m_fTrailDistance;
protected:
	const D3DXMATRIX*		m_pmatPlayerInfo;
	const D3DXMATRIX*		m_pmatWeaponRef;
	D3DXMATRIX*				m_matSphere;
};


#endif // Equipment_h__