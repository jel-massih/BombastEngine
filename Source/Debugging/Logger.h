#pragma once

//---------------------------------------------------------------------------------------------------------------------
// TYPICAL USAGE:
// 
// 1) Call the BE_ASSERT() macro just like you would for _ASSERTE()
//		Example -> BE_ASSERT(ptr != NULL);
// 2) Call the BE_ERROR() macro to send out an error message
//		Example -> BE_ERROR("Something Blew Up!");
//---------------------------------------------------------------------------------------------------------------------

#define BE_ERROR(str) if(g_pApp->GetHwnd() != NULL) {MessageBox(g_pApp->m_hWnd, str, L"Error", MB_OK);} else {OutputDebugString(str);}