#pragma once
//===============================================================
// BombastApp.h : Defines the entry point of the application
//=============================================================
#include "..\BombastEngineStd.h"
#include "..\Graphics3D\D3DClass.h"

class BombastApp
{
protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance; //The Module Instance
	bool m_bWindowedMode; //True if app is windowed, False if Fullscreen
	bool m_bIsRunning; //true if everything initialized and game in main loop
	bool m_bQuitRequested; //True if a quit was requested
	bool m_bQuitting;
	int m_iColorDepth;

public:
	BombastApp();

	virtual TCHAR *VGetGameTitle() {return _T("Dope Engine");}

	HWND GetHwnd() {return m_hWnd;}
	HINSTANCE GetInstance() {return m_hInstance;}
	virtual bool InitInstance(HINSTANCE hInstance, LPSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnClose();

	void AbortGame() {m_bQuitting = true;}
	bool IsRunning() {return m_bIsRunning;}
	void SetQuitting(bool bQuitting) {m_bQuitting = bQuitting;}

	void Run();
	void ShutDown();

private:
	D3DClass *m_pD3D;

private:
	void InitializeWindows();
	bool InitializeApp(int screenWidth, int screenHeight);

	bool Frame();

	void ShutdownWindows();
};

extern BombastApp *g_pApp;