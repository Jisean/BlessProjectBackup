/*!
 * \file SkyBox.h
 * \date 2016/03/15 13:22
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
	class CShader;
};

class CSkyBox
	: public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CSkyBox(void);

public:
	virtual HRESULT Initialize(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	virtual HRESULT AddComponent(void);

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);

private:
	Engine::CTransform*			m_pInfo;
	Engine::CTexture*			m_pTexture;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CShader*			m_pShader;
};

#endif // SkyBox_h__