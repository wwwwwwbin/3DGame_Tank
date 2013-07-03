#ifndef TIMER_INCLUDE
#define TIMER_INCLUDE

class Timer
{
	bool m_bUseLargeTime;
	__int64 m_int64OneSecondTicks;
	__int64 m_int64TimeTickStartCounts;
	unsigned long m_ulTimeStart;
	int m_iFrameCount;
	float m_fFPS;
	float m_fTime1,m_fTime2,m_fTimeSlice;
public:
	Timer(void);
	~Timer(void);
	void InitTime();
	float GetGamePlayTime();
	void UpdateFPS();
	inline float GetFPS(){return m_fFPS;}
};
#endif
