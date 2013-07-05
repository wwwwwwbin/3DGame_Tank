#include "stdafx.h"
#include "cstdio"
#include "DX_Sound.h"


DX_SoundManager::DX_SoundManager()
: m_pDirectSound(NULL)
{
}

DX_SoundManager::~DX_SoundManager()
{
	Release();
}

int DX_SoundManager::Init(HWND hWnd, DWORD dwCoopLevel)
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	hRetCode = DirectSoundCreate8(NULL, &m_pDirectSound, NULL);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	hRetCode = m_pDirectSound->SetCooperativeLevel(hWnd, dwCoopLevel); 
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	nResult = TRUE;
Exit0:
	if (!nResult)
	{
		Release();
	}
	return nResult;
}

void DX_SoundManager::Release( void )
{
	SafeRelease(m_pDirectSound);
}

int DX_SoundManager::SetPrimaryBufferFormat(WORD wChannels, DWORD dwSamplesPerSec, WORD wBitsPerSample)
{
	int		nResult  = FALSE;
	HRESULT hRetCode = FALSE;

	IDirectSoundBuffer* pDirectSoundBuf = NULL;

	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

    LOG_FAILD_JUMP(m_pDirectSound);

	ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
	bufferDesc.dwSize		 = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags		 = DSBCAPS_PRIMARYBUFFER; 
	bufferDesc.dwBufferBytes = 0;  
	bufferDesc.lpwfxFormat	 = NULL;

	hRetCode = m_pDirectSound->CreateSoundBuffer(&bufferDesc, &pDirectSoundBuf, NULL);
    LOG_HRESULT_FAILD_JUMP(hRetCode);

    ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX)); 
    waveFormat.wFormatTag		= WAVE_FORMAT_PCM; 
    waveFormat.nChannels		= wChannels; 
    waveFormat.nSamplesPerSec	= dwSamplesPerSec; 
    waveFormat.wBitsPerSample	= wBitsPerSample; 
    waveFormat.nBlockAlign		= (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	hRetCode = pDirectSoundBuf->SetFormat(&waveFormat);
	if (hRetCode != DS_OK)
	{
		switch(hRetCode)
		{
		case DSERR_BADFORMAT:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "DSERR_BADFORMAT");
			break;
		case DSERR_INVALIDCALL:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "DSERR_INVALIDCALL");
			break;
		case DSERR_INVALIDPARAM:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "DSERR_INVALIDPARAM");
			break;
		case DSERR_OUTOFMEMORY:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "DSERR_OUTOFMEMORY");
			break;
		case DSERR_PRIOLEVELNEEDED:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "DSERR_PRIOLEVELNEEDED");
			break;
		case DSERR_UNSUPPORTED:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "DSERR_UNSUPPORTED");
			break;
		default:
			CLogSystem::LogPrint(LOG_TYPE_DEBUG, "hRetCode not match");
		}
	}
	CLogSystem::LogPrint(LOG_TYPE_DEBUG, "%d, %d", hRetCode, DS_OK);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	nResult = TRUE;
Exit0:
	SafeRelease(pDirectSoundBuf);
	return nResult;
}

DX_SoundManager* DX_SoundManager::GetSoundManager()
{
	static DX_SoundManager* pInstance = NULL;
	if (!pInstance)
	{
		pInstance = new DX_SoundManager();
	}
	return pInstance;
}

IDirectSound8* DX_SoundManager::GetDirectSound()
{
	return m_pDirectSound;
}


DX_Sound::DX_Sound()
: m_pDirectSoundBuf8(NULL)
{

}

DX_Sound::~DX_Sound()
{
	SafeRelease(m_pDirectSoundBuf8);
}

int DX_Sound::LoadWaveFile(const char* cpszWaveFile)
{
	int		nResult  = FALSE;
	int		nRetCode = FALSE;
	FILE*	pFile	 = NULL;

	LOG_FAILD_JUMP(cpszWaveFile);

	nRetCode = fopen_s(&pFile, cpszWaveFile, "rb");
	LOG_FAILD_JUMP(!nRetCode);
	LOG_FAILD_JUMP(pFile);
	
	nRetCode = GetSecondaryBuffer(pFile);
	LOG_FAILD_JUMP(nRetCode);

	nRetCode = LoadDataInBuffer(pFile);
	LOG_FAILD_JUMP(pFile);
	
	nResult = TRUE;
Exit0:
	fclose(pFile);
	pFile = NULL;

	return nResult;
}

