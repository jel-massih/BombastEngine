#pragma once

//===============================================================
// BombastApp.h : Defines the entry point of the application
//=============================================================
#include "..\BombastEngineStd.h"
#include "..\Constants.h"
#include "..\Graphics3D\GraphicsManager.h"
#include "..\Scripting\LuaCoreManager.h"
#include "..\Utilities\Initialization.h"
#include "CoreGameLogic.h"
#include "..\Graphics3D\SceneNode.h"
#include "..\Events\EventManagerImpl.h"
#include "..\Utilities\Timer.h"

class BombastApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance; //The Module Instance
	bool m_bWindowedMode; //True if app is windowed, False if Fullscreen
	bool m_bIsRunning; //true if everything initialized and game in main loop
	bool m_bQuitting;
	int m_iColorDepth;
	Point m_screenSize;
	Point m_screenPosition;

	GraphicsManager* m_pGraphicsManager;
	LuaCoreManager* m_pLuaCoreManager;

	Timer* m_pTimer;

public:
	BombastApp();

	virtual TCHAR *VGetGameTitle() { return _T("Bombast Engine"); }

	HWND GetHwnd() { return m_hWnd; }
	HINSTANCE GetInstance() { return m_hInstance; }
	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT, int screenX = SCREEN_X, int screenY = SCREEN_Y);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnAltEnter();
	LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnClose();

	bool IsRunning() { return m_bIsRunning; }

	void Run();
	void ShutDown();

	GraphicsManager*  GetGraphicsManager() const;
	LuaCoreManager*  GetLuaCoreManager() const;

	CoreGameLogic* GetGameLogic() const { return m_pGame; }

	const Point &GetScreenSize()  { return m_screenSize; }

	struct GameOptions m_options;
	CoreGameLogic* m_pGame;

	class ResourceCache* m_pResourceCache;

	void UpdateGame(double dTime, float fElapsedTime);

	virtual CoreGameLogic* VCreateGameAndView() = 0;
	virtual bool VLoadGame();

	EventManager* m_pEventManager;

private:
	void InitializeWindows();
	bool InitializeApp(int screenWidth, int screenHeight);

	void AbortGame() { m_bQuitting = true; }
	void SetQuitting(bool quitting) { m_bQuitting = quitting; }

	bool Frame();
	bool Render();

	void ShutdownWindows();

	static BombastApp *m_pAppInstance;
};

extern BombastApp* g_pApp;
