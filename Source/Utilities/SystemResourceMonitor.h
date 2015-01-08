#pragma once

class SystemResourceMonitor
{
public:
	SystemResourceMonitor();
	~SystemResourceMonitor();

	double GetFPS() const;

	void Update();

private:
	double m_fps;
	int m_frameCount;

	unsigned long m_startTime;
};