int DX_Sound::GetSecondaryBuffer(FILE* pFile)
{
	int		nResult  = FALSE;
	HRESULT	hRetCode = E_FAIL;
	
	sWaveHeader*		pWaveHeader		= NULL;
	IDirectSoundBuffer* pDirectSoundBuf	= NULL;

	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;

	LOG_FAILD_JUMP(pFile);

	pWaveHeader = new sWaveHeader();
	LOG_FAILD_JUMP(pWaveHeader);

	fseek(pFile, 0, SEEK_SET);
	fread(pWaveHeader, 1, sizeof(sWaveHeader), pFile);

	LOG_FAILD_JUMP(!memcmp(pWaveHeader->RiffID,   "RIFF", 4));
	LOG_FAILD_JUMP(!memcmp(pWaveHeader->WaveID,   "WAVE", 4));
	LOG_FAILD_JUMP(!memcmp(pWaveHeader->FormatID, "fmt ", 4));
	LOG_FAILD_JUMP(!memcmp(pWaveHeader->DataID,   "data", 4));
	
	
	ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));
	waveFormat.wFormatTag		= WAVE_FORMAT_PCM;
	waveFormat.nChannels		= pWaveHeader->Channels;  
	waveFormat.nSamplesPerSec	= pWaveHeader->SampleRate;
	waveFormat.wBitsPerSample	= pWaveHeader->BitsPerSample;
	waveFormat.nBlockAlign		= (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	ZeroMemory(&bufferDesc,sizeof(DSBUFFERDESC));
	bufferDesc.dwSize		 = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags		 = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS|DSBCAPS_STATIC;
	bufferDesc.dwBufferBytes = pWaveHeader->DataSize;
	bufferDesc.lpwfxFormat	 = &waveFormat;

	hRetCode = DX_SoundManager::GetSoundManager()->GetDirectSound()->CreateSoundBuffer(&bufferDesc, &pDirectSoundBuf, NULL);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	hRetCode = pDirectSoundBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pDirectSoundBuf8);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	nResult = TRUE;
Exit0:
	pDirectSoundBuf->Release();
	delete pWaveHeader;

	return nResult;
}

int DX_Sound::LoadDataInBuffer(FILE* pFile)
{
	int		nResult			= FALSE;
	HRESULT	hRetCode		= E_FAIL;
	DWORD	dwSoundSize1	= 0;
	DWORD	dwSoundSize2	= 0;

	BYTE*			pSoundPtr1	= NULL;
	BYTE*			pSoundPtr2	= NULL;
	sWaveHeader*	pWaveHeader = NULL;
	
	LOG_FAILD_JUMP(pFile);
	
	pWaveHeader = new sWaveHeader();
	LOG_FAILD_JUMP(pWaveHeader);

	fseek(pFile, 0, SEEK_SET);
	fread(pWaveHeader, 1, sizeof(sWaveHeader),pFile);
	
	hRetCode = m_pDirectSoundBuf8->Lock(
			0, 
			pWaveHeader->DataSize,
			(void**)&pSoundPtr1, 
			&dwSoundSize1,
			(void**)&pSoundPtr2, 
			&dwSoundSize2,
			0
	);
	LOG_HRESULT_FAILD_JUMP(hRetCode);
	
	fread(pSoundPtr1, 1, dwSoundSize1, pFile);
	if (pSoundPtr2 != NULL)
	{
		fread(pSoundPtr2, 1, dwSoundSize2, pFile);
	}

	m_pDirectSoundBuf8->Unlock(pSoundPtr1, dwSoundSize1, pSoundPtr2, dwSoundSize2);
	

	nResult = TRUE;
Exit0:
	delete pWaveHeader;

	return nResult;
}

int DX_Sound::Play(bool bLoop)
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	LOG_FAILD_JUMP(m_pDirectSoundBuf8);

	hRetCode = m_pDirectSoundBuf8->SetCurrentPosition(0);
	LOG_HRESULT_FAILD_JUMP(hRetCode);
	
	hRetCode = m_pDirectSoundBuf8->Play(0, 0, bLoop);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	nResult = TRUE;
Exit0:
	return nResult;
}

int DX_Sound::SetVolume(long lVolume)
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	LOG_FAILD_JUMP(m_pDirectSoundBuf8);

	hRetCode = m_pDirectSoundBuf8->SetVolume(lVolume);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	nResult = TRUE;
Exit0:
	return nResult;
}

int DX_Sound::Stop()
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	LOG_FAILD_JUMP(m_pDirectSoundBuf8);

	hRetCode = m_pDirectSoundBuf8->Stop();
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	nResult = TRUE;
Exit0:
	return nResult;
}