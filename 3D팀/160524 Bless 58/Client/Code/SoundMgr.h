#ifndef SoundMgr_h__
#define SoundMgr_h__
/*********************************************************************************************
@ FileName : SoundMgr.h
@ FilePath : /System/Codes/
@ Date : 15.03.26
@ Author : SongSong
@ Brife : ���� ����� ����ϴ� �̱��� Ŭ����
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

	FMOD_CHANNEL*	m_pEffCh;		//����Ʈ ä��(�����1)
	FMOD_CHANNEL*	m_pEffCh2;		//����Ʈ ä��(�����2)
	FMOD_CHANNEL*	m_pEffHit1;		//����Ʈ ��Ʈ ä��(�����)

	FMOD_CHANNEL*	m_pEffCh3;		//����Ʈ ä��(�ȶ��1)
	FMOD_CHANNEL*	m_pEffCh4;		//����Ʈ ä��(�ȸ���2)
	FMOD_CHANNEL*	m_pEffHit2;		//����Ʈ ��Ʈ ä��(�ȶ��)
	FMOD_CHANNEL*	m_pEffCh4_Sub;	//����Ʈ ä�� ���ǿ�(�ȶ��2)	

	FMOD_CHANNEL*	m_pEffCh5;		//����Ʈ ä��(������1)
	FMOD_CHANNEL*	m_pEffCh6;		//����Ʈ ä��(������2)
	FMOD_CHANNEL*	m_pEffHit3;		//����Ʈ ��Ʈ ä��(������)

	FMOD_CHANNEL*	m_pEffCh7;		//����Ʈ ä��(������1)
	FMOD_CHANNEL*	m_pEffCh8;		//����Ʈ ä��(������2)
	FMOD_CHANNEL*	m_pEffHit4;		//����Ʈ ��Ʈ ä��(������)

	FMOD_CHANNEL*	m_pBgmCh;		//����BGM ä��
	FMOD_CHANNEL*	m_pSubBgmCh;		//����BGM ä��(ȯ����)

	FMOD_CHANNEL*	m_pMonster1[20];	//���� ���� ä��1(���� ���Ǿ�)
	FMOD_CHANNEL*	m_pMonster2[55];	//���� ���� ä��2(���� ������)
	FMOD_CHANNEL*	m_pMonster3[40];	//���� ���� ä��3(���� ��ó)
	FMOD_CHANNEL*	m_pMonster4[15];	//���� ���� ä��4(���̶�)
	FMOD_CHANNEL*	m_pMonster5[20];	//���� ���� ä��5(������)
	FMOD_CHANNEL*	m_pMonster6[20];	//���� ���� ä��6(���� ����)

	FMOD_CHANNEL*	m_pBoss1;		//���� ���� ä��(�ҿ�����1)
	FMOD_CHANNEL*	m_pBoss2;		//���� ���� ä��(�ҿ�����2)

	FMOD_CHANNEL*	m_pBoss3;		//���� ���� ä��(��Ʈ����Ʈ1)
	FMOD_CHANNEL*	m_pBoss4;		//���� ���� ä��(��Ʈ����Ʈ2)
	FMOD_CHANNEL*	m_pBoss5;		//���� ���� ä��(��Ʈ����Ʈ3)

	FMOD_CHANNEL*	m_pNPC1;		//Npc ���� ä��(��Ƽ����)
	FMOD_CHANNEL*	m_pNPC2;		//Npc ���� ä��(���̹�1)
	FMOD_CHANNEL*	m_pNPC3;		//Npc ���� ä��(���̹�2)
	FMOD_CHANNEL*	m_pNPC4;		//Npc ���� ä��(���̹�3)
	FMOD_CHANNEL*	m_pNPC5;		//Npc ���� ä��(���̹�4)

	FMOD_CHANNEL*	m_pQuest1;		//����Ʈ ä��

	FMOD_RESULT		m_Result;		//FMOD Running result;

	unsigned int	m_iVersion;
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;		//������� ������ �ִ� map
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