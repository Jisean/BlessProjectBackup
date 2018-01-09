/*!
 * \file Units.h
 * \date 2016/05/20 14:36
 *
 * \author �ǿ���
 * Contact: user@company.com
 *
 * \brief Unit ���� �θ�
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Units_h__
#define Units_h__

#include "LandObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CShader;
	class CMaterial;
	class CStatus;
	class CTransform;
	class CVIBuffer;
}

class CCalculator;
class CUnits : public CLandObject
{
	// ���� Ÿ��
public:
	enum TYPEID {TYPEID_PLAYER, TYPEID_MONSTER, TYPEID_BOSS, TYPEID_SUMMON, TYPEID_NPC};

protected:
	explicit CUnits(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CUnits(void);

protected:
	Engine::CDynamicMesh*		m_pMesh;
	Engine::CShader*			m_pShader;
	Engine::CStatus*			m_pStatus;
	CCalculator*				m_pCalculatorCom;
	Engine::CMaterial*			m_pMtrlCom;
	D3DXVECTOR3					m_vLook;
protected:		
	// Box ����
	vector<Engine::CVIBuffer*>			m_vecBoundingBox;
	vector<D3DXMATRIX*>					m_vecMatrixBox;
	vector<D3DXMATRIX*>					m_vecOriMat;
	// Sphere ����
	vector<LPD3DXMESH>					m_vecMesh;
	vector<D3DXMATRIX*>					m_vecMatrixSphere;
	// Animation ����
	vector<Engine::ANI_INFO*>			m_vecAniInfo;


	// ���� Ÿ��
	TYPEID								m_eUnitType;

public:
	virtual HRESULT Initialize(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {};
	virtual void Free(void);
public: // ------------- Get -------------//
	const Engine::CTransform*	GetInfo(){ return m_pInfo;}
	const Engine::CStatus*		GetStatus(){ return m_pStatus;}
	const Engine::CDynamicMesh*	GetMesh(){ return m_pMesh;}


	TYPEID	Get_UnitType(void) { return m_eUnitType; };
public: // ------------- Set -------------//
	void	SetAniInfo(vector<Engine::ANI_INFO*> pAniInfo);
	void	SetBoxInfo(vector<Engine::BOX_INFO*>* pData);
	void	SphereColor(LPDIRECT3DDEVICE9* ppDevice, LPD3DXMESH* ppMesh);  // Sphere ���� ǥ��
	void	BoxRender(void);
protected:
	void SetAniSpeed(float fAniSpeed = 1.f);
	void Set_ShadowCheck(LPD3DXEFFECT pEffect, D3DXMATRIX* pWorldMatrix);


protected:
	//============ �߰� ===============//
	vector<Engine::ATTPOSITION*>		m_vecAttPositionInfo;				// �ٰŸ� ���� ��ǥ
	vector<Engine::ATTPOSITION*>		m_vecLong_AttPositionInfo;			// ���Ÿ� ���� ��ǥ
	vector<float>						m_vecAttDist;						// ���� �Ÿ�

	bool								m_bAttPositionFull;
	bool								m_bLong_AttPositionFull;


public:
	//========Get========//
	vector<Engine::ATTPOSITION*>*	Get_AttackPosition(void) { return &m_vecAttPositionInfo; };
	vector<Engine::ATTPOSITION*>*	Get_LongAttackPosition(void) { return &m_vecLong_AttPositionInfo; };

	bool							Get_AttPositionFull(void) { return m_bAttPositionFull; };
	bool							Get_Long_AttPositionFull(void) { return m_bLong_AttPositionFull; };
	//========Set========//
	void							Set_AttPositionFull(bool bType) { m_bAttPositionFull = bType; };
	void							Set_Long_AttPositionFull(bool bType) { m_bLong_AttPositionFull = bType; };
	//======General======//
	void	AttackPositionInit(void);
	void	AttackPositionSetting(void);
};

#endif // Units_h__
