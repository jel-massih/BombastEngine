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

	BombastApp* app = BombastApp::GetGameInstance();

	if (app->InitInstance(hInstance, lpCmdLine, 0))
	{
		app->Run();
	}

	app->ShutDown();
	
	SAFE_DELETE(app);

	return 0;
}