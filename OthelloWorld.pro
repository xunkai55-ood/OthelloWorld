#-------------------------------------------------
#
# Project created by QtCreator 2013-09-04T09:06:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OthelloWorld
TEMPLATE = app

LIBS += -lwsock32

SOURCES += main.cpp\
        mainwindow.cpp \
    modedialog.cpp \
    header.cpp \
    ipclient.cpp \
    ipserver.cpp \
    board.cpp \
    oalgo.cpp \
    choosecolor.cpp \
    patrol.cpp \
    extimer.cpp

HEADERS  += mainwindow.h \
    modedialog.h \
    header.h \
    ipclient.h \
    ipserver.h \
    board.h \
    oalgo.h \
    choosecolor.h \
    patrol.h \
    extimer.h

FORMS    += mainwindow.ui \
    modedialog.ui \
    ipclient.ui \
    ipserver.ui

RESOURCES += \
    ui.qrc
