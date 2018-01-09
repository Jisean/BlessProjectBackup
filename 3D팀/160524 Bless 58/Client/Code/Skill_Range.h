#ifndef Skill_Range_h__
#define Skill_Range_h__

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

class CSkill_Range_1;
class CSkill_Range
	: public Engine::CGameObject
{
 
private:
	CSkill_Range(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkill_Range(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	void	Start_SkillRange(CSkill_Range_1* pSkillRange, _vec3 vPos, float fRange, float fSpeed);
private:
	virtual HRESULT AddComponent(void);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CShader*		m_pShader;

	CSkill_Range_1*			m_pSkill_Range_1;
private:
	bool					m_bStart;
	float					m_fSpeed;

	float					m_fAlpha_Ctr;

public:
	static CSkill_Range* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void	Free(void);
};

#endif // Skill_Range_h__
