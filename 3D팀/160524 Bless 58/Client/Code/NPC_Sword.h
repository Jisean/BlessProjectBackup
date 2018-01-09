#ifndef NPC_Sword_h__
#define NPC_Sword_h__

#include "Equipment.h"
#include "Include.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CShader;
	class CMaterial;
};

class CCityGuard2;
class CNPCs;
class CNPC_Sword
	: public CEquipment
{
private:
	explicit CNPC_Sword(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CNPC_Sword(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CNPC_Sword* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

public:
	const D3DXMATRIX*		m_pmatPlayerInfo;
	const D3DXMATRIX*		m_pmatWeaponRef;

	CNPCs*					m_pNpc;
	CCityGuard2*			m_pNPCCuard2;
	//CSoldier_1*				m_pSoldierNPC;

public:
	int				m_iIndex;
	TCHAR*			m_pObjName;
};
#endif // NPC_Sword_h__
