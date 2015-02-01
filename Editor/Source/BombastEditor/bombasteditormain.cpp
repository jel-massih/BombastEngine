#include "bombasteditormain.h"
#include "../Msvc/BombastEditorStd.h"
#include "BombastEditor.h"

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

void BombastEditorMain::closeEvent(QCloseEvent* event)
{
	g_BombastEditorApp.EditorQuit();
}