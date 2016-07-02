#include "../BombastEngineStd.h"
#include "../Bombast/BombastApp.h"
#include "Initialization.h"
#include <fstream>
#include <sstream>
#include <shellapi.h>

using namespace rapidxml;

//Checks if there is window already existing.
bool IsOnlyInstance(LPCTSTR gameTitle)
{
	//Find Window, if active, set and return false

	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);

	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd)
		{
			BE_ERROR("Instance Exists");
			//An instance of game is already running
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}

	return true;
}

//Check to see if there is megaBytesNeeded of Storage available
bool CheckStorage(const DWORDLONG megaBytesNeeded)
{
	//Check for enough free storage on current disk
	int const drive = _getdrive();
	struct _diskfree_t diskFree;

	_getdiskfree(drive, &diskFree);

	//Availablespace equals megabytes Free
	unsigned __int64 const availableSpace = (((diskFree.sectors_per_cluster * diskFree.bytes_per_sector) >> 10) * diskFree.avail_clusters) >> 10;

	if (availableSpace < megaBytesNeeded)
	{
		BE_ERROR("CheckStorage Failure: Not enough physical storage");
		return false;
	}

	return true;
}

/**Check to see if there is enough physical and virtual ram to run the game
* @param physicalRAMNeeded - Physical RAM needed in MB
* @param physicalRAMNeeded -  Virtual RAM needed in MB
*/
bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX status;

	status.dwLength = sizeof(status);

	GlobalMemoryStatusEx(&status);

	unsigned __int64 physMemoryMB = status.ullTotalPhys >> 20;
	unsigned __int64 virtMemoryMB = status.ullAvailVirtual >> 20;

	if (physMemoryMB < physicalRAMNeeded)
	{
		BE_ERROR("CheckMemory Failure: Not Enough Physical RAM");
		return false;
	}

	if (virtMemoryMB < virtualRAMNeeded)
	{
		BE_ERROR("CheckMemory Failure: Not Enough Virtual Memory");
		return false;
	}

	return true;
}

//Get CPU Speed
DWORD ReadCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;

	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

	if (lError == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}

	return dwMHz;
}

