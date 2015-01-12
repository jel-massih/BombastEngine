#pragma once

#include <Pdh.h>

class SystemResourceMonitor
{
public:
	SystemResourceMonitor();
	~SystemResourceMonitor();

	double GetFPS() const;

	void Frame();
	void Render();

	int GetCpuPercentage() const;

private:
	double m_fps;
	int m_frameCount;

	double m_startTime;

	bool m_bCpuReadable;
	HQUERY m_cpuQueryHandle;
	HCOUNTER m_cpuCounterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};