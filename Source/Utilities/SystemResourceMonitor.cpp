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
	m_frameCount++;

	double time = g_pApp->GetTimer()->GetTime()/1000.f;

	if (time >= m_startTime + 1000.f)
	{
		BE_INFO(ToStr(m_fps));
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_startTime = time;
	}

}

double SystemResourceMonitor::GetFPS() const
{
	return m_fps;
}