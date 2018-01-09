#include "stdafx.h"
#include "SoundMgr.h"
#include "Include.h"
#include <io.h>


IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr( void )
{
	m_pSystem = NULL;
	m_iVersion = 0;

	Initialize();
}

CSoundMgr::~CSoundMgr( void )
{
	
}

void CSoundMgr::Initialize()
{
	m_Result = FMOD_System_Create(&m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_GetVersion(m_pSystem, &m_iVersion);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_3D_RIGHTHANDED, NULL);
	ErrorCheck(m_Result);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t	fd;
	long handle;
	int iResult = 1;

	handle = _findfirst("..\\..\\SoundFile\\*.*", &fd);  //현재 폴더 내 모든 파일을 찾는다.       
	if (handle == -1)
	{
		return;
	}
	while (iResult != -1)     
	{ 
		TCHAR	szName[256];

		ZeroMemory(szName, sizeof(szName));
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fd.name, strlen(fd.name), szName, 256);

		TCHAR*	pName = new TCHAR[256];
		ZeroMemory(pName, sizeof(TCHAR) * 256);
		lstrcpy(pName, szName);

		char szFullName[256];
		strcpy_s(szFullName, "..\\..\\SoundFile\\" );
		strcat_s(szFullName, fd.name);

		FMOD_SOUND*		pSound;
		m_Result = FMOD_System_CreateSound(m_pSystem, szFullName, FMOD_HARDWARE, 0, &pSound);

		if(m_Result == FMOD_OK)
			m_mapSound.insert(make_pair(pName, pSound));
		else
			delete [] pName;

		iResult = _findnext(handle, &fd); 
	}
	_findclose(handle); 

	m_Result = FMOD_System_Update(m_pSystem);
	ErrorCheck(m_Result);
}

