#include "..\Bombast\BombastApp.h"

LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	//Setup Checks for memory leaks
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	//Used at Program Init to force leak check before exit
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(tmpDbgFlag);

	BombastApp* appInstance = BombastApp::GetGameInstance();

	appInstance->m_options.Init((ROOT_GAME_PATH + "Options.xml").c_str(), lpCmdLine);

	if(appInstance->InitInstance(hInstance, lpCmdLine, 0, appInstance->m_options.m_screenSize.x, appInstance->m_options.m_screenSize.y ))
	{
		appInstance->Run();
	}

	appInstance->ShutDown();
	
	SAFE_DELETE(appInstance);

	return 0;
}