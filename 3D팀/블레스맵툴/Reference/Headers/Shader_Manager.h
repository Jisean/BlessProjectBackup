#ifndef Shader_Manager_h__
#define Shader_Manager_h__

#include "Engine_Include.h"
#include "Base.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Manager : public CBase
{
	DECLARE_SINGLETON(CShader_Manager)
private:
	explicit CShader_Manager(void);
	virtual ~CShader_Manager(void);
public:
	CShader* Clone_Shader(const TCHAR* pShaderTag);
	LPD3DXEFFECT Get_EffectHandle(const TCHAR* pShaderTag);
public:
	HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFileName);
private:
	map<const TCHAR*, CShader*>				m_mapShader;
	typedef map<const TCHAR*, CShader*>		MAPSHADER;
private:
	CShader* Find_Shader(const TCHAR* pShaderTag);
public:
	virtual void Free(void);
};

END

#endif // Shader_Manager_h__
