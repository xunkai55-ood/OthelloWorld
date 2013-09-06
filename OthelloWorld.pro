#-------------------------------------------------
#
# Project created by QtCreator 2013-09-04T09:06:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OthelloWorld
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modedialog.cpp \
    ipclient.cpp \
    ipserver.cpp \
    board.cpp \
    oalgo.cpp

HEADERS  += mainwindow.h \
    modedialog.h \
    header.h \
    ipclient.h \
    ipserver.h \
    board.h \
    oalgo.h

FORMS    += mainwindow.ui \
    modedialog.ui \
    ipclient.ui \
    ipserver.ui
