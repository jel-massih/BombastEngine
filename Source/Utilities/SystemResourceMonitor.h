#pragma once

class SystemResourceMonitor
{
public:
	SystemResourceMonitor();

	double GetFPS() const;

	void Frame();
	void Render();

private:
	double m_fps;
	int m_frameCount;

	double m_startTime;
};