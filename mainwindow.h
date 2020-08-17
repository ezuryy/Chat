#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "server.h"

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStylePainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectButtonClicked();
    void onServReadyRead();
    void onClientReadyRead();
    void onSendButtonClicked();
    void onDisconnectButtonClicked();

private:
    Ui::MainWindow *ui;
    QLabel *lblServer;
    QLineEdit *address, *message;
    QTextEdit *list;
    QPushButton *connectB, *disconnectB, *sendB;
    QGridLayout *grid;
    QWidget *widget;

    qintptr portNum = 5555;
    Server *mServer;
    QTcpSocket *mClient;
    bool isServer;
    bool isDisconnected = false;
};
#endif // MAINWINDOW_H
