#include "../Msvc/BombastEditorStd.h"
#include "BombastEditor.h"

#include "Physics/Physics.h"

BombastEditorApp g_BombastEditorApp;

INT WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	return BombastEngine(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

CoreGameLogic* BombastEditorApp::VCreateGameAndView()
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