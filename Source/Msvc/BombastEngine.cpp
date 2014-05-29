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

	if(appInstance->InitInstance(hInstance, lpCmdLine, 0))
	{
		appInstance->Run();
	}

	appInstance->ShutDown();
	
	SAFE_DELETE(appInstance);

	return 0;
}