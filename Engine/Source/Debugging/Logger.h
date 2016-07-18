#pragma once

namespace BELogger
{
	void Init(bool bDebugConsoleEnabled, const char* logPath, const char* logName);
	void Destroy();

	void Assert(bool expr, const char* message, ...);
	
	void Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum, ...);
	void SetDisplayFlags(const char* tag, unsigned char flags);
}

#define BE_ERROR(str, ...) \
    do { \
        BELogger::Log("ERROR", str, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
		char formattedMsg[256]; \
		wchar_t wideMsg[256]; \
		sprintf_s(formattedMsg, sizeof(formattedMsg), str, ##__VA_ARGS__); \
		MultiByteToWideChar(CP_ACP, 0, formattedMsg, -1, wideMsg, 256); \
        if(g_pApp->GetHwnd() != NULL) { \
            MessageBox(g_pApp->GetHwnd(), wideMsg, L"Error", MB_OK); \
        } else { \
            OutputDebugString(wideMsg); \
		} \
	} while(0) \


#define BE_ASSERT(expr) \
	do { \
		if (!(expr)) \
		{ \
			assert(expr); \
		} \
	} while (0) \

#define BE_ASSERTf(expr, message, ...) \
	do { \
		if (!(expr)) \
		{ \
			BE_LOG("ASSERT FAIL", message, ##__VA_ARGS__); \
			BELogger::Assert(expr, message, ##__VA_ARGS__); \
		} \
	} while (0) \

#define BE_WARNING(str, ...) BELogger::Log("WARNING", str, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)

#define BE_INFO(str, ...) BE_LOG("INFO", str, ##__VA_ARGS__)

#define BE_LOG_GRAPHICS(str, ...) BE_LOG("GRAPHICS", str, ##__VA_ARGS__)
#define BE_LOG_PHYSICS(str, ...) BE_LOG("PHYSICS", str, ##__VA_ARGS__)
#define BE_LOG_SCRIPT(str, ...) BE_LOG("SCRIPT", str, ##__VA_ARGS__)

#define BE_LOG(tag, str, ...) BELogger::Log(tag, str, NULL, NULL, 0, ##__VA_ARGS__)

#define BE_HRETURN(x, str) { hr = (x); if( FAILED(hr) ) { BE_ERROR( str ); return hr; } }