#include "BombastApp.h"
#include "../Resources/ResourceCache.h"
#include "../Events/EventManagerImpl.h"
#include "..\Utilities\Timer.h"
#include "..\Graphics3D\SceneNode.h"

#include <cstdio>

BombastApp *g_pApp = NULL;

const char* LUA_PRE_INIT_FILE = "Scripts\\PreInit.lua";

BombastApp::BombastApp()
{
	g_pApp = this;

	m_hWnd = 0;

	m_iColorDepth = 32;

	m_bIsRunning = false;

	m_bQuitting = false;

	m_screenSize = Point(0, 0);

	m_pGraphicsManager = 0;

	m_pLuaCoreManager = 0;

	m_pResourceCache = 0;

	m_pGame = 0;

	m_pTimer = 0;
}

//Win32 Specific Stuff
bool BombastApp::InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenheight, int screenX, int screenY)
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
		BE_ERROR("CPUSpeed Error: You Do Not Have a Fast Enough CPU :(");
		return FALSE;
	}

	SetCursor(NULL);

	m_hInstance = hInstance;

	m_screenSize = Point(screenWidth, screenheight);
	m_screenPosition = Point(screenX, screenY);

	IResourceFile* zipFile = BE_NEW DevelopmentResourceZipFile(s2ws(ROOT_GAME_PATH + "Assets"));

	m_pResourceCache = BE_NEW ResourceCache(50, zipFile);

	if (!m_pResourceCache->Initialize())
	{
		BE_ERROR("Resource Error: Failed to initialize Resource Cashe");
		return false;
	}

	extern IResourceLoader* CreateXmlResourceLoader();
	extern IResourceLoader* CreateTextureResourceLoader();
	extern IResourceLoader* CreateLuaResourceLoader();
	extern IResourceLoader* CreateModelResourceLoader();

	m_pResourceCache->RegisterLoader(CreateXmlResourceLoader());
	m_pResourceCache->RegisterLoader(CreateTextureResourceLoader());
	m_pResourceCache->RegisterLoader(CreateLuaResourceLoader());
	m_pResourceCache->RegisterLoader(CreateModelResourceLoader());

	InitializeWindows();

	if(!GetHwnd())
	{
		BE_ERROR("Windows Error: Could Not Create The Window");
		return FALSE;
	}

	if(!InitializeApp(screenWidth, screenheight))
	{
		BE_ERROR("App Error: Could Not Initialize Application layer");
		return FALSE;
	}

	m_pGame = VCreateGameAndView();
	if (!m_pGame)
	{
		BE_ERROR("Game Error: Could Not Create The Game");
		return false;
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
                          m_screenPosition.x,    // x-position of the window
                          m_screenPosition.y,    // y-position of the window
                          m_screenSize.x,    // width of the window
                          m_screenSize.y,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          m_hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

    ShowWindow(m_hWnd, 10);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
}

LRESULT BombastApp::OnAltEnter()
{
	m_pGraphicsManager->GetRenderer()->VToggleFullscreen();
	return 0;
}

LRESULT CALLBACK BombastApp::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
		case WM_CLOSE:
		{
			if (g_pApp->m_bQuitting)
			{
				g_pApp->OnClose();
			}
			break;
		}

		case WM_SYSCOMMAND:
		{
			g_pApp->OnSysCommand(wParam, lParam);
			break;
		}

		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_RETURN)
			{
				return g_pApp->OnAltEnter();
			}

			break;
		}

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			if (g_pApp->m_pGame)
			{
				CoreGameLogic* pGame = g_pApp->m_pGame;

				AppMsg msg;
				msg.m_hWnd = hWnd;
				msg.m_uMsg = message;
				msg.m_wParam = wParam;
				msg.m_lParam = lParam;
				for (GameViewList::reverse_iterator it = pGame->m_gameViews.rbegin(); it != pGame->m_gameViews.rend(); it++)
				{
					if ((*it)->VOnMsgProc(msg))
					{
						return true;
					}
				}
			}

			break;
		}
    }

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT BombastApp::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case SC_MAXIMIZE:
		{
			if (m_bWindowedMode && IsRunning())
			{
				OnAltEnter();
			}
			return 0;
		}
		case SC_CLOSE:
		{
			m_bQuitting = true;
			return 0;
		}
		default:
			return DefWindowProc(GetHwnd(), WM_SYSCOMMAND, wParam, lParam);
	}

	return 0;
}

