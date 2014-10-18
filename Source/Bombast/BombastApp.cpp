//========================================================================
// BombastApp.cpp : Defines Application Layer for BombastEngine
//========================================================================

#include "BombastApp.h"
#include "../Resources/ResourceCache.h"

#include <cstdio>

BombastApp *g_pApp = NULL;

const char* LUA_PRE_INIT_FILE = "Scripts\\PreInit.lua";

BombastApp::BombastApp()
{
	g_pApp = this;

	m_hWnd = 0;

	m_iColorDepth = 32;

	m_bIsRunning = false;

	m_screenSize = Point(0, 0);

	m_pLuaCoreManager = 0;

	m_pGraphicsManager = 0;

	m_pEntitiesManager = 0;

	m_pResourceCache = 0;

	m_pGame = 0;
}

//Win32 Specific Stuff
bool BombastApp::InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenheight)
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

	m_pResourceCache->RegisterLoader(CreateXmlResourceLoader());
	m_pResourceCache->RegisterLoader(CreateTextureResourceLoader());
	m_pResourceCache->RegisterLoader(CreateLuaResourceLoader());

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
                          0,    // x-position of the window
                          0,    // y-position of the window
                          m_options.m_screenSize.x,    // width of the window
                          m_options.m_screenSize.y,    // height of the window
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
		return false;
	}

	result = m_pGraphicsManager->Initialize(m_hWnd);
	if (!result)
	{
		return false;
	}

	m_pEntitiesManager = BE_NEW EntitiesManager();
	if (!m_pEntitiesManager)
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

	return true;
}

void BombastApp::ShutDown()
{
	SAFE_DELETE(m_pGame);

	if (m_pLuaCoreManager)
	{
		m_pLuaCoreManager->Shutdown();
		SAFE_DELETE(m_pLuaCoreManager);
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

	SAFE_DELETE(m_pResourceCache);

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

EntitiesManager* BombastApp::GetEntitiesManager() const
{
	return m_pEntitiesManager;
}

GraphicsManager* BombastApp::GetGraphicsManager() const
{
	return m_pGraphicsManager;
}

LuaCoreManager* BombastApp::GetLuaCoreManager() const
{
	return m_pLuaCoreManager;
}

bool BombastApp::VLoadGame()
{
	return m_pGame->VLoadGame(m_options.m_level.c_str());
}