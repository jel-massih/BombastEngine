#pragma once

class Timer
{
public:
	bool Initialize();
	void Frame();

	double GetFrameTime() const { return m_frameTime; }
	double GetTime() const { return m_currentTime; };

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_counter;
	INT64 m_counterStart;

	double m_ticksPerMs;
	double m_currentTime;
	double m_frameTime;
};