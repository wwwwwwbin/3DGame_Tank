#include "StdAfx.h"
#include "Timer.h"

Timer::Timer(void)
{
	InitTime();
}
Timer::~Timer(void)
{
}

void Timer::InitTime()
{
	m_iFrameCount = 0;
	m_fFPS = 0;
	m_fTime1 = 0;
	m_fTime2 = 0;
	m_fTimeSlice = 0;
	if(QueryPerformanceFrequency((LARGE_INTEGER*) &m_int64OneSecondTicks)){
		m_bUseLargeTime = true;
		QueryPerformanceCounter((LARGE_INTEGER*) &m_int64TimeTickStartCounts);
	}
	else{
		MessageBox(NULL,"无法使用高精度时间计数器","警告",MB_OK|MB_ICONINFORMATION);
		m_bUseLargeTime = false;
		m_ulTimeStart = timeGetTime();
	}
}

float Timer::GetGamePlayTime()
{
	__int64 int64TimeCurrentCounts;
	if(m_bUseLargeTime){
		QueryPerformanceCounter((LARGE_INTEGER*) &int64TimeCurrentCounts);
		return ((float)(int64TimeCurrentCounts - m_int64TimeTickStartCounts)*(1.0f/m_int64OneSecondTicks)*1000.0f);
	}
	else{
		return ((float)(timeGetTime() - m_ulTimeStart));
	}
}

void Timer::UpdateFPS()
{
	++m_iFrameCount;
	if(m_iFrameCount%5 == 1){
		m_fTime1 = GetGamePlayTime()/1000;
	}
	else if(m_iFrameCount%5 == 0){
		m_fTime2 = GetGamePlayTime()/1000;
		m_fTimeSlice = (float)fabs(m_fTime1 - m_fTime2);
	}
	m_fFPS = 5/m_fTimeSlice;
}