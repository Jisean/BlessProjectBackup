

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
BYTE GetDIKeyState(BYTE KeyID)
{
	return CInput::GetInstance()->GetDIKeyState(KeyID);
}

BYTE GetDIMouseState(Engine::CInput::MOUSECLICK eKeyID)
{
	return CInput::GetInstance()->GetDIMouseState(eKeyID);
}

long GetDIMouseMove(Engine::CInput::MOUSEMOVE eKeyID)
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
float Get_Time(void)
{
	return Engine::CTimeMgr::GetInstance()->GetTime();
}
//--Setter--------------------------------------------------------//
void Set_Time(void)
{
	Engine::CTimeMgr::GetInstance()->SetTime();
}
//--General-------------------------------------------------------//
void Init_Time(void)
{
	Engine::CTimeMgr::GetInstance()->InitTime();
}






//======================== For.Release_System =======================//
void Release_System(void)
{
	CFontMgr::GetInstance()->DestroyInstance();
	CInput::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
}