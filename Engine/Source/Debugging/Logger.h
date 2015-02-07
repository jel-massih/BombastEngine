#pragma once

#include "../Utilities/String.h"

namespace BELogger
{
	void Init(bool bDebugConsoleEnabled, const char* logPath, const char* logName);
	void Destroy();

	void Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum, ...);
	void SetDisplayFlags(const char* tag, unsigned char flags);
}

#define BE_ERROR(str, ...) \
    do { \
        BELogger::Log("ERROR", str, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
		wchar_t formattedMsg[256]; \
		swprintf_s(formattedMsg, sizeof(formattedMsg), L"%hs", str); \
		swprintf_s(formattedMsg, sizeof(formattedMsg), formattedMsg, ##__VA_ARGS__); \
        if(g_pApp->GetHwnd() != NULL) { \
            MessageBox(g_pApp->GetHwnd(), formattedMsg, L"Error", MB_OK); \
		                } else { \
            OutputDebugString(formattedMsg); \
		                } \
	            } while(0) \


#define BE_ASSERT(expr) \
	do { \
		if (!(expr)) \
				{ \
			assert(expr); \
				} \
		} while (0) \

#define BE_ASSERT_MESSAGE(expr, message) \
	do { \
		if (!(expr)) \
					{ \
			assert(expr && message); \
					} \
		} while (0) \

#define BE_WARNING(str, ...) BELogger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)

#define BE_INFO(str, ...) BE_LOG("INFO", s, ##__VA_ARGS__)

#define BE_LOG_GRAPHICS(str, ...) BE_LOG("graphics", str, ##__VA_ARGS__)

#define BE_LOG(tag, str, ...) BELogger::Log(tag, s, NULL, NULL, 0, ##__VA_ARGS__)

#define BE_HRETURN(x, str) { hr = (x); if( FAILED(hr) ) { BE_ERROR( str ); return hr; } }