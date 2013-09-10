#include "mainwindow.h"
#include <QApplication>

#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f("://main.qss");
    qDebug() << f.open(QFile::ReadOnly);
    QTextStream str(&f);
    QString style = str.readAll();
    qDebug() << style;
    a.setStyleSheet(style);

    MainWindow w;
    w.show();

    return a.exec();
}
