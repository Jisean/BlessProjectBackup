#ifndef Skill_Range_1_h__
#define Skill_Range_1_h__


#include "GameObject.h"
#include "Include.h"
#include "Equipment.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CShader;
};


class CSkill_Range_1
	: public Engine::CGameObject
{

private:
	CSkill_Range_1(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkill_Range_1(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	bool	Get_bEnd(void) { return m_bEnd;} ;

public:
	void	Set_bCancle(bool bType) { m_bCancle = bType; };

public:
	void	Start_SkillRange(_vec3 vPos, float fRange , float fMax, float fSpeed);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;


private:
	bool					m_bCancle;
	bool					m_bEnd;
	float					m_fMaxRange;
	float					m_fSpeed;


	float					m_fAlpha_Ctr;


public:
	static CSkill_Range_1* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void	Free(void);
};
#endif // Skill_Range_1_h__
