//========================================================================
// BombastApp.cpp : Defines Application Layer for BombastEngine
//========================================================================

#include "BombastApp.h"

BombastApp *g_pApp;

BombastApp::BombastApp()
{
	g_pApp = this;

	m_iColorDepth = 32;

	m_bIsRunning = false;
	
	m_bQuitRequested = false;
	m_bQuitting = false;
}

//Win32 Specific Stuff

bool BombastApp::InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd, int screenWidth, int screenheight)
{
	SetCursor(NULL);

	m_hInstance = hInstance;

	InitializeWindows();

	if(!GetHwnd())
	{
		return FALSE;
	}

	if(!InitializeApp(screenWidth, screenheight))
	{
		return FALSE;
	}

	m_bIsRunning = true;

	return true;
}

void BombastApp::InitializeWindows()
{
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MsgProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"BombastEngine";

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    m_hWnd = CreateWindowEx(NULL,
                          L"BombastEngine",    // name of the window class
                          VGetGameTitle(),   // title of the window
                          WS_OVERLAPPEDWINDOW,    // window style
                          300,    // x-position of the window
                          300,    // y-position of the window
                          500,    // width of the window
                          400,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          m_hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    ShowWindow(m_hWnd, 10);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

LRESULT CALLBACK BombastApp::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage(0);
            return 0;
        }

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

bool BombastApp::InitializeApp(int screenWidth, int screenHeight)
{
	m_pD3D = BE_NEW(D3DClass);
	if(!m_pD3D)
	{
		return FALSE;
	}

	if(!m_pD3D->Initialize(screenWidth, screenHeight, true, m_hWnd, false, 1000.0f, 0.1f))
	{
		MessageBox(m_hWnd, L"Could not initialize D3D11", L"Error", MB_OK);
		return FALSE;
	}

	return true;
}

void BombastApp::Run()
{
	MSG msg;
	bool bDone;

	ZeroMemory(&msg, sizeof(msg));

	bDone = false;
	while(!bDone)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
		{
			bDone = true;
		}
		else
		{
			if(!Frame())
			{
				bDone = true;
			}
		}
	}

	return;
}

bool BombastApp::Frame()
{
	return true;
}

void BombastApp::ShutDown()
{
	ShutdownWindows();
}

void BombastApp::ShutdownWindows()
{
	ShowCursor(true);

	if(!m_bWindowedMode)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	UnregisterClass(VGetGameTitle(), m_hInstance);
	m_hInstance = NULL;

	return;
}