//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Timer.cpp
//  Version     :   1.0
//  Creater     :   weibin Huang
//  Date        :   2013-07-04 21:06
//  Comment     :   Container of Texture
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Timer.h"

CTimer::CTimer(void)
: m_nFrameCount(0)
, m_ulTimeStart(0)
, m_fFPS(0.f)
, m_fTime1(0.f)
, m_fTime2(0.f)
, m_fTimeSlice(0.f)
, m_lld64OneSecondTicks(0)
, m_lldTimeTickStartCounts(0)
, m_bUseLargeTime(false)

{
}

CTimer::~CTimer(void)
{
}

int CTimer::InitTime()
{
	int nResult  = FALSE;
	int nRetCode = FALSE;
	
	m_fFPS			= 0;
	m_fTime1		= 0;
	m_fTime2		= 0;
	m_fTimeSlice	= 0;
	m_nFrameCount	= 0;

	nRetCode = QueryPerformanceFrequency((LARGE_INTEGER*) &m_lld64OneSecondTicks);
	if (nRetCode)
	{
		m_bUseLargeTime = true;
		QueryPerformanceCounter((LARGE_INTEGER*) &m_lldTimeTickStartCounts);
	}
	else
	{
		MessageBox(NULL, "无法使用高精度时间计数器", "警告", MB_OK | MB_ICONINFORMATION);
		m_bUseLargeTime = false;
		m_ulTimeStart = timeGetTime();
	}

	nResult = TRUE;
Exit0:
	return nResult;
}

float CTimer::GetGamePlayTime()
{
	float	fResult					= 0.f;
	__int64 lldTimeCurrentCounts	= 0;

	if (m_bUseLargeTime)
	{
		QueryPerformanceCounter((LARGE_INTEGER*) &lldTimeCurrentCounts);
		fResult = (float)(lldTimeCurrentCounts - m_lldTimeTickStartCounts) * (1.0f / m_lld64OneSecondTicks) * 1000.0f;
	}
	else
	{
		fResult = (float)(timeGetTime() - m_ulTimeStart);
	}

	return fResult;
}

void CTimer::UpdateFPS()
{
	++m_nFrameCount;
	if (m_nFrameCount % 5 == 1)
	{
		m_fTime1 = GetGamePlayTime() / 1000;
	}
	else if (m_nFrameCount % 5 == 0)
	{
		m_fTime2 = GetGamePlayTime() / 1000;
		m_fTimeSlice = (float)fabs(m_fTime1 - m_fTime2);
	}

	m_fFPS = 5 / m_fTimeSlice;
}

float CTimer::GetFPS()
{
	return m_fFPS;
}

CTimer* CTimer::GetInstance( void )
{
	static CTimer* pInstance = NULL;

	if (!pInstance)
	{
		pInstance = new CTimer();
	}

	return pInstance;
}
