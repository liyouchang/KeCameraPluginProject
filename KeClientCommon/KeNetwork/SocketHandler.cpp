#include "SocketHandler.h"
#include "CommunicationThread.h"
#include <QTcpSocket>
#include <QDebug>
#include "kenet_global.h"
SocketHandler::SocketHandler(QObject *parent) :
    QObject(parent),m_socket(0)
{
    initailize();
}



SocketHandler::~SocketHandler()
{
    delete m_socket;
}

void SocketHandler::initailize()
{
    qDebug("SocketHandler::initailize start");
    this->moveToThread(&CommunicationThread::instance());
    CommunicationThread::instance().start();
    QObject::connect(this,&SocketHandler::sbCreateSocket,
                     this,&SocketHandler::sCreateSocket,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbConnectToHost,
                     this,&SocketHandler::sConnectToHost,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbDisConnectFromHost,
                     this,&SocketHandler::sDisConnectFromHost,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbWriteData,
                     this,&SocketHandler::sWriteData,
                     Qt::BlockingQueuedConnection);


    //
    qDebug("SocketHandler::initailize end");
}

void SocketHandler::CreateSocket()
{
    emit sbCreateSocket();
}

int SocketHandler::ConnectToHost(const QString &hostName, quint16 port)
{
    QMutexLocker locker(&errorMutex);
    emit sbConnectToHost(hostName,port);
    return lastError;
}

int SocketHandler::DisConnectFromHost()
{
    QMutexLocker locker(&errorMutex);
    emit sbDisConnectFromHost();
    return lastError;
}

int SocketHandler::WriteData(const QByteArray &byteArray)
{
    QMutexLocker locker(&errorMutex);
    emit sbWriteData(byteArray);
    return lastError;
}

int SocketHandler::SetSocketDescriptor(qintptr socketDescriptor)
{
    this->m_socket->setSocketDescriptor(socketDescriptor);
    return 0;
}

QByteArray SocketHandler::ReadAll()
{
    return m_socket->readAll();
}

QAbstractSocket *SocketHandler::getSocket()
{
    return m_socket;
}

void SocketHandler::sCreateSocket()
{
        //qDebug("SocketHandler::initailize new socket %d",type);
        m_socket = new QTcpSocket();

        //QObject::connect(m_socket,&QAbstractSocket::readyRead,
        //                 this,&SocketHandler::saReadyRead);
        QObject::connect(m_socket,&QAbstractSocket::readyRead,
                         this,&SocketHandler::sReadData);
        QObject::connect(m_socket,&QAbstractSocket::disconnected,
                         this,&SocketHandler::saDisConnected);
        QObject::connect(m_socket,&QAbstractSocket::connected,
                         this,&SocketHandler::saConnected);
}

void SocketHandler::sConnectToHost(const QString &hostName, quint16 port)
{
    this->m_socket->connectToHost(hostName,port);
    if(!this->m_socket->waitForConnected()){
        qWarning("SocketHandler::sConnectToHost waitForConnected error");
        lastError = KE_NETCONNECT_ERROR;
        return;
    }
    lastError = 0;
}

void SocketHandler::sConnectToAddr(const QHostAddress &address, quint16 port)
{
    this->m_socket->connectToHost(address,port);
    if(!this->m_socket->waitForConnected(2000)){
        qWarning("SocketHandler::sConnectToAddr waitForConnected error");
        lastError = KE_NETCONNECT_ERROR;
        return;
    }
    lastError = 0;
}

void SocketHandler::sDisConnectFromHost()
{
    this->m_socket->disconnectFromHost();
    if(!this->m_socket->waitForDisconnected(2000)){
        qWarning("SocketHandler::sDisConnectFromHost waitForDisconnected error");
        lastError = KE_NETWORK_ERROR;
        return;
    }
    lastError = 0;
}

void SocketHandler::sWriteData(const QByteArray & byteArray)
{
    int ret = m_socket->write(byteArray);
    if(ret == -1){

        lastError = KE_NETWRITE_ERROR;
        return;
    }
    lastError = 0;
}

void SocketHandler::sReadData()
{
    QByteArray allBytes = m_socket->readAll();
    emit sdReadedData(allBytes);
}
