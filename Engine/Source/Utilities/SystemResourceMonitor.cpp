#include "SystemResourceMonitor.h"

SystemResourceMonitor::SystemResourceMonitor()
	:m_fps(0), m_frameCount(0), m_startTime(0)
{
}

void SystemResourceMonitor::Frame()
{

}

void SystemResourceMonitor::Render()
{
	double time = g_pApp->GetTimer()->GetTime();
	m_frameCount++;

	if (time >= m_startTime + 1000.f)
	{
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_startTime = time;
	}

}

double SystemResourceMonitor::GetFPS() const
{
	return m_fps;
}