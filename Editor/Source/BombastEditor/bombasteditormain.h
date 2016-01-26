#pragma once

#include <QtWidgets/QMainWindow>

namespace Ui {
	class BombastEditor;
}

class BombastEditorMain : public QMainWindow
{
	Q_OBJECT

public:
	BombastEditorMain(QWidget *parent = 0);
	~BombastEditorMain();

	void InitializeEditor();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void InitEngine();
	void SetupContentTree();

	void closeEvent(QCloseEvent* event) override;

private:

	Ui::BombastEditor* ui;
};