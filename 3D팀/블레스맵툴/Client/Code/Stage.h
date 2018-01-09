/*!
 * \file Stage.h
 * \date 2015/04/03 0:13
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 스테이지
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Include.h"

class CStage
	: public Engine::CScene
{
public:
	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI};

private:
	CStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage(void);
public:
	virtual HRESULT InitScene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Light(void);
public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free(void);
};

#endif // Stage_h__