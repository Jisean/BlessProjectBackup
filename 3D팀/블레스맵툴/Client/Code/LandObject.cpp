#include "stdafx.h"
#include "LandObject.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
{

}

CLandObject::~CLandObject(void)
{

}

_int CLandObject::Update(const _float& fTimeDelta)
{
	return 0;
}
