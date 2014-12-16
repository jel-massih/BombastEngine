#include "../Game/ProcessManager.h"
#include "RealtimeProcess.h"
#include "CriticalSection.h"

DWORD g_maxLoops = 100000;
DWORD g_protectedTotal = 0;
CRITICAL_SECTION g_criticalSection;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD maxLoops = *static_cast<DWORD*>(lpParam);
	DWORD dwCount = 0;
	while (dwCount < maxLoops)
	{
		dwCount++;

		EnterCriticalSection(&g_criticalSection);
		g_protectedTotal++;
		LeaveCriticalSection(&g_criticalSection);
	}

	return TRUE;
}

void CreateThreads()
{
	InitializeCriticalSection(&g_criticalSection);

	for (int i = 0; i < 20; i++)
	{
		HANDLE m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, &g_maxLoops, 0, NULL);
	}
}

RealtimeProcess::RealtimeProcess(int priority)
{
	m_threadId = 0;
	m_threadPriority = priority;
}

DWORD WINAPI RealtimeProcess::ThreadProc(LPVOID lpParam)
{
	RealtimeProcess* proc = static_cast<RealtimeProcess*>(lpParam);
	proc->VThreadProc();
	return TRUE;
}

void RealtimeProcess::VOnInit()
{
	Process::VOnInit();
	m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, &m_threadId);

	if (m_hThread == NULL)
	{
		BE_ERROR("Could not Create Thread!");
		Fail();
		return;
	}

	SetThreadPriority(m_hThread, m_threadPriority);
}

class ProtectedProcess : public RealtimeProcess
{
public:
	static DWORD g_protectedTotal;
	static CriticalSection g_criticalSection;

	DWORD m_maxLoops;
	ProtectedProcess(DWORD maxLoops) : RealtimeProcess() { m_maxLoops = maxLoops; }

	virtual void VThreadProc();
};

DWORD ProtectedProcess::g_protectedTotal = 0;
CriticalSection ProtectedProcess::g_criticalSection;

void ProtectedProcess::VThreadProc()
{
	DWORD dwCount = 0;

	while (dwCount < m_maxLoops)
	{
		dwCount++;

		{
			ScopedCriticalSection locker(g_criticalSection);
			g_protectedTotal++;
		}
	}

	Succeed();
}

class UnprotectedProcess : public RealtimeProcess
{
public:
	static DWORD g_unprotectedTotal;
	DWORD m_maxLoops;
	UnprotectedProcess(DWORD maxLoops) : RealtimeProcess() { m_maxLoops = maxLoops; }
	virtual void VThreadProc();
};

DWORD UnprotectedProcess::g_unprotectedTotal = 0;

void UnprotectedProcess::VThreadProc()
{
	DWORD dwCount = 0;
	while (dwCount < m_maxLoops)
	{
		dwCount++;
		g_unprotectedTotal++;
	}

	Succeed();
}

int g_threadCount = 50;
DWORD g_threadLoops = 100000;

void testThreading(ProcessManager* processManager, bool bRunProtected)
{
	Process* proc = BE_NEW UnprotectedProcess(g_threadLoops);
	processManager->AttachProcess(proc);

	return;
}