#pragma once

#include "../Utilities/String.h"

namespace BELogger
{
	void Init(bool bDebugConsoleEnabled);
	void Destroy();

	void Log(const std::string& tag, const std::string& message, const char* func, const char* file, unsigned int lineNum);
	void SetDisplayFlags(const std::string& tag, unsigned char flags);
}

#define BE_ERROR(str) \
	do { \
		std::wstring s(s2ws(str)); \
		if(g_pApp->GetHwnd() != NULL) { \
			MessageBox(g_pApp->GetHwnd(), s.c_str(), L"Error", MB_OK); \
		} else { \
			OutputDebugString(s.c_str()); \
		} \
		} while(0) \

#define BE_ASSERT(expr) \
do \
{ \
	if (!(expr)) \
	{ \
		assert(expr); \
	} \
} \
while (0) \

#define BE_ASSERT_MESSAGE(expr, message) \
do \
{ \
	if (!(expr)) \
		{ \
		assert(expr && message); \
		} \
} \
while (0) \

#define BE_HRETURN(x, str)    { hr = (x); if( FAILED(hr) ) { BE_ERROR( str ); return hr; } }