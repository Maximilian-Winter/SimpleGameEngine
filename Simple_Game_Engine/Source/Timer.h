// Timer 
// Class to get frame duration.
#ifndef _TIMER_H_
#define _TIMER_H_


// Include
#include <windows.h>
#include <string>
#include <fstream>


class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool Initialize();
	void Frame();

	float GetTime();

	void SaveLastFrameTimeToTextFile(std::string FileName);

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif