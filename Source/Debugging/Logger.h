#ifndef LOGGER_H
#define LOGGER_H

#include <string>

// Call the BE_ERROR() macro to send out an error message
//		Example -> BE_ERROR("Something Blew Up!");
#define BE_ERROR(str) \
	std::wstring s((str)); \
	if(BombastApp::GetGameInstance()->GetHwnd() != NULL) { \
		MessageBox(BombastApp::GetGameInstance()->GetHwnd(), s.c_str(), L"Error", MB_OK); \
	} else { \
		OutputDebugString(s.c_str()); \
	} \

#endif