#include "Logger.h"

#include <iostream>
#include <fstream>

class LoggingManager;
static LoggingManager* s_pLogManager = NULL;

class LoggingManager
{
public:
	LoggingManager(const char* logFilePath, bool bDebugConsoleEnabled);
	~LoggingManager();

	void Initialize();

	void Log(const std::string& tag, const std::string& message, const char* func, const char* file, unsigned int lineNum);

	void SetDisplayFlags(const std::string& tag, unsigned char flags);

private:
	bool m_bDebugConsoleEnabled;
	std::ofstream m_logFile;
};

LoggingManager::LoggingManager(const char* logFilePath, bool bDebugConsoleEnabled)
{
	m_logFile.open(logFilePath, std::ios::out);
	if (m_logFile.fail())
	{
		char buf[1000];
		strerror_s(buf, 1000, errno);
		BE_ERROR("Failed to open Log File: " + std::string(buf));
	}

	m_bDebugConsoleEnabled = bDebugConsoleEnabled;
}

LoggingManager::~LoggingManager()
{
	Log("LOGGER", "LoggingManager Shutting Down", NULL, NULL, 0);

	m_logFile.close();

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
		msg += " in: ";
	}

	if (file != NULL)
	{
		msg += "\n";
		msg += file;
	}

	if (lineNum != 0)
	{
		msg += "::";
		msg += ToStr(lineNum);
	}

	msg += "\n";

	std::cout << msg;

	if (m_logFile.is_open())
	{
		m_logFile << msg;
		m_logFile.flush();
	}
}

void LoggingManager::SetDisplayFlags(const std::string& tag, unsigned char flags)
{

}

namespace BELogger
{
	void Init(bool bDebugConsoleEnabled, const char* logPath, const char* logName)
	{
		if (!s_pLogManager)
		{
			time_t rawTime;
			struct tm timeinfo;
			char buffer[80];
			time(&rawTime);
			localtime_s(&timeinfo, &rawTime);
			strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S_", &timeinfo);

			std::string logFileName = logPath;
			logFileName += "/";
			logFileName += buffer;
			logFileName += logName;

			s_pLogManager = BE_NEW LoggingManager(logFileName.c_str(), bDebugConsoleEnabled);
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