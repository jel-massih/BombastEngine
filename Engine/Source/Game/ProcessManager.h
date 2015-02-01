#pragma once

#include "Process.h"

class ProcessManager
{
	typedef std::list<Process*> ProcessList;

	ProcessList m_processList;

public:
	~ProcessManager();

	unsigned int UpdateProcesses(const float deltaMs);
	Process* AttachProcess(Process* pProcess);
	void AbortAllProcesses(bool immediate);

	size_t GetProcessCount() const { return m_processList.size(); }

private:
	void ClearAllProcesses();
};