GameOptions::GameOptions()
{
	m_level = "EngineResources.World.default.xml";
	m_renderer = "Direct3D 11";
	m_screenSize = Point(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_screenPosition = Point(SCREEN_X, SCREEN_Y);

	m_physicsEngine = EPhysicsEngine::BE_PHYSICS_BOMBAST;

	m_bDebugConsoleEnabled = false;
	m_debugConsolePosition = Point(0, 0);	

	m_debugLogPath = DEBUG_LOG_PATH;
	m_debugLogName = DEBUG_LOG_FILENAME;

	m_pDoc = NULL;

	m_additionalContentDirectories = "";
}

void GameOptions::Init(const char* xmlFilePath, LPWSTR lpCmdLine)
{
	std::ifstream file(xmlFilePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	xml_document<> doc;
	
	doc.parse<0>(&content[0]);

	//Get Root node
	xml_node<> *pRoot = doc.first_node("Options");

	if (!pRoot)
	{
		return;
	}

	//Fetch each child and load component
	xml_node<> *pNode = NULL;
	pNode = pRoot->first_node("Graphics");
	if (pNode)
	{
		std::string attribute;
		attribute = pNode->first_attribute("renderer")->value();
		if (attribute != "Direct3D 9" && attribute != "Direct3D 11")
		{
			BE_ERROR("Bad Renderer setting in Graphics options.");
		}
		else
		{
			m_renderer = attribute;
		}
			
		if (pNode->first_attribute("width"))
		{
			attribute = pNode->first_attribute("width")->value();
			m_screenSize.x = atoi(attribute.c_str());
		}

		if (pNode->first_attribute("height"))
		{
			attribute = pNode->first_attribute("height")->value();
			m_screenSize.y = atoi(attribute.c_str());
		}

		if (pNode->first_attribute("x"))
		{
			attribute = pNode->first_attribute("x")->value();
			m_screenPosition.x = atoi(attribute.c_str());
		}

		if (pNode->first_attribute("y"))
		{
			attribute = pNode->first_attribute("y")->value();
			m_screenPosition.y = atoi(attribute.c_str());
		}

		if (pNode->first_attribute("fullscreen"))
		{
			std::string val = pNode->first_attribute("fullscreen")->value();
			std::transform(val.begin(), val.end(), val.begin(), ::tolower); //Convert to lower for equality check
			m_bFullscreen = val != "false";
		}
		else {
			m_bFullscreen = false;
		}

		if (pNode->first_attribute("vsync"))
		{
			std::string val = pNode->first_attribute("vsync")->value();
			std::transform(val.begin(), val.end(), val.begin(), ::tolower); //Convert to lower for equality check
			m_bVsync = val != "false";
		}
		else {
			m_bVsync = true;
		}
	}

	pNode = pRoot->first_node("Physics");
	if (pNode)
	{
		std::string attribute;
		if (pNode->first_attribute("engine")) 
		{
			attribute = pNode->first_attribute("engine")->value();
			if (StringToPhysicsEngineType.count(attribute)) 
			{
				m_physicsEngine = StringToPhysicsEngineType[attribute];
			}
			else 
			{
				BE_ERROR("Bad Physics Engine setting in Physics options.");
			}
		}

		if (pNode->first_attribute("visualize"))
		{
			std::string val = pNode->first_attribute("visualize")->value();
			std::transform(val.begin(), val.end(), val.begin(), ::tolower); //Convert to lower for equality check
			m_bVisualizeDebugPhysics = val != "false";
		}
		else {
			m_bVisualizeDebugPhysics = false;
		}
	}

	pNode = pRoot->first_node("World");
	if (pNode)
	{
		std::string attribute;
		attribute = pNode->first_attribute("level")->value();
		m_level = attribute;
	}

	pNode = pRoot->first_node("Debugging");
	if (pNode)
	{
		xml_node<> *pDebugNode = pNode->first_node("DebugConsole");
		if (pDebugNode)
		{
			if (pDebugNode->first_attribute("enabled"))
			{
				std::string val = pDebugNode->first_attribute("enabled")->value();
				std::transform(val.begin(), val.end(), val.begin(), ::tolower); //Convert to lower for equality check
				m_bDebugConsoleEnabled = val != "false";
			}
			else {
				m_bDebugConsoleEnabled = true;
			}

			std::string attribute;
			if (pDebugNode->first_attribute("x"))
			{
				attribute = pDebugNode->first_attribute("x")->value();
				m_screenPosition.x = atoi(attribute.c_str());
			}

			if (pDebugNode->first_attribute("y"))
			{
				attribute = pDebugNode->first_attribute("y")->value();
				m_screenPosition.y = atoi(attribute.c_str());
			}
		}

		pDebugNode = pNode->first_node("Logging");
		if (pDebugNode)
		{
			if (pDebugNode->first_attribute("path"))
			{
				m_debugLogPath = pDebugNode->first_attribute("path")->value();
			}
			
			if (pDebugNode->first_attribute("filename"))
			{
				m_debugLogName = pDebugNode->first_attribute("filename")->value();
			}
		}
	}

	pNode = pRoot->first_node("Resources");
	if (pNode)
	{
		xml_node<>* pAdditionalResourcePaths = pNode->first_node("AdditionalContentPaths");
		if (pAdditionalResourcePaths)
		{
			xml_node<>* pPathNode = pAdditionalResourcePaths->first_node("ContentDirectory");
			if (pPathNode) {
				do {
					m_additionalContentDirectories += pPathNode->value();
					m_additionalContentDirectories += ",";
					pPathNode = pPathNode->next_sibling("ContentDirectory");
				} while (pPathNode);
			}
		}
	}

	if (lpCmdLine != NULL)
	{
		ProcessCommandLineArguments(lpCmdLine);
	}
}

std::string GetCmdOption(const std::vector<std::string>& args, const std::string& option)
{
	auto itr = std::find(args.begin(), args.end(), option);
	if (itr != args.end() && ++itr != args.end())
	{
		return *itr;
	}
	return "";
}

bool CmdOptionExists(const std::vector<std::string>& args, const std::string& option)
{
	return std::find(args.begin(), args.end(), option) != args.end();
}

void GameOptions::ProcessCommandLineArguments(LPWSTR lpCmdLine)
{
	std::istringstream iss(ws2s(lpCmdLine));
	std::vector<std::string> vArgs{ std::istream_iterator<std::string>{iss},
							 std::istream_iterator<std::string>{} };

	std::string resX = GetCmdOption(vArgs, "-ResX");
	if (resX != "")
	{
		m_screenSize.x = atoi(resX.c_str());
	}

	std::string resY = GetCmdOption(vArgs, "-ResY");
	if (resY != "")
	{
		m_screenSize.y = atoi(resY.c_str());
	}
}