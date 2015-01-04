#pragma once

#include "../BombastEngineStd.h"
#include "rapidxml.hpp"
#include <string>

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

	rapidxml::xml_document<> *m_pDoc;

	GameOptions();
	~GameOptions() { SAFE_DELETE(m_pDoc); }

	void Init(const char* xmlFilePath, LPWSTR lpCmdLine);
};