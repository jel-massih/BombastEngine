#pragma once

#include "DebugText.h"

class DebugManager
{
public:
	DebugManager();

	bool Initialize();
	void Shutdown();

	bool Update();
	void Render();

	DebugText* GetDebugText() const { return m_pDebugText; }

private:
	DebugText* m_pDebugText;
};