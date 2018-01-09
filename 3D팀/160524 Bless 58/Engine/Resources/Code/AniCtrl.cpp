#include "AniCtrl.h"

Engine::CAniCtrl::CAniCtrl(void)
: m_pAniCtrl(NULL) 
, m_iMaxAniSet(0)
, m_iCurrentTrack(0)
, m_iNewTrack(1)
, m_dAccTime(0)
, m_dPeriod(0)
, m_iIndexState(0)
{
	ZeroMemory(&m_tAniInfo, sizeof(ANI_INFO));
	m_tAniInfo.fSpeed = 1.f;
	m_tAniInfo.fDelay = 1.f;
	m_tAniInfo.fWeight = 0.1f;
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
: m_pAniCtrl(NULL) 
, m_iMaxAniSet(0)
, m_iCurrentTrack(0)
, m_iNewTrack(1)
, m_dAccTime(0)
, m_dPeriod(0)
, m_iIndexState(0)
{
	rhs.m_pAniCtrl->CloneAnimationController(
		rhs.m_pAniCtrl->GetMaxNumAnimationOutputs()
		, rhs.m_pAniCtrl->GetMaxNumAnimationSets()
		, rhs.m_pAniCtrl->GetMaxNumTracks()
		, rhs.m_pAniCtrl->GetMaxNumEvents()
		, &m_pAniCtrl);

	ZeroMemory(&m_tAniInfo, sizeof(ANI_INFO));
	m_tAniInfo.fSpeed = 1.f;
	m_tAniInfo.fDelay = 1.f;
	m_tAniInfo.fWeight = 0.1f;

	m_iMaxAniSet = rhs.m_pAniCtrl->GetMaxNumAnimationSets();
}

Engine::CAniCtrl::~CAniCtrl(void)
{
	
}

LPD3DXANIMATIONCONTROLLER* Engine::CAniCtrl::GetAniCtrl(void)
{
	return &m_pAniCtrl;
}

DOUBLE Engine::CAniCtrl::GetTrackPosition(void)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);
	
	return TrackInfo.Position;
}

DOUBLE Engine::CAniCtrl::GetPeriod(void)
{
	return m_dPeriod;
}

void Engine::CAniCtrl::SetAnimationSet(UINT iAniIdx)
{
	//if(iAniIdx >= m_iMaxAniSet)
	//	return;

	//if(m_iCurrentTrack == 0)
	//	m_iNewTrack = 1;
	//else
	//	m_iNewTrack = 0;

	//LPD3DXANIMATIONSET		pAS = NULL;
	//m_pAniCtrl->GetAnimationSet(iAniIdx, &pAS);
	////m_pAniCtrl->GetAnimationSetByName()
	//m_dPeriod = pAS->GetPeriod();

	//m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);
	//pAS->Release();

	//m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	//m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	////현재 재생되고 있는 트랙을 종료
	//m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + 0.25);
	//m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	//m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	////새로운 트랙을 킨다.
	//m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	//m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	//m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	//m_dAccTime = 0.0;
	//m_iCurrentTrack = m_iNewTrack;
	//m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	//m_pAniCtrl->ResetTime();
	if(iAniIdx >= m_iMaxAniSet)
		return;

	if(m_iIndexState == iAniIdx)
		return;

	m_iIndexState = iAniIdx;

	if(m_iCurrentTrack == 0)
		m_iNewTrack = 1;
	else
		m_iNewTrack = 0;

	LPD3DXANIMATIONSET		pAS = NULL;
	m_pAniCtrl->GetAnimationSet(iAniIdx, &pAS);
	//m_pAniCtrl->GetAnimationSetByName()
	m_dPeriod = pAS->GetPeriod();

	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);
	pAS->Release();

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	//현재 재생되고 있는 트랙을 종료
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + 0.25);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, m_tAniInfo.fSpeed, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, m_tAniInfo.fWeight, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	//새로운 트랙을 킨다.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, m_tAniInfo.fSpeed, m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, (1.f - m_tAniInfo.fWeight), m_dAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_dAccTime = 0.0;
	m_iCurrentTrack = m_iNewTrack;
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_pAniCtrl->ResetTime();
}

void Engine::CAniCtrl::SetTrackPos(DOUBLE dPosition)
{
	m_pAniCtrl->SetTrackPosition(m_iCurrentTrack, dPosition);
}

void Engine::CAniCtrl::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->AdvanceTime(fTimePerSec, NULL);
	m_dAccTime += DOUBLE(fTimePerSec);
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}