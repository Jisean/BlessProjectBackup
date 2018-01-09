/*!
 * \file SceneSelector.h
 * \date 2015/03/03 11:48
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 새로운 씬을 생성하는 기능을 수행한다.
 *
 * \note
*/
#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Stage.h"
#include "Logo.h"
#include "Logo2.h"
#include "Stage2.h"

class CSceneSelector
{
public:
	enum SCENE {SCENE_LOGO, SCENE_STAGE, SCENE_LOGO2, SCENE_STAGE2, SCENE_END};
public:
	explicit CSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CSceneSelector(void) {}
public:
	HRESULT operator () (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice) 
	{
		switch(m_eSceneID)
		{
		case SCENE_LOGO:
			*ppScene = CLogo::Create(pDevice);
			break;

		case SCENE_STAGE:
			*ppScene = CStage::Create(pDevice);
			break;

		case SCENE_LOGO2:
			*ppScene = CLogo2::Create(pDevice);
			break;

		case SCENE_STAGE2:
			*ppScene = CStage2::Create(pDevice);
			break;
		}
	
		NULL_CHECK_RETURN(*ppScene, E_FAIL);

		return S_OK;
	}
private:
	SCENE			m_eSceneID;
};


#endif // SceneSelector_h__
