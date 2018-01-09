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
inline float Get_Time(void);
//--Setter--------------------------------------------------------//
inline void Set_Time(void);
//--General-------------------------------------------------------//
inline void Init_Time(void);


//======================== For.Frame_Manager =======================//
//--Getter--------------------------------------------------------//
//--Setter--------------------------------------------------------//
//--General-------------------------------------------------------//


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