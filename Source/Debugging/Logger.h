#ifndef LOGGER_H
#define LOGGER_H

// Call the BE_ERROR() macro to send out an error message
//		Example -> BE_ERROR("Something Blew Up!");
#define BE_ERROR(str) if(BombastApp::GetGameInstance()->GetHwnd() != NULL) {MessageBox(BombastApp::GetGameInstance()->GetHwnd(), str, L"Error", MB_OK);} else {OutputDebugString(str);}

#endif