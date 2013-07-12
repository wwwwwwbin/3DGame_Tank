//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Timer.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-04 21:06
//  Comment     :   Container of Texture
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_INCLUDE
#define TIMER_INCLUDE

class CTimer
{
public:
	static CTimer* GetInstance(void);

	int  InitTime();
	void  UpdateFPS();
	float GetGamePlayTime();
	float GetFPS();

private:
	CTimer(void);
	~CTimer(void);

	int				m_nFrameCount;
	bool			m_bUseLargeTime;
	float			m_fFPS;
	float			m_fTime1,m_fTime2,m_fTimeSlice;
	__int64 		m_lld64OneSecondTicks;
	__int64 		m_lldTimeTickStartCounts;
	unsigned long	m_ulTimeStart;
	
};
#endif