LRESULT BombastApp::OnClose()
{
	SAFE_DELETE(m_pGame);

	SAFE_DELETE(m_pEventManager);

	SAFE_DELETE(m_pResourceCache);

	PostQuitMessage(0);

	return 0;
}

bool BombastApp::InitializeApp(int screenWidth, int screenHeight)
{
	bool result;

	m_pTimer = BE_NEW Timer();
	if (!m_pTimer)
	{
		return false;
	}

	result = m_pTimer->Initialize();
	if (!result)
	{
		return false;
	}

	m_pGraphicsManager = BE_NEW GraphicsManager();
	if (!m_pGraphicsManager)
	{
		return false;
	}

	result = m_pGraphicsManager->Initialize(m_hWnd);
	if (!result)
	{
		return false;
	}

	m_pLuaCoreManager = BE_NEW LuaCoreManager();
	if(!m_pLuaCoreManager)
	{
		return false;
	}

	result = m_pLuaCoreManager->Initialize();
	if(!result)
	{
		return false;
	}

	//Load PreInit Lua File
	{
		Resource resource(LUA_PRE_INIT_FILE);
		ResourceHandle* pResourceHandle = g_pApp->m_pResourceCache->GetHandle(&resource);
	}

	m_pEventManager = BE_NEW EventManager("BombastApp Event Manager", true);
	if (!m_pEventManager)
	{
		BE_ERROR("FAiled to create Event Manager");
		return false;
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

			if (!Render())
			{
				bDone = true;
			}

			UpdateGame(m_pTimer->GetTime(), m_pTimer->GetFrameTime());
		}
	}

	return;
}

bool BombastApp::Frame()
{
	m_pTimer->Frame();

	return true;
}

bool BombastApp::Render()
{
	if (m_pGame)
	{
		m_pGame->VOnRender(m_pTimer->GetTime(), m_pTimer->GetFrameTime());
	}

	return true;
}

void BombastApp::ShutDown()
{
	SAFE_DELETE(m_pGame);

	SAFE_DELETE(m_pEventManager);

	if (m_pLuaCoreManager)
	{
		m_pLuaCoreManager->Shutdown();
		SAFE_DELETE(m_pLuaCoreManager);
	}

	if (m_pGraphicsManager)
	{
		m_pGraphicsManager->Shutdown();
		SAFE_DELETE(m_pGraphicsManager);
	}

	SAFE_DELETE(m_pResourceCache);

	SAFE_DELETE(m_pTimer);

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

GraphicsManager* BombastApp::GetGraphicsManager() const
{
	return m_pGraphicsManager;
}

LuaCoreManager* BombastApp::GetLuaCoreManager() const
{
	return m_pLuaCoreManager;
}

const Timer* BombastApp::GetTimer() const
{
	return m_pTimer;
}

bool BombastApp::VLoadGame()
{
	return m_pGame->VLoadGame(m_options.m_level.c_str());
}

void BombastApp::UpdateGame(double dTime, float fElapsedTime)
{
	if (g_pApp->m_bQuitting)
	{
		PostMessage(g_pApp->GetHwnd(), WM_CLOSE, 0, 0);
	}

	if (g_pApp->m_pGame)
	{
		IEventManager::Get()->VUpdate(20);

		g_pApp->m_pGame->VOnUpdate(dTime, fElapsedTime);
	}
}