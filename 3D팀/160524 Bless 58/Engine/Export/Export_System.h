/*!
 * \file Export_System.h
 * \date 2015/04/02 19:19
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
#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "FrameMgr.h"
#include "TimeMgr.h"
#include "FontMgr.h"
#include "Input.h"


BEGIN(Engine)

//======================== For.GraphicDev =======================//
//--Getter--------------------------------------------------------//
inline LPDIRECT3DDEVICE9 Get_GraphicDev(void);

//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline HRESULT Ready_GraphicDev(Engine::CGraphicDev::WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);


//======================== For.Timer_Manager =======================//
//--Getter--------------------------------------------------------//
inline float Get_TimeDelta(const TCHAR* pTimerTag);
//--Setter--------------------------------------------------------//
inline void Set_TimeDelta(const TCHAR* pTimerTag);
//--General-------------------------------------------------------//
inline HRESULT Ready_Timer(const TCHAR* pTimerTag);


// =========================== For.FrameMgr =========================== //
// For.Getter----------------------------------------------------------------------------------------------------------------------------------------------------------------
inline bool Pass_Limit(const TCHAR* pFrameTag, const _float& fTimeDelta);
// For.Setter----------------------------------------------------------------------------------------------------------------------------------------------------------------
// For.General----------------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT Ready_Frame(const TCHAR* pFrameTag, const _float& fLimitTime);


//======================== For.Font_Manager =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey , const UINT& iHeight, const UINT& iWidth, const UINT& iWeight);
inline void Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color);

//======================== For.Input_Manager =======================//
//--Getter--------------------------------------------------------//
inline BYTE GetDIKeyState(BYTE KeyID);
inline BYTE GetDIMouseState(Engine::CInput::MOUSECLICK eKeyID);
inline long GetDIMouseMove(Engine::CInput::MOUSEMOVE eKeyID);
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//
inline HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
inline void Update_InputDev(void);


//======================== For.Release_System =======================//
inline void Release_System(void);


#include "Export_System.inl"

END

#endif // Export_System_h__