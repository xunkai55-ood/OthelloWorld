#include "patrol.h"

#include <iostream>
#include <QThread>
#include <QPoint>
#include <QDebug>

WSADATA Ws;
SOCKET ServerSocket, ClientSocket;
struct sockaddr_in LocalAddr, ClientAddr, ServerAddr;
int Ret = 0;
int AddrLen = 0;
char sendBuffer[MAX_PATH];

using namespace std;

PatrolThread::PatrolThread(QObject *parent) :
    QThread(parent)
{
}

ServerThread::ServerThread(QObject *parent) :
    QThread(parent)
{
}


/* defination of Patrol */

const char *Patrol::getIp()
{
    return serverIp.toStdString().c_str();
}

Patrol::Patrol(int server, QString ip, QObject *parent) :
    QObject(parent),
    isServer(server)
{
    serverThread = new ServerThread(this);
    patrolThread = new PatrolThread(this);

    connect(serverThread, SIGNAL(enemyReady()), this, SLOT(startPatrol()));
    connect(serverThread, SIGNAL(fatalError()), this, SIGNAL(fatalError()));
    connect(patrolThread, SIGNAL(recvOthello(int, int)),
            this, SIGNAL(recvOthello(int, int)));
    connect(patrolThread, SIGNAL(patrolDead()), this, SIGNAL(fatalError()));
    connect(this, SIGNAL(fatalError()), this, SLOT(sendFatalError()));

    serverIp = ip;
}

Patrol::~Patrol()
{
    if (isServer)
        haltServer();
    else
        haltClient();
    patrolThread->quit();
    serverThread->quit();
}

int Patrol::initClient()
{
    // Init Windows Socket
    if (WSAStartup(MAKEWORD(2,2), &Ws) != 0)
    {
        cout << "Init Windows Socket Failed::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }

    // Create Socket
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "Create Socket Failed::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(getIp());
    ServerAddr.sin_port = htons(PORT);
    memset(ServerAddr.sin_zero, 0x00, 8);

    Ret = ::connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (Ret == SOCKET_ERROR)
    {
        cout << "Connect Error::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }
    else
    {
        qDebug() << "Connect successfully!";
    }

    startPatrol();
}

int Patrol::initServer()
{
    // Init Windows Socket
    if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
    {
        cout << "Init Windows Socket Failed::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }

    // Create Socket
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == INVALID_SOCKET)
    {
        cout<<"Create Socket Failed::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }

    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_addr.s_addr = inet_addr(getIp());
    LocalAddr.sin_port = htons(PORT);
    memset(LocalAddr.sin_zero, 0x00, 8);

    // Bind Socket
    Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if (Ret != 0)
    {
        cout << "Bind Socket Failed::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }

    Ret = listen(ServerSocket, 10);
    if (Ret != 0)
    {
        cout << "listen Socket Failed::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }

    serverThread->start();
}

void ServerThread::run()
{
    AddrLen = sizeof(ClientAddr);
    ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "Accept Failed::" << GetLastError() << endl;
        emit fatalError();
        quit();
    }

    qDebug() << "connection::" << inet_ntoa(ClientAddr.sin_addr) << ":"
             << ClientAddr.sin_port;

    emit enemyReady();
}

void Patrol::startPatrol()
{
    emit patrolConnected();
    patrolThread->start();
}

void PatrolThread::run()
{
    int Ret = 0, x, y;
    char recvBuffer[MAX_PATH];

    while (true)
    {
        memset(recvBuffer, 0x00, sizeof(recvBuffer));
        Ret = recv(ClientSocket, recvBuffer, MAX_PATH, 0);
        qDebug("receive something %d", Ret);
        qDebug() << recvBuffer;
        if (Ret == SOCKET_ERROR)
        {
            qDebug("wrong!");
            emit patrolDead();
            break;
        }
        if (Ret == 0)
        {
            emit recvOthello(GAME_INFO, GAME_RUNAWAY);
            break;
        }
        sscanf(recvBuffer, "%d %d", &x, &y);
        emit recvOthello(x, y);
    }

    exit(0);
}

void Patrol::sendOthello(int x, int y)
{
    sprintf(sendBuffer, "%d %d", x, y);
    Ret = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
    if (Ret == SOCKET_ERROR)
        emit fatalError();
}

void Patrol::sendFatalError()
{
    sendOthello(GAME_INFO, GAME_FATALERROR);
}

void Patrol::haltServer()
{
    closesocket(ServerSocket);
    closesocket(ClientSocket);
    WSACleanup();
}

void Patrol::haltClient()
{
    closesocket(ClientSocket);
    WSACleanup();
}
