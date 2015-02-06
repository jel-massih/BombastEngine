#pragma once

#include "../Utilities/String.h"

namespace BELogger
{
	void Init(bool bDebugConsoleEnabled, const char* logPath, const char* logName);
	void Destroy();

	void Log(const char* tag, const char* message, const char* func, const char* file, unsigned int lineNum, ...);
	void SetDisplayFlags(const char* tag, unsigned char flags);
}

#define BE_ERROR(str) \
	do { \
		BELogger::Log("ERROR", str, __FUNCTION__, __FILE__, __LINE__); \
		std::wstring s(s2ws(str)); \
		if(g_pApp->GetHwnd() != NULL) { \
			MessageBox(g_pApp->GetHwnd(), s.c_str(), L"Error", MB_OK); \
				} else { \
			OutputDebugString(s.c_str()); \
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

#define BE_WARNING(str) \
	do { \
		std::string s(str); \
		BELogger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__); \
		} while(0) \

#define BE_INFO(str) \
	do { \
		std::string s(str); \
		BELogger::Log("INFO", s, NULL, NULL, 0); \
			} while(0) \

#define BE_LOG_GRAPHICS(str) BE_LOG("graphics", str);

#define BE_LOG(tag, str, ...) \
	do { \
		std::string s(str); \
		BELogger::Log(tag, s, NULL, NULL, 0); \
			} while(0) \

#define BE_HRETURN(x, str)    { hr = (x); if( FAILED(hr) ) { BE_ERROR( str ); return hr; } }