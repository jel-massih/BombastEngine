#pragma once

class Timer
{
public:
	bool Initialize();
	void Frame();

	float GetFrameTime() const { return m_frameTime; }
	double GetTime() const { return (double)m_startTime; };

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};