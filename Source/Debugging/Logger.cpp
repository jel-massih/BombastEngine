#include "Logger.h"

#include <iostream>

class LoggingManager;
static LoggingManager* s_pLogManager = NULL;

class LoggingManager
{
public:
	LoggingManager(bool bDebugConsoleEnabled);
	~LoggingManager();

	void Initialize();

	void Log(const std::string& tag, const std::string& message, const char* func, const char* file, unsigned int lineNum);

	void SetDisplayFlags(const std::string& tag, unsigned char flags);

private:
	bool m_bDebugConsoleEnabled;
};

LoggingManager::LoggingManager(bool bDebugConsoleEnabled)
{
	m_bDebugConsoleEnabled = bDebugConsoleEnabled;
}

LoggingManager::~LoggingManager()
{
	Log("LOGGER", "LoggingManager Shutting Down", NULL, NULL, 0);
	if (m_bDebugConsoleEnabled)
	{
		FreeConsole();
	}
}

void LoggingManager::Initialize()
{
	if (m_bDebugConsoleEnabled)
	{
		AllocConsole();

		FILE* pCout;
		freopen_s(&pCout, "CONIN$", "r", stdin);
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);
	}

	Log("LOGGER", "LoggingManager Initialized", NULL, NULL, 0);
}

void LoggingManager::Log(const std::string& tag, const std::string& message, const char* func, const char* file, unsigned int lineNum)
{
	std::string msg = "[" + tag + "] " + message + "\n";
	
	if (func != NULL)
	{
		msg += "\nFunction: ";
		msg += func;
	}

	if (file != NULL)
	{
		msg += "\n";
		msg += file;
	}

	if (lineNum != 0)
	{
		msg += "::";
		char lineBuffer[11];
		memset(&lineBuffer, 0, sizeof(char));
		msg += _itoa_s(lineNum, lineBuffer, 10);
	}

	msg += "\n";

	std::cout << msg;
}

void LoggingManager::SetDisplayFlags(const std::string& tag, unsigned char flags)
{

}

namespace BELogger
{
	void Init(bool bDebugConsoleEnabled)
	{
		if (!s_pLogManager)
		{
			s_pLogManager = BE_NEW LoggingManager(bDebugConsoleEnabled);
			s_pLogManager->Initialize();
		}
	}

	void Destroy()
	{
		SAFE_DELETE(s_pLogManager);
	}

	void Log(const std::string& tag, const std::string& message, const char* func, const char* file, unsigned int lineNum)
	{
		if (s_pLogManager)
		{
			s_pLogManager->Log(tag, message, func, file, lineNum);
		}
	}

	void SetDisplayFlags(const std::string& tag, unsigned char flags)
	{
		if (s_pLogManager)
		{
			s_pLogManager->SetDisplayFlags(tag, flags);
		}
	}
}