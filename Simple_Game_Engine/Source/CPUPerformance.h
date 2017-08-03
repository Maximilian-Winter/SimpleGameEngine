// CPU performance monitoring class
#ifndef _CPUPERFORMANCE_H_
#define _CPUPERFORMANCE_H_


// Linking
#pragma comment(lib, "pdh.lib")


// Includes
#include <pdh.h>


class CPUPerformance
{
public:
	CPUPerformance();
	CPUPerformance(const CPUPerformance&);
	~CPUPerformance();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

#endif