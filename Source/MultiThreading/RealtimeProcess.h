#pragma once

#include "../Game/Process.h"

class RealtimeProcess : public Process
{
protected:
	HANDLE m_hThread;
	DWORD m_threadId;
	int m_threadPriority;

public:
	RealtimeProcess(int priority = THREAD_PRIORITY_NORMAL);
	virtual ~RealtimeProcess() { CloseHandle(m_hThread); }
	static DWORD WINAPI ThreadProc(LPVOID lpParam);

protected:
	virtual void VOnInit();
	virtual void VOnUpdate(unsigned long deltaMs) override {}
	virtual void VThreadProc() = 0;
};