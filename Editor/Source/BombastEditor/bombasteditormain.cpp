#include "bombasteditormain.h"
#include "ui_bombasteditormain.h"
#include "../Msvc/BombastEditorStd.h"
#include "BombastEditor.h"

#include <QKeyEvent>
#include <QFileSystemModel>

BombastEditorMain::BombastEditorMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui->setupUi(this);
}

BombastEditorMain::~BombastEditorMain()
{
}

void BombastEditorMain::InitializeEditor()
{
	SetupContentTree();
	InitEngine();
}

void BombastEditorMain::InitEngine()
{
	HINSTANCE hInstance = qWinAppInst();
	HINSTANCE hPrevInstance = qWinAppPrevInst();
	HWND hWnd = (HWND)ui->bombastViewport->winId();

	EditorMain(hInstance, hPrevInstance, hWnd);
}

void BombastEditorMain::SetupContentTree()
{
	QFileSystemModel *model = new QFileSystemModel;
	ui->contentTree->setModel(model);
	//ui->contentTree->setRootIndex(model->setRootPath("D:/Projects/BombastEngine/Engine/Assets"));
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

void BombastEditorMain::mousePressEvent(QMouseEvent* event)
{
	std::string keyName;
	switch (event->button()) {
		case Qt::MouseButton::LeftButton:
			keyName = "PointerLeft";
			break;
		case Qt::MouseButton::RightButton:
			keyName = "PointerRight";
			break;
		default:
			return;
	}

	g_BombastEditorApp.OnViewportMouseDown(event->x(), event->y(), keyName);
}

void BombastEditorMain::mouseReleaseEvent(QMouseEvent* event)
{
	std::string keyName;
	switch (event->button()) {
	case Qt::MouseButton::LeftButton:
		keyName = "PointerLeft";
		break;
	case Qt::MouseButton::RightButton:
		keyName = "PointerRight";
		break;
	default:
		return;
	}

	g_BombastEditorApp.OnViewportMouseUp(event->x(), event->y(), keyName);
}