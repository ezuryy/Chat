#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    ~Server();
    void Connect(QString &addr,const int &p);
    void clear();
    void killSocket();
    void killServer();
    void onReadyRead();

public:
    QTcpSocket *mSocket;
    QTcpServer *mServer;
};

#endif // SERVER_H
