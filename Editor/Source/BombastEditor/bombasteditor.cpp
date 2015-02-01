#include "../Msvc/BombastEditorStd.h"
#include "BombastEditor.h"
#include "Physics/Physics.h"

#include <QtWidgets\qapplication.h>
#include <QtWidgets\qtextedit.h>

BombastEditorApp g_BombastEditorApp;

int main(int argv, char** args)
{
	QApplication app(argv, args);

	QTextEdit textEdit;
	textEdit.show();

	//return BombastEngine(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	return app.exec();
}

CoreGameLogic* BombastEditorApp::VCreateGameAndView()
{
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