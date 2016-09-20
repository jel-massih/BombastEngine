#pragma once

#include "..\BombastEngineStd.h"
#include "..\Constants.h"
#include "..\Graphics3D\GraphicsManager.h"
#include "..\Scripting\LuaCoreManager.h"
#include "..\Utilities\Initialization.h"
#include "CoreGameLogic.h"

class EventManager;
class Timer;
class SystemResourceMonitor;

class BombastApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	bool m_bWindowedMode;
	bool m_bIsRunning;
	bool m_bQuitting;
	int m_iColorDepth;
	Point m_screenSize;
	Point m_screenPosition;

	GraphicsManager* m_pGraphicsManager;
	LuaCoreManager* m_pLuaCoreManager;

	Timer* m_pTimer;

public:
	BombastApp();

    virtual TCHAR *VGetGameTitle() { return _TEXT("Bombast Engine"); }

	HWND GetHwnd() { return m_hWnd; }
	HINSTANCE GetInstance() { return m_hInstance; }
	virtual bool InitInstance(HINSTANCE hInstance, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT, int screenX = SCREEN_X, int screenY = SCREEN_Y);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnAltEnter();
	LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnClose();

	bool IsRunning() { return m_bIsRunning; }

	void Run();
	void ShutDown();

	GraphicsManager*  GetGraphicsManager() const { return m_pGraphicsManager; }
	LuaCoreManager*  GetLuaCoreManager() const { return m_pLuaCoreManager; }
	const Timer*  GetTimer() const { return m_pTimer; }

	CoreGameLogic* GetGameLogic() const { return m_pGame; }

	const Point &GetScreenSize()  { return m_screenSize; }

	struct GameOptions m_options;
	CoreGameLogic* m_pGame;

	class ResourceCache* m_pResourceCache;

	void UpdateGame(float deltaMs, double elapsedMs);

	virtual CoreGameLogic* VCreateGameAndView() = 0;
	virtual bool VLoadGame();

	EventManager* m_pEventManager;

	void AbortGame() { m_bQuitting = true; }

	bool Render();

protected:
	bool Frame();
	void SetQuitting(bool quitting) { m_bQuitting = quitting; }

private:
	void InitializeWindows();
	bool InitializeApp(int screenWidth, int screenHeight);
	
	bool InitializeResourceCache(std::string rootProjectPath);

	void ShutdownWindows();

	static BombastApp *m_pAppInstance;
};

extern BombastApp* g_pApp;
