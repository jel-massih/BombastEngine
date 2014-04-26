#pragma once

// Call the BE_ERROR() macro to send out an error message
//		Example -> BE_ERROR("Something Blew Up!");
#define BE_ERROR(str) if(g_pApp->GetHwnd() != NULL) {MessageBox(g_pApp->GetHwnd(), str, L"Error", MB_OK);} else {OutputDebugString(str);}