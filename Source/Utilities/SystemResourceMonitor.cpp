#include "SystemResourceMonitor.h"

SystemResourceMonitor::SystemResourceMonitor()
	:m_fps(0), m_frameCount(0), m_startTime(0)
{
}

void SystemResourceMonitor::Update()
{
	m_frameCount++;

	if (g_pApp->GetTimer()->GetTime() >= m_startTime + 1000.f)
	{
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_startTime = g_pApp->GetTimer()->GetTime();
	}
}

double SystemResourceMonitor::GetFPS() const
{
	return m_fps;
}