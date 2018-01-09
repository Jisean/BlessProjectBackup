/*!
 * \file MapObjects.h
 * \date 2016/05/20 15:01
 *
 * \author ±Ç¿ÀÈÆ
 * Contact: user@company.com
 *
 * \brief MapObject ºÎ¸ð
 *
 * TODO: long description
 *
 * \note
*/
#ifndef MapObjects_h__
#define MapObjects_h__

#include "LandObject.h"

namespace Engine
{
	class CStaticMesh;
	class CShader;
	class CMaterial;
}

class CCalculator;
class CMapObjects : public CLandObject
{
protected:
	explicit CMapObjects(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMapObjects(void);

protected:
	Engine::CStaticMesh*		m_pMesh;
	Engine::CShader*			m_pShader;
	CCalculator*				m_pCalculatorCom;
	Engine::CMaterial*			m_pMtrlCom;
	float						m_foth;
	const Engine::CTransform*	m_pPlayerInfo;
public: // ------------- Get -------------//
	const Engine::CTransform*	GetInfo(){ return m_pInfo;}
	const Engine::CStaticMesh*	GetMesh(){ return m_pMesh;}
public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};
protected:
	void Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix = NULL);
};

#endif // MapObjects_h__
