#include "BombastEditor.h"
#include "../Msvc/BombastEditorStd.h"
#include "Bombast\interfaces.h"
#include "Physics/Physics.h"
#include "bombasteditormain.h"

#include <qapplication.h>

BombastEditorApp g_BombastEditorApp;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	BombastEditorMain w;
	w.show();

	return a.exec();
}

INT EditorMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	HWND hWnd)
{
	bool bConsoleEnabled = false;

	//Setup Checks for memory leaks
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	//Used at Program Init to force leak check before exit
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(tmpDbgFlag);

	g_pApp->m_options.Init((ROOT_GAME_PATH + "Options.xml").c_str());

	BELogger::Init(g_pApp->m_options.m_bDebugConsoleEnabled, g_pApp->m_options.m_debugLogPath.c_str(), g_pApp->m_options.m_debugLogName.c_str());

	if (!g_pApp->InitInstance(hInstance, hWnd, g_pApp->m_options.m_screenSize.x, g_pApp->m_options.m_screenSize.y, g_pApp->m_options.m_screenPosition.x, g_pApp->m_options.m_screenPosition.y))
	{
		return FALSE;
	}

	return 0;
}

CoreGameLogic *BombastEditorApp::VCreateGameAndView()
{
	g_pApp->m_options.m_level = "World\\World.xml";

	m_pGame = BE_NEW BombastEditorLogic();
	m_pGame->Initialize();

	return m_pGame;
}

BombastEditorLogic::BombastEditorLogic()
{
	m_pGamePhysics = CreateGamePhysics();
}

BombastEditorLogic::~BombastEditorLogic()
{
}