#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modedialog.h"
#include "ipclient.h"
#include "ipserver.h"

#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMode();
    if (mode == MODE_CLIENT)
        bd = new Board(0, this);
    else
        bd = new Board(1, this);

    setCentralWidget(bd);
}

void MainWindow::initMode()
{
    ModeDialog md;
    mode = md.exec();
    if (mode == MODE_CLIENT)
    {
        IpClient dlg;
        if (!dlg.exec())
        {
            mode = MODE_NONE;
        }
        else
        {
            serverIp = dlg.getIp();
        }
    }
    else if (mode == MODE_SERVER)
    {
        IpServer dlg;
        if (!dlg.exec())
        {
            mode = MODE_NONE;
        }
        else
        {
            serverIp = dlg.getIp();
        }
    }
    qDebug("%d", mode);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete bd;
}

int MainWindow::appMode() const
{
    return mode;
}

Board *MainWindow::getBoard() const
{
    return bd;
}
