#include "SystemResourceMonitor.h"

SystemResourceMonitor::SystemResourceMonitor()
	:m_fps(0), m_frameCount(0), m_startTime(0)
{
	PDH_STATUS status;
	m_bCpuReadable = true;

	status = PdhOpenQuery(NULL, 0, &m_cpuQueryHandle);
	if (status != ERROR_SUCCESS)
	{
		m_bCpuReadable = false;
	}

	status = PdhAddCounter(m_cpuQueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_cpuCounterHandle);
	if (status != ERROR_SUCCESS)
	{
		m_bCpuReadable = false;
	}

	m_lastSampleTime = GetTickCount();
	m_cpuUsage = 0;
}

SystemResourceMonitor::~SystemResourceMonitor()
{
	if (m_bCpuReadable)
	{
		PdhCloseQuery(m_cpuQueryHandle);
	}
}

void SystemResourceMonitor::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (m_bCpuReadable)
	{
		if (m_lastSampleTime + 1000 < GetTickCount())
		{
			m_lastSampleTime = GetTickCount();

			PdhCollectQueryData(m_cpuQueryHandle);
			PdhGetFormattedCounterValue(m_cpuCounterHandle, PDH_FMT_LONG, NULL, &value);
			m_cpuUsage = value.longValue;
		}
	}
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

int SystemResourceMonitor::GetCpuPercentage() const
{
	if (m_bCpuReadable)
	{
		return (int)m_cpuUsage;
	}
	
	return 0;
}