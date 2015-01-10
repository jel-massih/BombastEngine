#pragma once

class DebugText;

class HumanView;

class DebugManager
{
public:
	DebugManager();

	bool Initialize(HumanView* owner);
	void Shutdown();

	bool Update(const int deltaMs);
	void Render();

	DebugText* GetDebugText() const { return m_pDebugText; }

private:
	DebugText* m_pDebugText;
	SystemResourceMonitor* m_pSystemResourceMonitor;

	HumanView* m_pOwner;
};