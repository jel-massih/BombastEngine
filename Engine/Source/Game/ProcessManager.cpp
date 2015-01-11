#include "ProcessManager.h"

ProcessManager::~ProcessManager()
{
	ClearAllProcesses();
}

unsigned int ProcessManager::UpdateProcesses(const float deltaMs)
{
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		Process* pCurrProcess = (*it);

		ProcessList::iterator thisIt = it;
		it++;

		if (pCurrProcess->GetState() == Process::UNINITIALIZED)
		{
			pCurrProcess->VOnInit();
		}

		if (pCurrProcess->GetState() == Process::RUNNING)
		{
			pCurrProcess->VOnUpdate(deltaMs);
		}

		if (pCurrProcess->IsDead())
		{
			switch (pCurrProcess->GetState())
			{
				case Process::SUCCEEDED:
				{
					pCurrProcess->VOnSuccess();
					Process* pChild = pCurrProcess->RemoveChild();
					if (pChild)
					{
						AttachProcess(pChild);
					}
					else
					{
						successCount++;
					}

					break;
				}

				case Process::FAILED:
				{
					pCurrProcess->VOnFail();
					failCount++;
					break;
				}

				case Process::ABORTED:
				{
					pCurrProcess->VOnAbort();
					failCount++;
					break;
				}
			}

			m_processList.erase(thisIt);
		}
	}

	return ((successCount << 16) | failCount);
}

Process* ProcessManager::AttachProcess(Process* pProcess)
{
	m_processList.push_front(pProcess);
	return pProcess;
}

void ProcessManager::ClearAllProcesses()
{
	m_processList.clear();
}

void ProcessManager::AbortAllProcesses(bool immediate)
{
	ProcessList::iterator it = m_processList.begin();

	while (it != m_processList.end())
	{
		ProcessList::iterator tempIt = it;
		it++;

		Process* pProcess = *tempIt;
		if (pProcess->IsAlive())
		{
			pProcess->SetState(Process::ABORTED);
			if (immediate)
			{
				pProcess->VOnAbort();
				m_processList.erase(tempIt);
			}
		}
	}
}