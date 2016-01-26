#include "BombastEditor.h"
#include "../Msvc/BombastEditorStd.h"
#include "Bombast\interfaces.h"
#include "BombastEditorLogic.h"
#include "bombasteditormain.h"
#include "Utilities\Timer.h"

#include <qapplication.h>

BombastEditorApp g_BombastEditorApp;

std::string ROOT_GAME_PATH = "../../../../Engine/Game/";

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BombastEditorMain editor;
	editor.show();
	editor.InitializeEditor();

	g_BombastEditorApp.SetEditor(&editor);

	return a.exec();
}

INT EditorMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	HWND hWnd)
{
	bool bConsoleEnabled = false;

	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(tmpDbgFlag);

	g_pApp->m_options.Init((ROOT_GAME_PATH + "Options.xml").c_str());

	BELogger::Init(g_pApp->m_options.m_bDebugConsoleEnabled, g_pApp->m_options.m_debugLogPath.c_str(), g_pApp->m_options.m_debugLogName.c_str());

	if (!g_pApp->InitInstance(hInstance, hWnd))
	{
		return FALSE;
	}

	return TRUE;
}

BombastEditorApp::~BombastEditorApp()
{
	SAFE_DELETE(m_mainEditor);
}

void BombastEditorApp::OnIdleThread()
{
	Frame();
	Render();
	UpdateGame(m_pTimer->GetFrameTime(), m_pTimer->GetTime());
}

void BombastEditorApp::SetEditor(BombastEditorMain* pEditor)
{
	m_mainEditor = pEditor;

	m_messageHandler.Init(this);
}

void BombastEditorApp::OnViewportKeyDown(BYTE keyCode)
{
	static_cast<BombastEditorLogic*>(m_pGame)->OnKeyDown(keyCode);
}

void BombastEditorApp::OnViewportKeyUp(BYTE keyCode)
{
	static_cast<BombastEditorLogic*>(m_pGame)->OnKeyUp(keyCode);
}

void BombastEditorApp::OnViewportMouseDown(const int posX, const int posY, const std::string& keyName)
{
	static_cast<BombastEditorLogic*>(m_pGame)->OnMouseDown(Point(posX, posY), keyName);
}

void BombastEditorApp::OnViewportMouseUp(const int posX, const int posY, const std::string& keyName)
{
	static_cast<BombastEditorLogic*>(m_pGame)->OnMouseUp(Point(posX, posY), keyName);
}

CoreGameLogic *BombastEditorApp::VCreateGameAndView()
{
	m_pGame = BE_NEW BombastEditorLogic();
	m_pGame->Initialize();

	return m_pGame;
}

void BombastEditorApp::EditorQuit()
{
	SetQuitting(true);
	OnClose();

	ShutDown();

	BELogger::Destroy();
}