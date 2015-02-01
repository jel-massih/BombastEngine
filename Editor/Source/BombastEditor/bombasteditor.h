#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_bombasteditor.h"

class BombastEditor : public QMainWindow
{
	Q_OBJECT

public:
	BombastEditor(QWidget *parent = 0);
	~BombastEditor();

private:
	Ui::BombastEditor ui;
};