#ifndef Collision_h__
#define Collision_h__

#include "Include.h"
#include "Component.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}
class CDynamicCamera;
class CCollision : public Engine::CComponent
{
private:
	explicit CCollision(void);
	virtual ~CCollision(void);
public:
	_bool	Collision_Distance(Engine::CTransform* pInfo1, Engine::CTransform* pInfo2, float fCollRadius);
	_bool	Collision_Distance(Engine::CTransform* pInfo1, D3DXVECTOR3 vPos, float fCollRadius);
	_bool	Collision_Distance(D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, float fCollRadius);
	void	Collision_Mesh_Player(Engine::CTransform* pInfo, Engine::CStaticMesh* pMesh);
	void	Collision_Mesh_Cam(CDynamicCamera* pCam, Engine::CTransform* pInfo, Engine::CStaticMesh* pMesh);

	bool	CollisionSphere(Engine::CTransform* Info1, Engine::CTransform* Info2);
public:
	static CCollision* Create(void);
public:
	virtual void Free(void);
};


#endif // Collision_h__
