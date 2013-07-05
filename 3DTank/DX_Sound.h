#pragma once

#include "dsound.h"

struct sWaveHeader 
{
	char  RiffID[4];         
	long  WaveformChunkSize; 
	char  WaveID[4];         
	char  FormatID[4];       
	long  FormatChunkSize;    
	short FormatTag;         
	short Channels;    
	long  SampleRate;
	long  BytesPerSec;
	short BlockAlign;
	short BitsPerSample;
	char  DataID[4];
	long  DataSize;
};

class DX_SoundManager
{
public:
	static DX_SoundManager* GetSoundManager();

	int  Init(HWND hWnd, DWORD dwCoopLevel = DSSCL_NORMAL);
	void Release(void);
	int  SetPrimaryBufferFormat(WORD wChannels=1, DWORD dwSamplesPerSec = 22050, WORD wBitsPerSample = 16);

	IDirectSound8* GetDirectSound();

private:
	DX_SoundManager();
	~DX_SoundManager();

	IDirectSound8* m_pDirectSound;
};

class DX_Sound
{
public:
	DX_Sound();
	~DX_Sound();

	int Play(bool bLoop);
	int Stop();

	int LoadWaveFile(const char* pcszWaveFile);
	int SetVolume(long lVolume = DSBVOLUME_MAX);
	
private:
	int GetSecondaryBuffer(FILE* pFile);
	int LoadDataInBuffer(FILE* pFile);

	IDirectSoundBuffer8* m_pDirectSoundBuf8;
};
