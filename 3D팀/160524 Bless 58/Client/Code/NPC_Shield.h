#ifndef NPC_Shield_h__
#define NPC_Shield_h__

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
class CNPC_Shield
	: public CEquipment
{
private:
	explicit CNPC_Shield(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CNPC_Shield(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	void		 Test();
private:
	virtual HRESULT AddComponent(void);

public:
	static CNPC_Shield* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);



public:
	const D3DXMATRIX*		m_pmatPlayerInfo;
	const D3DXMATRIX*		m_pmatWeaponRef;

	CCityGuard2*			m_pNPCCuard2;
	CNPCs*					m_pNpc;
	//======================
public:
	int				m_iIndex;
	TCHAR*			m_pObjName;
};
#endif // NPC_Shield_h__
