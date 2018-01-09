/*!
 * \file Logo.h
 * \date 2015/04/03 0:04
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 로고
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Logo2_h__
#define Logo2_h__

#include "Scene.h"
#include "Include.h"

class CSecondThread;
class CLoading;
class CLogo2
	: public Engine::CScene
{
public:
	//각 씬에 필요한 레이어를 정의한다.
	//enum LAYERID {/*LAYER_ENVIRONMENT, */LAYER_GAMELOGIC};

private:
	explicit CLogo2(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo2(void);

public:
	virtual HRESULT InitScene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CLogo2* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_Light(void);

private:
	CLoading*		m_pLoading;
	CSecondThread*	m_pSecondThread;
public:
	virtual void Free(void);
};

#endif // Logo2_h__