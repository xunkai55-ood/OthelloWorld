#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "header.h"
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
    int mode;
    int userColor;
    QString serverIp;

    void initMode();
};

#endif // MAINWINDOW_H
