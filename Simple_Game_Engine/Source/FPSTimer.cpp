#include "FPSTimer.h"


FPSTimer::FPSTimer()
{
}


FPSTimer::FPSTimer(const FPSTimer& other)
{
}


FPSTimer::~FPSTimer()
{
}


void FPSTimer::Initialize()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
	return;
}


void FPSTimer::Frame()
{
	m_count++;

	if(timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;
		
		m_startTime = timeGetTime();
	}
}


int FPSTimer::GetFps()
{
	return m_fps;
}