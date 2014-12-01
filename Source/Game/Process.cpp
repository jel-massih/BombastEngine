#include "Process.h"

Process::Process()
{
	m_state = UNINITIALIZED;
}

Process::~Process()
{
	if (m_pChild)
	{
		m_pChild->VOnAbort();
	}
}

Process* Process::RemoveChild()
{
	if (m_pChild)
	{
		Process* pChild = m_pChild;
		m_pChild = nullptr;
		return pChild;
	}

	return nullptr;
}