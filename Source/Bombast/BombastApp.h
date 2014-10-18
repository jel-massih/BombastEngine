#ifndef BOMBAST_APP_H
#define BOMBAST_APP_H

//===============================================================
// BombastApp.h : Defines the entry point of the application
//=============================================================
#include "..\BombastEngineStd.h"
#include "..\Constants.h"
#include "../Graphics3D/GraphicsManager.h"
#include "../Entity/EntitiesManager.h"
#include "../Scripting/LuaCoreManager.h"
#include "../Utilities/Initialization.h"
#include "CoreGameLogic.h"

class BombastApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance; //The Module Instance
	bool m_bWindowedMode; //True if app is windowed, False if Fullscreen
	bool m_bIsRunning; //true if everything initialized and game in main loop
	int m_iColorDepth;
	Point m_screenSize;

	GraphicsManager* m_pGraphicsManager;
	EntitiesManager* m_pEntitiesManager;
	LuaCoreManager* m_pLuaCoreManager;

public:
	BombastApp();

	virtual TCHAR *VGetGameTitle() { return _T("Bombast Engine"); }

	HWND GetHwnd() { return m_hWnd; }
	HINSTANCE GetInstance() { return m_hInstance; }
	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool IsRunning() { return m_bIsRunning; }

	void Run();
	void ShutDown();

	EntitiesManager*  GetEntitiesManager() const;
	GraphicsManager*  GetGraphicsManager() const;
	LuaCoreManager*  GetLuaCoreManager() const;

	CoreGameLogic* GetGameLogic() const { return m_pGame; }


	static BombastApp *GetGameInstance()
	{
		if (!m_pAppInstance)
			m_pAppInstance = BE_NEW BombastApp;
		return m_pAppInstance;
	}

	const Point &GetScreenSize()  { return m_screenSize; }

	struct GameOptions m_options;
	CoreGameLogic* m_pGame;

	class ResourceCache *m_pResourceCache;

	virtual bool VCreateGame();
	virtual bool VLoadGame();

private:
	void InitializeWindows();
	bool InitializeApp(int screenWidth, int screenHeight);

	bool Frame();

	void ShutdownWindows();

	static BombastApp *m_pAppInstance;
};

#endif