/*!
 * \file ParticleSystem.h
 * \date 2016/05/04 11:23
 *
 * \author 세웅
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Particle_System_h__
#define Particle_System_h__

// 생성자
// 소멸자
// public 변수
// public 함수
// private 변수
// private 함수
// Create()
// Free()


#include "GameObject.h"
#include "Shader.h"
#include "Engine_Include.h"

BEGIN(Engine)


class ENGINE_DLL CParticle_System : public CGameObject
{

protected:
	explicit CParticle_System(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CParticle_System(void);


protected:	
	ANGLE					m_eAngleDir;		//Circle의 방향
	PARTICLE_SHAPE			m_eParticleShape;	
	D3DXVECTOR3             m_vOrgin;  
	BOX						m_tBoundingBox;
	void*					m_pShape;	
	float                   m_fEmitRate;   
	float                   m_fSize;
	float					m_fSpeed;
	float					m_fLifeTime;
	float					m_fCreateTime;
	float					m_fAddTime;
	IDirect3DTexture9*      m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	list<ATTRIBUTE>			m_ParticleList; 
	DWORD                   m_dwMaxParticle; 		 
	D3DXVECTOR4				m_vColor;
	DWORD					m_dwParticleCount;
	

	CShader*				m_pShader;



	//Public 함수
public:
	virtual	HRESULT Ready_Particle(const PARTICLEINFO* pParticleInfo);
	virtual void Reset_Attribute();									
	virtual void Reset_Particle(ATTRIBUTE*	pAttribute);
	virtual void Add_Particle();
	void	SetShape(const PARTICLEINFO* pParticleInfo);
	
	
public:
	virtual _int Update(const _float& fTimeDelta);
	
	
	virtual void Render(void);									

	bool	Empty();				
	bool	Dead();					

	DWORD	FtoDw(float f);
	void	SetRandomInfo(D3DXVECTOR3* vPos, D3DXVECTOR3* vDir);	
	void	GetRandomVector( D3DXVECTOR3* out, D3DXVECTOR3* min, D3DXVECTOR3* max);
	float	GetRandomFloat(float lowBound, float highBound);
	float Compute_ViewZ(const D3DXVECTOR3* pPos);


protected:										
	virtual void DeleteDeadParticles();

	
public:
	static CParticle_System* Create(LPDIRECT3DDEVICE9 pGraphicDev, const PARTICLEINFO* pParticleInfo);
	virtual void Free(void);
	

	
};
END


#endif // Particle_System_h__