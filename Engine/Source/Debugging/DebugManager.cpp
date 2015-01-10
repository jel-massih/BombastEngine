#include "DebugManager.h"
#include "DebugText.h"
#include "../Bombast/BombastApp.h"

#include "../Game/HumanView.h"
#include "../Utilities/SystemResourceMonitor.h"

DebugManager::DebugManager()
{
	m_pSystemResourceMonitor = 0;
	m_pDebugText = 0;
	m_pOwner = 0;
}

bool DebugManager::Initialize(HumanView* owner)
{
	bool result;
	
	m_pOwner = owner;

	m_pSystemResourceMonitor = BE_NEW SystemResourceMonitor;
	if (!m_pSystemResourceMonitor)
	{
		return false;
	}

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

	m_pDebugText->AddString("CameraPos");
	m_pDebugText->AddString("CameraRot");

	m_pDebugText->AddString("FPS");

	return true;
}

void DebugManager::Shutdown()
{
	if (m_pDebugText)
	{
		m_pDebugText->Shutdown();
		SAFE_DELETE(m_pDebugText);
	}

	SAFE_DELETE(m_pSystemResourceMonitor);
}

bool DebugManager::Update(const int deltaMs)
{
	Vec3 viewPos = m_pOwner->m_pCamera->GetPosition();
	Vec3 viewRot = m_pOwner->m_pCamera->GetDirection();
	
	m_pSystemResourceMonitor->Frame();

	m_pDebugText->UpdateString("CameraPos", (std::string("Pos: ") + ToStr(viewPos)).c_str());

	char buffer[100];
	_snprintf_s(buffer, 100, "Rot: (X: %d, Y: %d, Z: %d)", (int)XMConvertToDegrees(viewRot.x), (int)XMConvertToDegrees(viewRot.y), (int)XMConvertToDegrees(viewRot.z));
	m_pDebugText->UpdateString("CameraRot", buffer);
	m_pDebugText->UpdateString("FPS", ("FPS: " + ToStr((int)m_pSystemResourceMonitor->GetFPS())).c_str());

	return true;
}

void DebugManager::Render()
{
	if (m_pSystemResourceMonitor)
	{
		m_pSystemResourceMonitor->Render();
	}

	if (m_pDebugText)
	{
		m_pDebugText->Render();
	}
}