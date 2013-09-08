#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modedialog.h"
#include "ipclient.h"
#include "ipserver.h"
#include "patrol.h"

#include <QDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("黑白棋 - Othello World");
    initMode();
    if (mode == MODE_CLIENT)
    {
        patrol = new Patrol(0, serverIp, this);
        bd = new Board(0, this);
    }
    else
    {
        patrol = new Patrol(1, serverIp, this);
        bd = new Board(1, this);
    }

    connect(patrol, SIGNAL(patrolConnected()), bd, SLOT(gamePrepare()));


    if (mode == MODE_CLIENT)
        patrol->initClient();
    else
        patrol->initServer();

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
}

MainWindow::~MainWindow()
{
    delete patrol;
    delete bd;
    delete ui;
}

int MainWindow::appMode() const
{
    return mode;
}

Board *MainWindow::getBoard() const
{
    return bd;
}
