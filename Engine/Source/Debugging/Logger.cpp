#include "Logger.h"

#include <iostream>
#include <fstream>

//Size of a logged Message
const int LOG_MESSAGE_SIZE = 512;

class LoggingManager;
static LoggingManager* s_pLogManager = NULL;

class LoggingManager
{
public:
	LoggingManager(const char* logFilePath, bool bDebugConsoleEnabled);
	~LoggingManager();

	void Initialize();

	void Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum, va_list args);
	void Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum);

	void SetDisplayFlags(const std::string& tag, unsigned char flags);

private:
	void WriteToLog(const char* message);

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
		BE_ERROR("Failed to open Log File: %s", buf);
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

void LoggingManager::Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum, va_list args)
{
	char formattedMsg[LOG_MESSAGE_SIZE];
	vsprintf_s(formattedMsg, sizeof(formattedMsg), message, args);
	Log(tag, formattedMsg, func, file, lineNum);
}

void LoggingManager::Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum)
{
	char formattedMsg[LOG_MESSAGE_SIZE];

	if (func != NULL && file != NULL && lineNum != NULL) {
		sprintf_s(formattedMsg, sizeof(formattedMsg), "[%s] %s\nFunction: %s in: %s::%u", tag, message, func, file, lineNum);
	}
	else if (func != NULL && file != NULL) {
		sprintf_s(formattedMsg, sizeof(formattedMsg), "[%s] %s\nFunction: %s in: %s", tag, message, func, file);
	}
	else if (file != NULL) {
		sprintf_s(formattedMsg, sizeof(formattedMsg), "[%s] %s\nFile: %s", tag, message, file);
	}
	else {
		sprintf_s(formattedMsg, sizeof(formattedMsg), "[%s] %s", tag, message);
	}

	WriteToLog(message);
}

void LoggingManager::WriteToLog(const char* message)
{
	std::cout << message << "\n";

	if (m_logFile.is_open())
	{
		m_logFile << message << "\n";
		m_logFile.flush();
	}
}

void LoggingManager::SetDisplayFlags(const std::string& tag, unsigned char flags)
{
	BE_ERROR("Method Not Implemented: LoggingManager::SetDisplayFlags");
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

	void Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum, ...)
	{
		if (s_pLogManager)
		{
			va_list args;
			va_start(args, lineNum);
			s_pLogManager->Log(tag, message, func, file, lineNum, args);
		}
	}

	void SetDisplayFlags(const char* tag, unsigned char flags)
	{
		if (s_pLogManager)
		{
			s_pLogManager->SetDisplayFlags(tag, flags);
		}
	}
}