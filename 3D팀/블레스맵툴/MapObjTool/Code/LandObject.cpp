#include "stdafx.h"
#include "LandObject.h"
#include "Transform.h"
#include "ObjectTool.h"
#include "MainFrm.h"
#include "MyForm.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
{
	ZeroMemory(m_tcKey, sizeof(TCHAR) * 256);
}

CLandObject::~CLandObject(void)
{

}

int CLandObject::Update(const float& fTimeDelta)
{
	CObjectTool* pObjTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_Tab2;

	/*if(pObjTool->m_WireFrame.GetCheck() == TRUE)
	{

	}*/

	return 0;
}
void CLandObject::SetPos(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CLandObject::SetScale(D3DXVECTOR3 vScale)
{
	m_pInfo->m_vScale = vScale;
}

void CLandObject::SetAngle(D3DXVECTOR3 vAngle)
{
	m_pInfo->m_fAngle[Engine::ANGLE_X] = vAngle.x;
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = vAngle.x;
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = vAngle.x;
}
