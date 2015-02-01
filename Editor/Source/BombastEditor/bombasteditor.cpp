#include "bombasteditor.h"
#include "../Msvc/BombastEditorStd.h"
#include "Bombast\interfaces.h"
#include "Physics/Physics.h"

#include <qapplication.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	HINSTANCE hInstance = qWinAppInst();
	HINSTANCE hPrevInstance = qWinAppPrevInst();

	//return BombastEngine(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	BombastEditor w;
	w.show();


	return a.exec();
}

BombastEditor::BombastEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

BombastEditor::~BombastEditor()
{

}
