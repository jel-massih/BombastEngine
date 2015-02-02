#include "bombasteditormain.h"
#include "../Msvc/BombastEditorStd.h"
#include "BombastEditor.h"

#include <QKeyEvent>

BombastEditorMain::BombastEditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	InitEngine();
}

BombastEditorMain::~BombastEditorMain()
{
}

void BombastEditorMain::InitEngine()
{
	HINSTANCE hInstance = qWinAppInst();
	HINSTANCE hPrevInstance = qWinAppPrevInst();
	HWND hWnd = (HWND)ui.bombastViewport->winId();

	EditorMain(hInstance, hPrevInstance, hWnd);
}

void BombastEditorMain::keyPressEvent(QKeyEvent* event)
{
	int qKey = event->key();
	g_BombastEditorApp.OnViewportKeyDown((BYTE)qKey);
}

void BombastEditorMain::keyReleaseEvent(QKeyEvent* event)
{
	int qKey = event->key();
	g_BombastEditorApp.OnViewportKeyUp((BYTE)qKey);
}

void BombastEditorMain::closeEvent(QCloseEvent* event)
{
	g_BombastEditorApp.EditorQuit();
}