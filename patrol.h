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
    void run();

signals:
    void recvOthello(int, int);
    void patrolDead();
};

class ServerThread : public QThread
{
    Q_OBJECT
public:
    void run();

signals:
    void enemyReady();
    void fatalError();
};

class Patrol : public QObject
{
    Q_OBJECT
public:
    explicit Patrol(int server, QString serverIp, QObject *parent = 0);
    ~Patrol();

signals:
    void fatalError();
    void recvOthello(int, int);

public slots:
    void sendOthello(int, int);
    void startPatrol();

private:
    QString serverIp;

    ServerThread *serverThread;
    PatrolThread *patrolThread;

    const char *getIp();
    int isServer;

    int initClient();
    int initServer();
    void haltServer();
    void haltClient();
};

#endif // PATROL_H
