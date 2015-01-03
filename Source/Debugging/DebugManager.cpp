#include "DebugManager.h"
#include "../Bombast/BombastApp.h"

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

	m_pDebugText->AddString("CameraPos");

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
	Mat4x4 viewMat;
	g_pApp->GetGraphicsManager()->GetRenderer()->VGetViewMatrix(viewMat);
	Vec3 viewPos = viewMat.GetPosition();
	char buffer[100];
	_snprintf_s(buffer, 100, "Pos: (X: %d, Y: %d, Z: %d)", (int)viewPos.x, (int)viewPos.y, viewPos.z);
	m_pDebugText->UpdateString("CameraPos", buffer);

	return true;
}

void DebugManager::Render()
{
	if (m_pDebugText)
	{
		m_pDebugText->Render();
	}
}