#ifndef SoundMgr_h__
#define SoundMgr_h__
/*********************************************************************************************
@ FileName : SoundMgr.h
@ FilePath : /System/Codes/
@ Date : 15.03.26
@ Author : SongSong
@ Brife : 사운드 재생을 담당하는 싱글톤 클래스
*********************************************************************************************/
#include "Engine_Include.h"
#include "Base.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"


class CSoundMgr : public Engine::CBase
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	FMOD_SYSTEM*	m_pSystem;		//FMOD System Pointer

	FMOD_CHANNEL*	m_pEffCh;		//이펙트 채널(가디언1)
	FMOD_CHANNEL*	m_pEffCh2;		//이펙트 채널(가디언2)
	FMOD_CHANNEL*	m_pEffHit1;		//이펙트 히트 채널(가디언)

	FMOD_CHANNEL*	m_pEffCh3;		//이펙트 채널(팔라딘1)
	FMOD_CHANNEL*	m_pEffCh4;		//이펙트 채널(팔리딘2)
	FMOD_CHANNEL*	m_pEffHit2;		//이펙트 히트 채널(팔라딘)
	FMOD_CHANNEL*	m_pEffCh4_Sub;	//이펙트 채널 장판용(팔라딘2)	

	FMOD_CHANNEL*	m_pEffCh5;		//이펙트 채널(매지션1)
	FMOD_CHANNEL*	m_pEffCh6;		//이펙트 채널(매지션2)
	FMOD_CHANNEL*	m_pEffHit3;		//이펙트 히트 채널(매지션)

	FMOD_CHANNEL*	m_pEffCh7;		//이펙트 채널(레인져1)
	FMOD_CHANNEL*	m_pEffCh8;		//이펙트 채널(레인져2)
	FMOD_CHANNEL*	m_pEffHit4;		//이펙트 히트 채널(레인져)

	FMOD_CHANNEL*	m_pBgmCh;		//메인BGM 채널
	FMOD_CHANNEL*	m_pSubBgmCh;		//서브BGM 채널(환경음)

	FMOD_CHANNEL*	m_pMonster1[20];	//몬스터 사운드 채널1(스켈 스피어)
	FMOD_CHANNEL*	m_pMonster2[55];	//몬스터 사운드 채널2(스켈 워리어)
	FMOD_CHANNEL*	m_pMonster3[40];	//몬스터 사운드 채널3(스켈 아처)
	FMOD_CHANNEL*	m_pMonster4[15];	//몬스터 사운드 채널4(미이라)
	FMOD_CHANNEL*	m_pMonster5[20];	//몬스터 사운드 채널5(슬라임)
	FMOD_CHANNEL*	m_pMonster6[20];	//몬스터 사운드 채널6(스켈 울프)

	FMOD_CHANNEL*	m_pBoss1;		//보스 사운드 채널(소울이터1)
	FMOD_CHANNEL*	m_pBoss2;		//보스 사운드 채널(소울이터2)

	FMOD_CHANNEL*	m_pBoss3;		//보스 사운드 채널(고스트나이트1)
	FMOD_CHANNEL*	m_pBoss4;		//보스 사운드 채널(고스트나이트2)
	FMOD_CHANNEL*	m_pBoss5;		//보스 사운드 채널(고스트나이트3)

	FMOD_CHANNEL*	m_pNPC1;		//Npc 사운드 채널(시티가드)
	FMOD_CHANNEL*	m_pNPC2;		//Npc 사운드 채널(와이번1)
	FMOD_CHANNEL*	m_pNPC3;		//Npc 사운드 채널(와이번2)
	FMOD_CHANNEL*	m_pNPC4;		//Npc 사운드 채널(와이번3)
	FMOD_CHANNEL*	m_pNPC5;		//Npc 사운드 채널(와이번4)

	FMOD_CHANNEL*	m_pQuest1;		//퀘스트 채널

	FMOD_RESULT		m_Result;		//FMOD Running result;

	unsigned int	m_iVersion;
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;		//사운드들을 가지고 있는 map
public:
	void	Initialize();
	void	LoadSoundFile();

	void	PlayGuardianSound1(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayGuardianSound2(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayGuardianSound3(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayPaladinSound1(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayPaladinSound2(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayPaladinSound2_1(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayPaladinSound3(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayMagicianSound1(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayMagicianSound2(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayMagicianSound3(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayRangerSound1(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayRangerSound2(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlayRangerSound3(TCHAR*	szSoundKey, float fVolume = 1.f);

	void	PlaySoundByNum(int iNum);
	void	PlayBgm(TCHAR*	szSoundKey, float fVolume = 1.f);
	void	PlaySubBgm(TCHAR*	szSoundKey, float fVolume = 1.f);

	void	SkelSpearSound( TCHAR*	szSoundKey , DWORD dwIndex, float fVolume = 1.f);
	void	SkelWarriorSound( TCHAR*	szSoundKey , DWORD dwIndex, float fVolume = 1.f);
	void	SkelArcherSound( TCHAR*	szSoundKey , DWORD dwIndex, float fVolume = 1.f);
	void	MummySound( TCHAR*	szSoundKey , DWORD dwIndex, float fVolume = 1.f);
	void	SlimeSound( TCHAR*	szSoundKey , DWORD dwIndex, float fVolume = 1.f);
	void	SkelWolfSound( TCHAR*	szSoundKey , DWORD dwIndex, float fVolume = 1.f);

	void	SoulEaterSound1( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	SoulEaterSound2( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	GhostKnightSound1( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	GhostKnightSound2( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	GhostKnightSound3( TCHAR*	szSoundKey , float fVolume = 1.f);

	void	CityGuardSound( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	WyvernSound1( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	WyvernSound2( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	WyvernSound3( TCHAR*	szSoundKey , float fVolume = 1.f);
	void	WyvernSound4( TCHAR*	szSoundKey , float fVolume = 1.f);

	void	PlayQuestSound( TCHAR*	szSoundKey , float fVolume = 1.f);

	void	UpdateSound();
	void	StopBgm();
	void	StopSubBgm();
	void	StopPlaySound();
	void	StopPlaySound2();
	void	StopSoundAll();
	map<TCHAR*, FMOD_SOUND*>*	GetSoundMap(){return &m_mapSound;}
private:
	void	ErrorCheck(FMOD_RESULT _res);
	void	SetVolume(FMOD_CHANNEL* m_pChannel, float fVolume);
	virtual void	Free(void);
private:
	explicit CSoundMgr(void);
public:
	virtual ~CSoundMgr(void);
};


#endif // SoundMgr_h__