#ifndef BOMBASTEDITOR_H
#define BOMBASTEDITOR_H

#include <QMainWindow>

namespace Ui {
class BombastEditor;
}

class BombastEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit BombastEditor(QWidget *parent = 0);
    ~BombastEditor();

private:
    Ui::BombastEditor *ui;
};

#endif // BOMBASTEDITOR_H
