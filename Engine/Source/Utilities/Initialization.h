#pragma once

#include "../BombastEngineStd.h"
#include "rapidxml.hpp"
#include <string>
#include "../Physics/Physics.h"

extern bool IsOnlyInstance(LPCTSTR gameTitle);
extern bool CheckStorage(const DWORDLONG diskSpaceNeeded);
extern bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);
extern DWORD ReadCPUSpeed();

struct GameOptions
{
	std::string m_level;

	std::string m_renderer;
	Point m_screenSize;
	Point m_screenPosition;
	bool m_bFullscreen;
	bool m_bVsync;

	EPhysicsEngine m_physicsEngine;
	bool m_bVisualizeDebugPhysics;

	rapidxml::xml_document<> *m_pDoc;

	bool m_bDebugConsoleEnabled;
	Point m_debugConsolePosition;
	std::string m_debugLogPath;
	std::string m_debugLogName;

	GameOptions();
	~GameOptions() { SAFE_DELETE(m_pDoc); }

	void Init(const char* xmlFilePath, LPWSTR lpCmdLine = NULL);

	void ProcessCommandLineArguments(LPWSTR lpCmdLine);
};