#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "header.h"
#include "patrol.h"
#include "board.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int appMode() const;
    Board* getBoard() const;

private:
    Ui::MainWindow *ui;
    Board *bd;
    Patrol *patrol;
    int mode;
    int userColor;
    QString serverIp;

    char ip[100];

    void initMode();
};

#endif // MAINWINDOW_H
