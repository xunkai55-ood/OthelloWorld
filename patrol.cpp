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

/* defination of Patrol */

const char *Patrol::getIp()
{
    return serverIp.toStdString().c_str();
}

Patrol::Patrol(int server, QString ip, QObject *parent) :
    QObject(parent),
    isServer(server)
{
    serverThread = new ServerThread();
    patrolThread = new PatrolThread();

    serverIp = ip;
    int flag;
    if (isServer)
        flag = initServer();
    else
        flag = initClient();
    if (flag != 0)
    {
        emit fatalError();
        return;
    }

    connect(serverThread, SIGNAL(enemyReady()), this, SLOT(startPatrol()));
    connect(serverThread, SIGNAL(fatalError()), this, SIGNAL(fatalError()));
    connect(patrolThread, SIGNAL(recvOthello(int, int)),
            this, SIGNAL(recvOthello(int, int)));
    connect(patrolThread, SIGNAL(patrolDead()), this, SIGNAL(fatalError()));
}

Patrol::~Patrol()
{
    if (isServer)
        haltServer();
    else
        haltClient();
    patrolThread->quit();
    serverThread->quit();
    delete patrolThread;
    delete serverThread;
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
        cout<<"Create Socket Failed::"<<GetLastError()<<endl;
        emit fatalError();
        return -1;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(getIp());
    ServerAddr.sin_port = htons(PORT);
    memset(ServerAddr.sin_zero, 0x00, 8);
    qDebug("ip : %s", getIp());
    qDebug() << ServerAddr.sin_addr.s_addr;
    qDebug() << htons(PORT);

    Ret = ::connect(ClientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (Ret == SOCKET_ERROR)
    {
        cout << "Connect Error::" << GetLastError() << endl;
        emit fatalError();
        return -1;
    }
    else
    {
        qDebug() << "连接成功!" << endl;
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
    qDebug() << LocalAddr.sin_addr.s_addr;
    qDebug() << LocalAddr.sin_port;

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

    qDebug() << "server start!";

    serverThread->start();
}

void ServerThread::run()
{
    sleep(200);
    AddrLen = sizeof(ClientAddr);
    ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "Accept Failed::" << GetLastError() << endl;
        emit fatalError();
        quit();
    }

    qDebug() << "connection::" << inet_ntoa(ClientAddr.sin_addr) << ":"
             << ClientAddr.sin_port << endl;

    emit enemyReady();
}

void Patrol::startPatrol()
{
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
        if (Ret == SOCKET_ERROR)
        {
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