void CSoundMgr::PlayGuardianSound1( TCHAR* szSoundKey  , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh);
	SetVolume(m_pEffCh, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayGuardianSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh2);
	SetVolume(m_pEffCh2, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayGuardianSound3( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffHit1);
	SetVolume(m_pEffHit1, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayPaladinSound1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh3);
	SetVolume(m_pEffCh3, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayPaladinSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh4);
	SetVolume(m_pEffCh4, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayPaladinSound2_1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh4_Sub);
	SetVolume(m_pEffCh4_Sub, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayPaladinSound3( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffHit2);
	SetVolume(m_pEffHit2, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayMagicianSound1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh5);
	SetVolume(m_pEffCh5, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayMagicianSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh6);
	SetVolume(m_pEffCh6, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayMagicianSound3( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffHit3);
	SetVolume(m_pEffHit3, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayRangerSound1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh7);
	SetVolume(m_pEffCh7, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayRangerSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh8);
	SetVolume(m_pEffCh8, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayRangerSound3( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffHit4);
	SetVolume(m_pEffHit4, fVolume);

	ErrorCheck(m_Result);
}




void CSoundMgr::PlayBgm( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pBgmCh);
	SetVolume(m_pBgmCh, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlaySubBgm( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pSubBgmCh);
	SetVolume(m_pSubBgmCh, fVolume);

	ErrorCheck(m_Result);
}



void CSoundMgr::SkelSpearSound( TCHAR* szSoundKey , DWORD dwIndex, float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pMonster1[dwIndex]);
	SetVolume(m_pMonster1[dwIndex], fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::SkelWarriorSound( TCHAR* szSoundKey , DWORD dwIndex, float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pMonster2[dwIndex]);
	SetVolume(m_pMonster2[dwIndex], fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::SkelArcherSound( TCHAR* szSoundKey , DWORD dwIndex, float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pMonster3[dwIndex]);
	SetVolume(m_pMonster3[dwIndex], fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::MummySound( TCHAR* szSoundKey , DWORD dwIndex, float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pMonster4[dwIndex]);
	SetVolume(m_pMonster4[dwIndex], fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::SlimeSound( TCHAR* szSoundKey , DWORD dwIndex, float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pMonster5[dwIndex]);
	SetVolume(m_pMonster5[dwIndex], fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::SkelWolfSound( TCHAR* szSoundKey , DWORD dwIndex, float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pMonster6[dwIndex]);
	SetVolume(m_pMonster6[dwIndex], fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::SoulEaterSound1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pBoss1);
	SetVolume(m_pBoss1, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::SoulEaterSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pBoss2);
	SetVolume(m_pBoss2, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::GhostKnightSound1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pBoss3);
	SetVolume(m_pBoss3, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::GhostKnightSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pBoss4);
	SetVolume(m_pBoss4, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::GhostKnightSound3( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pBoss5);
	SetVolume(m_pBoss5, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::CityGuardSound( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pNPC1);
	SetVolume(m_pNPC1, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::WyvernSound1( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pNPC2);
	SetVolume(m_pNPC2, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::WyvernSound2( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pNPC3);
	SetVolume(m_pNPC3, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::WyvernSound3( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pNPC4);
	SetVolume(m_pNPC4, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::WyvernSound4( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pNPC5);
	SetVolume(m_pNPC5, fVolume);

	ErrorCheck(m_Result);
}

void CSoundMgr::PlayQuestSound( TCHAR* szSoundKey , float fVolume/* = 1.f*/)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter =  find_if(m_mapSound.begin(), m_mapSound.end(), CStringCompare(szSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pQuest1);
	SetVolume(m_pQuest1, fVolume);

	ErrorCheck(m_Result);
}


void CSoundMgr::UpdateSound()
{
	m_Result = FMOD_System_Update(m_pSystem);
	ErrorCheck(m_Result);
}

void CSoundMgr::StopBgm()
{
	m_Result = FMOD_Channel_Stop(m_pBgmCh);
	ErrorCheck(m_Result);
}

void CSoundMgr::StopSubBgm()
{
	m_Result = FMOD_Channel_Stop(m_pSubBgmCh);
	ErrorCheck(m_Result);
}

void CSoundMgr::StopPlaySound()
{
	m_Result = FMOD_Channel_Stop(m_pEffCh);
	ErrorCheck(m_Result);
}

void CSoundMgr::StopPlaySound2()
{
	m_Result = FMOD_Channel_Stop(m_pEffCh2);
	ErrorCheck(m_Result);
}


void CSoundMgr::StopSoundAll()
{
	m_Result = FMOD_Channel_Stop(m_pEffCh);
	ErrorCheck(m_Result);
	m_Result = FMOD_Channel_Stop(m_pEffCh2);
	ErrorCheck(m_Result);
	m_Result = FMOD_Channel_Stop(m_pBgmCh);
	ErrorCheck(m_Result);
}

void CSoundMgr::ErrorCheck( FMOD_RESULT _res )
{

}

void CSoundMgr::SetVolume(FMOD_CHANNEL* m_pChannel, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannel, fVolume);
}

void CSoundMgr::PlaySoundByNum( int iNum )
{
	if((size_t)iNum > m_mapSound.size())
		return;

	map<TCHAR*, FMOD_SOUND*>::iterator iter = m_mapSound.begin();
	for(int i = 0; i < iNum + 1; ++i)
		++iter;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, iter->second, 0 ,&m_pEffCh);
	ErrorCheck(m_Result);
}

void CSoundMgr::Free(void)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter = m_mapSound.begin();
	map<TCHAR*, FMOD_SOUND*>::iterator iter_end = m_mapSound.end();

	while(iter != iter_end)
	{
		m_Result = FMOD_Sound_Release(iter->second);
		ErrorCheck(m_Result);

		delete [](iter->first);
		iter = m_mapSound.erase(iter);
		if(iter == iter_end)
			break;
	}

	m_Result = FMOD_System_Close(m_pSystem);
	ErrorCheck(m_Result);
}