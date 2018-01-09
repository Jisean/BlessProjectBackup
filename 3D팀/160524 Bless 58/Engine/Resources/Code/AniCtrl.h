/*!
 * \file AniCtrl.h
 * \date 2016/03/10 13:34
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

#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
public:
	CAniCtrl(void);
	CAniCtrl(const CAniCtrl& rhs);
	~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER* GetAniCtrl(void);
	DOUBLE	GetTrackPosition(void);
	DOUBLE	GetPeriod(void);
	//----------------Ãß°¡ºÐ----------------------//
	void	SetAniInfo(ANI_INFO AniInfo) { m_tAniInfo = AniInfo;}

public:
	void SetAnimationSet(UINT iAniIdx);
	void SetTrackPos(DOUBLE dPosition);
	void FrameMove(const float& fTimePerSec);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;
	ANI_INFO						m_tAniInfo;

private:
	UINT		m_iMaxAniSet;
	UINT		m_iCurrentTrack;
	UINT		m_iNewTrack;
	DOUBLE		m_dAccTime;
	DOUBLE		m_dPeriod;
	UINT		m_iIndexState;
private:
	void Free(void);
};

END

#endif // AniCtrl_h__