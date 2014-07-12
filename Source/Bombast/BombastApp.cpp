//========================================================================
// BombastApp.cpp : Defines Application Layer for BombastEngine
//========================================================================

#include "BombastApp.h"

#include <cstdio>

BombastApp* BombastApp::m_pAppInstance = 0;

BombastApp::BombastApp()
{
	m_hWnd = 0;

	m_iColorDepth = 32;

	m_bIsRunning = false;

	m_pGraphicsManager = 0;

	m_pEntitiesManager = 0;

	m_pBombastGame = 0;
}

//Win32 Specific Stuff
bool BombastApp::InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd, int screenWidth, int screenheight)
{
	if(!IsOnlyInstance(VGetGameTitle()))
	{
		return FALSE;
	}

	if(!CheckStorage(DISK_SPACE_NEEDED))
	{
		return FALSE;
	}
	
	if(!CheckMemory(PHYS_RAM_NEEDED, VIRT_RAM_NEEDED))
	{
		return FALSE;
	}

	if(ReadCPUSpeed() < CPU_SPEED_NEEDED)
	{
		BE_ERROR(L"CPUSpeed Error: You Do Not Have a Fast Enough CPU :(");
		return FALSE;
	}

	SetCursor(NULL);

	m_hInstance = hInstance;

	m_screenSize = Point(screenWidth, screenheight);

	InitializeWindows();

	if(!GetHwnd())
	{
		BE_ERROR(L"Windows Error: Could Not Create The Window");
		return FALSE;
	}

	if(!InitializeApp(screenWidth, screenheight))
	{
		BE_ERROR(L"App Error: Could Not Initialize Application layer");
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
                          0,    // x-position of the window
                          0,    // y-position of the window
                          SCREEN_WIDTH,    // width of the window
                          SCREEN_HEIGHT,    // height of the window
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
	bool result;

	m_pGraphicsManager = BE_NEW GraphicsManager();
	if (!m_pGraphicsManager)
	{
		return FALSE;
	}

	result = m_pGraphicsManager->Initialize(m_hWnd);
	if (!result)
	{
		return FALSE;
	}

	m_pEntitiesManager = BE_NEW EntitiesManager();
	if (!m_pEntitiesManager)
	{
		return FALSE;
	}

	m_pBombastGame = BE_NEW BombastGame();
	if (!m_pBombastGame)
	{
		return FALSE;
	}

	result = m_pBombastGame->Initialize(m_hWnd, m_hInstance);
	if (!result)
	{
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
	bool result;
	result = m_pGraphicsManager->Frame();
	if (!result) {
		return FALSE;
	}

	result = m_pBombastGame->Frame();
	if (!result)
	{
		return FALSE;
	}

	return true;
}

void BombastApp::ShutDown()
{
	if (m_pBombastGame)
	{
		m_pBombastGame->Shutdown();
		SAFE_DELETE(m_pBombastGame);
	}

	if (m_pEntitiesManager)
	{
		m_pEntitiesManager->Shutdown();
		SAFE_DELETE(m_pEntitiesManager);
	}

	if (m_pGraphicsManager)
	{
		m_pGraphicsManager->Shutdown();
		SAFE_DELETE(m_pGraphicsManager);
	}

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

EntitiesManager* BombastApp::GetEntitiesManager()
{
	return m_pEntitiesManager;
}

GraphicsManager* BombastApp::GetGraphicsManager()
{
	return m_pGraphicsManager;
}