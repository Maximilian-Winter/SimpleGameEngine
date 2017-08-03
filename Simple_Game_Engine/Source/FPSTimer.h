// FPSTimer
// Class to give info about the actual fps count.
#ifndef _FPSTIMER_H_
#define _FPSTIMER_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: FPSTimer
////////////////////////////////////////////////////////////////////////////////
class FPSTimer
{
public:
	FPSTimer();
	FPSTimer(const FPSTimer&);
	~FPSTimer();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif