#pragma once

class DebugText;
class DebugPhysics;

class HumanView;

class DebugManager
{
public:
	DebugManager();
	~DebugManager();

	bool Initialize(HumanView* owner);

	bool Update(const float deltaMs);
	void Render();

	DebugText* GetDebugText() const { return m_pDebugText; }
	DebugPhysics* GetDebugPhysics() const { return m_pDebugPhysics; }

private:
	DebugText* m_pDebugText;
	DebugPhysics* m_pDebugPhysics;
	SystemResourceMonitor* m_pSystemResourceMonitor;

	HumanView* m_pOwner;
};