#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    grid = new QGridLayout();
    lblServer = new QLabel(tr("Server : "));
    address = new QLineEdit(tr("127.0.0.1"));
    message = new QLineEdit();
    list = new QTextEdit();
    connectB = new QPushButton("Connect");
    disconnectB = new QPushButton("Disconnect");
    sendB = new QPushButton("Send");

    QString styleForTextEdit = "color: black;"
                        "background-color: white;"
                        "selection-color: white;"
                        "selection-background-color: #9A2EFE;";
    QString styleForButton = "color: black;"
                        "border-width: 0px;";
    address->setStyleSheet(styleForTextEdit);
    message->setStyleSheet(styleForTextEdit);
    list->setStyleSheet(styleForTextEdit);

    connectB->setStyleSheet(styleForButton);
    disconnectB->setStyleSheet(styleForButton);
    sendB->setStyleSheet(styleForButton);

    setStyleSheet("background-color: #E0F2F7;");

    grid->addWidget(lblServer, 0, 0);
    grid->addWidget(address, 0, 1);
    grid->addWidget(connectB, 0, 2);
    grid->addWidget(disconnectB, 0, 3);
    grid->addWidget(list, 1, 0, 1, 4);
    grid->addWidget(message, 2, 0, 2, 3);
    grid->addWidget(sendB, 2, 3, 2, 1);

    grid->setSpacing(10);
    grid->setMargin(30);

    widget = new QWidget();
    widget->setLayout(grid);
    setCentralWidget(widget);

    connect(connectB, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));
    connect(disconnectB, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));
    connect(sendB, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));
    connect(message, SIGNAL(returnPressed()), this, SLOT(onSendButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectButtonClicked()
{
    mClient = new QTcpSocket(this);
    mClient->connectToHost(address->text(), portNum);
    mClient->waitForConnected();
    isServer = false;

    if (mClient->state() == QTcpSocket::ConnectedState){
        mClient->write("<i>Successful connection :)</i>");

        connect(mClient, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));

    } else if (mClient->state() == QTcpSocket::UnconnectedState) {
        mServer = new Server(this);
        QString addr = address->text();
        mServer->Connect(addr, portNum);
        mServer->mSocket->write("<i>Successful connection :)</i>");
        isServer = true;

        connect(mServer->mSocket, SIGNAL(readyRead()), this, SLOT(onServReadyRead()));

    } else {
        list->append("<i>Something went wrong :(</i>");
        list->append("CLIENT STATE: " + static_cast<QString>(mClient->state()));
        list->append("SERVER STATE: " + static_cast<QString>(mServer->mSocket->state()));
    }
}

void MainWindow::onServReadyRead() {
        QByteArray dataToRead = mServer->mSocket->readAll();
        list->append("<b>Client: </b>" + dataToRead);
        if(dataToRead == "<i>Client has left.</i>") {
            isDisconnected = true;
        }
}

void MainWindow::onClientReadyRead(){
        QByteArray dataToRead = mClient->readAll();
        list->append("<b>Server: </b>" + dataToRead);
        if(dataToRead == "<i>Server has left.</i>") {
            isDisconnected = true;
        }
}

void MainWindow::onSendButtonClicked()
{
    QByteArray dataToWrite(message->text().toUtf8());
    message->clear();
    message->setFocus();
    if (isServer && mServer->mSocket->state()==QTcpSocket::ConnectedState && !isDisconnected) {
        mServer->mSocket->write( dataToWrite);
        list->append("<b>You: </b> " + dataToWrite);
    } else if(mClient->state() == QTcpSocket::ConnectedState && !isDisconnected){
        mClient->write(dataToWrite);
        list->append("<b>You: </b> " + dataToWrite);
    }
}


void MainWindow::onDisconnectButtonClicked()
{
    if (!isServer) {
        mClient->write("<i>Client has left.</i>");
        mClient->disconnectFromHost();
    } else {
        mServer->mSocket->write("<i>Server has left.</i>");
        mServer->mSocket->waitForBytesWritten();
        mServer->clear();
    }
    if (!isDisconnected)
        list->append("<i>You are disconnected.</i>");
    isDisconnected = true;
}

