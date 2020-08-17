#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    mServer = new QTcpServer(this);
}

Server::~Server() {
    clear();
}

void Server::Connect(QString &addr, const int &p) {
    if(!mServer->listen(static_cast<QHostAddress>(addr), p)){
        qDebug() <<"Failed to listen " << addr << ":" << p;
        return;
    }
    mServer->waitForNewConnection(-1);
    mSocket = mServer->nextPendingConnection();
}

void Server::clear()
{
    killSocket();
    killServer();
}

void Server::killSocket()
{
    if(mSocket)
    {
        if(mSocket->isOpen())
            mSocket->close();
        mSocket->deleteLater();
        delete mSocket;
        this->mSocket = 0;
    }
}

void Server::killServer()
{
    if(mServer)
    {
        if(mServer->isListening())
             mServer->close();
        mServer->deleteLater();
        delete mServer;
        this->mServer = 0;
    }
}
