#ifndef PATROL_H
#define PATROL_H

#define PORT 4000

#include <windows.h>
#include <QObject>
#include <QThread>

#include "header.h"
/* define some threads */

class PatrolThread : public QThread
{
    Q_OBJECT
public:
    explicit PatrolThread(QObject *parent);
    void run();

signals:
    void recvOthello(int, int);
    void patrolDead();
};

class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(QObject *parent);
    void run();

signals:
    void enemyReady();
    void fatalError();
    void patrolConnected();
};

class Patrol : public QObject
{
    Q_OBJECT
public:
    explicit Patrol(int server, QString serverIp, QObject *parent = 0);
    ~Patrol();
    int initClient();
    int initServer();

signals:
    void fatalError();
    void recvOthello(int, int);
    void patrolConnected();

public slots:
    void sendFatalError();
    void sendOthello(int, int);
    void startPatrol();

private:
    QString serverIp;

    ServerThread *serverThread;
    PatrolThread *patrolThread;

    const char *getIp();
    int isServer;

    void haltServer();
    void haltClient();
};

#endif // PATROL_H
