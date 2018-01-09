#include "stdafx.h"
#include "UI.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Stage.h"
#include "Texture.h"

CUI::CUI(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
, m_iCnt(0)
{
	m_htComponent.Ready_Table(119);
}

CUI::~CUI(void)
{

}

void CUI::Free(void)
{
	Engine::CGameObject::Free();
}
