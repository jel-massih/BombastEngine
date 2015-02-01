#include "bombasteditormain.h"
#include "../Msvc/BombastEditorStd.h"

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