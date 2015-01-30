#include "bombasteditor.h"
#include "ui_bombasteditor.h"

BombastEditor::BombastEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BombastEditor)
{
    ui->setupUi(this);

    HINSTANCE hInstance = qWinAppInst();
    WId hwnd = ui->graphicsFrame->winId();
}

BombastEditor::~BombastEditor()
{
    delete ui;
}
