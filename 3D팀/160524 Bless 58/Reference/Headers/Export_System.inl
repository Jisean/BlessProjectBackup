//======================== For.GraphicDev =======================//
//--Getter--------------------------------------------------------//
LPDIRECT3DDEVICE9 Engine::Get_GraphicDev(void)
{
	return CGraphicDev::GetInstance()->GetDevice();	
}

//--Setter--------------------------------------------------------//

//--General-------------------------------------------------------//
HRESULT Engine::Ready_GraphicDev(Engine::CGraphicDev::WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	return CGraphicDev::GetInstance()->InitGraphicDev(Mode, hWnd, wSizeX, wSizeY);
}


//======================== For.Font_Manager =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
HRESULT Engine::Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey , const UINT& iHeight, const UINT& iWidth, const UINT& iWeight)
{
	return CFontMgr::GetInstance()->AddFont(pDevice, pFontKey, iHeight, iWidth, iWeight);
}

void Engine::Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render(pFontKey, vPos, pString, Color);
}


//======================== For.Input_Manager =======================//
//--Getter--------------------------------------------------------//
BYTE Engine::GetDIKeyState(BYTE KeyID)
{
	return CInput::GetInstance()->GetDIKeyState(KeyID);
}

BYTE Engine::GetDIMouseState(Engine::CInput::MOUSECLICK eKeyID)
{
	return CInput::GetInstance()->GetDIMouseState(eKeyID);
}

long Engine::GetDIMouseMove(Engine::CInput::MOUSEMOVE eKeyID)
{
	return CInput::GetInstance()->GetDIMouseMove(eKeyID);
}
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
HRESULT Engine::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInput::GetInstance()->InitInputDevice(hInst, hWnd);
}

void Engine::Update_InputDev(void)
{
	CInput::GetInstance()->SetInputState();		
}


//======================== For.TimeMgr =======================//
//--Getter--------------------------------------------------------//
float Engine::Get_TimeDelta(const TCHAR* pTimerTag)
{
	return Engine::CTimeMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
//--Setter--------------------------------------------------------//
void Engine::Set_TimeDelta(const TCHAR* pTimerTag)
{
	Engine::CTimeMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
//--General-------------------------------------------------------//
HRESULT Engine::Ready_Timer(const TCHAR* pTimerTag)
{
	return Engine::CTimeMgr::GetInstance()->Ready_Timer(pTimerTag);
}

// =========================== For.FrameMgr =========================== //
// For.Getter----------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Engine::Pass_Limit(const TCHAR* pFrameTag, const _float& fTimeDelta)
{
	return Engine::CFrameMgr::GetInstance()->Pass_Limit(pFrameTag, fTimeDelta);
}
// For.Setter----------------------------------------------------------------------------------------------------------------------------------------------------------------
// For.General----------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_Frame(const TCHAR* pFrameTag, const _float& fLimitTime)
{
	return Engine::CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fLimitTime);
}

//======================== For.Release_System =======================//
void Release_System(void)
{
	Engine::CFontMgr::GetInstance()->DestroyInstance();
	Engine::CInput::GetInstance()->DestroyInstance();
	Engine::CTimeMgr::GetInstance()->DestroyInstance();
	Engine::CFrameMgr::GetInstance()->DestroyInstance();
	Engine::CGraphicDev::GetInstance()->DestroyInstance();
}