#pragma once
//===============================================================
// BombastApp.h : Defines the entry point of the application
//=============================================================
#include "..\BombastEngineStd.h"
#include "..\Constants.h"
#include "GraphicsManager.h"

class BombastApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance; //The Module Instance
	bool m_bWindowedMode; //True if app is windowed, False if Fullscreen
	bool m_bIsRunning; //true if everything initialized and game in main loop
	int m_iColorDepth;

	GraphicsManager* m_pGraphicsManager;

public:
	BombastApp();

	virtual TCHAR *VGetGameTitle() {return _T("Dope Engine");}

	HWND GetHwnd() {return m_hWnd;}
	HINSTANCE GetInstance() {return m_hInstance;}
	virtual bool InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool IsRunning() {return m_bIsRunning;}
	
	void Run();
	void ShutDown();

	bool IsOnlyInstance(LPCTSTR gameTitle);
	bool CheckStorage(const DWORDLONG diskSpaceNeeded);
	bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);
	DWORD ReadCPUSpeed();

private:
	void InitializeWindows();
	bool InitializeApp(int screenWidth, int screenHeight);

	bool Frame();

	void ShutdownWindows();
};

extern BombastApp *g_pApp;