#pragma once

#include <Windows.h>

class CriticalSection : public BE_NonCopyable
{
public:
	CriticalSection()
	{
		InitializeCriticalSection(&m_cs);
	}

	~CriticalSection()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

protected:
	mutable CRITICAL_SECTION m_cs;
};

class ScopedCriticalSection : public BE_NonCopyable
{
public:
	ScopedCriticalSection(CriticalSection& cResource)
		: m_csResource(cResource)
	{
		m_csResource.Lock();
	}

	~ScopedCriticalSection()
	{
		m_csResource.Unlock();
	}

private:
	CriticalSection& m_csResource;
};