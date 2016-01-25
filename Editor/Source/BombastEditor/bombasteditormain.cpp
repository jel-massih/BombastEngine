#include "bombasteditormain.h"
#include "../Msvc/BombastEditorStd.h"
#include "BombastEditor.h"

#include <QKeyEvent>
#include <QFileSystemModel>
#include <QTreeView>

BombastEditorMain::BombastEditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	InitEngine();
	SetupContentTree();
}

BombastEditorMain::~BombastEditorMain()
{
}

void BombastEditorMain::InitEngine()
{
	//HINSTANCE hInstance = qWinAppInst();
	//HINSTANCE hPrevInstance = qWinAppPrevInst();
	//HWND hWnd = (HWND)ui.bombastViewport->winId();

	//EditorMain(hInstance, hPrevInstance, hWnd);
}

void BombastEditorMain::SetupContentTree()
{
	QFileSystemModel *model = new QFileSystemModel;
	ui.contentTree->setModel(model);
	ui.contentTree->setRootIndex(model->setRootPath("D:/Projects/BombastEngine/Engine/Assets"));
}

void BombastEditorMain::keyPressEvent(QKeyEvent* event)
{
	int qKey = event->nativeVirtualKey();
	g_BombastEditorApp.OnViewportKeyDown((BYTE)qKey);
}

void BombastEditorMain::keyReleaseEvent(QKeyEvent* event)
{
	int qKey = event->nativeVirtualKey();
	g_BombastEditorApp.OnViewportKeyUp((BYTE)qKey);
}

void BombastEditorMain::closeEvent(QCloseEvent* event)
{
	g_BombastEditorApp.EditorQuit();
}