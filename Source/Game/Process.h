#pragma once

#include "../Bombast/BombastApp.h"

class Process
{
	friend class ProcessManager;

public:
	enum State
	{
		UNINITIALIZED = 0,
		REMOVED,

		RUNNING,
		PAUSED,

		SUCCEEDED,
		FAILED,
		ABORTED
	};

public:
	Process();
	virtual ~Process();

	inline void Succeed();
	inline void Fail();

	inline void Pause();
	inline void UnPause();

	State GetState() const { return m_state; }
	bool IsAlive() const { return (m_state == RUNNING || m_state == PAUSED); }
	bool IsDead() const { return (m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED); }
	bool IsRemoved() const { return m_state == REMOVED; }
	bool IsPaused() const { return m_state == PAUSED; }

	inline void AttachChild(Process* pChild);
	Process* RemoveChild();
	Process* PeekChild() { return m_pChild; }

protected:
	virtual void VOnInit() { m_state = RUNNING; }
	virtual void VOnUpdate(unsigned long deltaMs) = 0;
	virtual void VOnSuccess() {}
	virtual void VOnFail() {}
	virtual void VOnAbort() {}

private:
	void SetState(State newState) { m_state = newState; }

private:
	State m_state;
	Process* m_pChild;
};

inline void Process::Succeed()
{
	BE_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_state = SUCCEEDED;
}

inline void Process::Fail()
{
	BE_ASSERT(m_state == RUNNING || m_state == PAUSED);
	m_state = FAILED;
}

inline void Process::AttachChild(Process* pChild)
{
	if (m_pChild)
	{
		m_pChild->AttachChild(pChild);
	}
	else
	{
		m_pChild = pChild;
	}
}

inline void Process::Pause()
{
	if (m_state == RUNNING)
	{
		m_state = PAUSED;
	}
	else
	{
		BE_ERROR("Attempting to pause a process that isnt running");
	}
}

inline void Process::UnPause()
{
	if (m_state == PAUSED)
	{
		m_state = RUNNING;
	}
	else
	{
		BE_ERROR("Attempting to unpause a process that isn't paused");
	}
}