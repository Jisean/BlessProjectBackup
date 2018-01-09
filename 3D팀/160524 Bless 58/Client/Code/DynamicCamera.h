//////////////////////////////////////////////////////////////////////////수정본 문제있으면 원본 푸셈

/*!
 * \file DynamicCamera.h
 * \date 2016/02/24 13:32
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

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"
#include "Include.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}
class CMainPlayer;
class CDynamicCamera
	: public Engine::CCamera
{
public:
	enum CAMSTATE{ CAM_NORMAL, CAM_FREE, CAM_WYVERN_STATIC ,CAM_NORMAL_DYNAMIC , CAM_WYVERN_DYNAMIC,CAM_DEFAULT};
	enum ACTIONCAM{ ACT_CHARCHANGE, ACT_BOSSROOM };
private:
	CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CDynamicCamera(void);

public:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
	HRESULT RenewalCamera(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Free(void);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	void KeyCheck(const _float& fTimeDelta);
	void FixMouse(void);
	void MouseMove(void);
	void FixedMouseMove(void);
	void NormalCamera(void);
	void ActionCamera(const _float& fTimeDelta);
	void WyvernCamera_Static(void);
	void WyvernCamera_Dynamic(void);
private:
	bitset<2>				m_bActionCamSet;
	bitset<8>				m_bBossCamSeq;
	_float					m_fCamSpeed;
	_bool					m_bClick;
	_bool					m_bMouseFix;
	_bool					m_bChange;
	_float					m_fBossCamTime;
	_bool					m_bBossCamStart;
	_myvec					m_vBossCamAtSet[8];
	_myvec					m_vBossCamEyeSet[8];
	_float					m_fBossCamSpeed[8];
private:
	CAMSTATE				m_eCamState , m_ePrevState;
	_float					m_fDistance;
	D3DXVECTOR3				m_vPreEye;
	D3DXVECTOR3				m_vPreAt;
	CMainPlayer*			m_pMainPlayer;
private:
	_float					m_fTimeDelta;	// Get_Time
	_float					m_fSeconds;
private:
	_float					m_fSaveAngle;	// Y 자전
	_float					m_fAngle_Pitch;	// X 공전
	_float					m_fAngle_Yaw;	// Y 공전
	_bool					m_bInterpol;
	_bool					m_bFree;
private:
	Engine::CDynamicMesh*	m_pPlayerMesh;
	Engine::CTransform*		m_pPlayerInfo;
	Engine::CDynamicMesh*	m_pWyvernMesh;
	Engine::CTransform*		m_pWyvernInfo;
private:
	_bool					m_bLengthStart;
	_float					m_fLengthStart;
	/////////////////////Get////////////////////////
public:
	_bool	 Get_MouseFix(void){ return m_bMouseFix; }
	CAMSTATE Get_CamState();
	_float	 Get_SaveAngle();
	_float	 Get_fAngle_Yaw();
	_bool	 Get_Free();
	D3DXVECTOR3	GetAt(void){ return m_vAt; }
	D3DXVECTOR3 GetEye(void){ return m_vEye; }
	float		Get_Distance(){return m_fDistance; }
	bool		Get_BossCam(){ return !m_bActionCamSet.test(1); }
public:
	/////////////////////SET////////////////////////
	void	Set_MouseFix(bool bMouseFix);
	void	Set_CamState(CAMSTATE eCam);
	void	Set_CameraType(WORD	Number);
	void	Set_SaveAngle(_float fAngle);
	void	Set_fAngle_Yaw(_float fAngle_Yaw);
	void	Set_Interpol(_bool bCheck);
	void	Set_Free(_bool bCheck);
	void	Set_Eye(_vec3 vEye){m_vEye = vEye; }
	void	Set_Distance(_float fDist){ m_fDistance = fDist; }
	void	Set_ActionCam(WORD wActionCam){ m_bActionCamSet.set(wActionCam); }
};

#endif // DynamicCamera_h__


//////////////////////////////////////////////////////////////////////////