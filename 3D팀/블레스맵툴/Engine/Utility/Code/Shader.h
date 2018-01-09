#ifndef Shader_h__
#define Shader_h__

#include "Engine_Include.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& Instance);
	virtual ~CShader(void);
public:
	LPD3DXEFFECT Get_EffectHandle(void) {
		return m_pEffect;}
	CShader* Clone_Shader(void);
public:
	HRESULT Ready_Shader(const TCHAR* pFileName);
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXBUFFER				m_pErrBuffer;
	LPD3DXEFFECT				m_pEffect;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFileName);
	virtual void Free(void);
};

END

#endif // Shader_h__
