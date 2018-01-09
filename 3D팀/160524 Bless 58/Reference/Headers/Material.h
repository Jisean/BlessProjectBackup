#ifndef Material_h__
#define Material_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial : public Engine::CComponent
{
private:
	explicit CMaterial(void);
	virtual ~CMaterial(void);
public:
	const D3DMATERIAL9* Get_Material(void);
public:
	HRESULT Ready_Material(const D3DMATERIAL9* pMaterial);
	HRESULT Ready_Material(const D3DXCOLOR& Diffuse, const D3DXCOLOR& Ambient
		, const D3DXCOLOR& Specular, const D3DXCOLOR& Emissive, const _float& fPower);	
public:
	static CMaterial* Create(const D3DMATERIAL9* pMaterial);
	static CMaterial* Create(const D3DXCOLOR& Diffuse, const D3DXCOLOR& Ambient
		, const D3DXCOLOR& Specular, const D3DXCOLOR& Emissive, const _float& fPower);
private:
	D3DMATERIAL9			m_MtrlInfo;
public:
	virtual void Free(void);
};

END

#endif // Material_h__
