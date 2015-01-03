#include "DebugManager.h"

DebugManager::DebugManager()
{
	m_pDebugText = 0;
}

bool DebugManager::Initialize()
{
	bool result;

	m_pDebugText = BE_NEW DebugText();
	if (!m_pDebugText)
	{
		return false;
	}

	result = m_pDebugText->Initialize();
	if (!result)
	{
		return false;
	}

	return true;
}

void DebugManager::Shutdown()
{
	if (m_pDebugText)
	{
		m_pDebugText->Shutdown();
		SAFE_DELETE(m_pDebugText);
	}
}

bool DebugManager::Update(const int deltaMs)
{
	return true;
}

void DebugManager::Render()
{
	if (m_pDebugText)
	{
		m_pDebugText->Render();
	}
}