#include "SocketHandler.h"
#include "CommunicationThread.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include "kenet_global.h"
#include <QTimer>
#include <QDebug>
SocketHandler::SocketHandler(QObject *parent) :
    QObject(parent),m_socket(0)
{
    this->connectTimeout = 3000;
    this->toTryConnect = false;
    initailize();
}



SocketHandler::~SocketHandler()
{
    if(m_socket){
        qDebug()<<"delete socket addr = "<<m_addr.toString()<<" port = "<<m_port;
        //m_socket->disconnectFromHost();
        this->sDisConnectFromHost();
        delete m_socket;
    }
}

void SocketHandler::initailize()
{
    this->moveToThread(&CommunicationThread::instance());
    CommunicationThread::instance().start();
    QObject::connect(this,&SocketHandler::sbCreateSocket,
                     this,&SocketHandler::sCreateSocket,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbConnectToHost,
                     this,&SocketHandler::sConnectToHost,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbConnectToAddr,
                     this,&SocketHandler::sConnectToAddr,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbDisConnectFromHost,
                     this,&SocketHandler::sDisConnectFromHost,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::sbWriteData,
                     this,&SocketHandler::sWriteData,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::saWriteData,
                     this,&SocketHandler::sWriteData);

    QObject::connect(this,&SocketHandler::sbSetSocketDescriptor,
                     this,&SocketHandler::sSetSocketDescriptor,
                     Qt::BlockingQueuedConnection);

    QObject::connect(this,&SocketHandler::saCleanUp,
                     this,&SocketHandler::sCleanUp);
}

void SocketHandler::cleanUp()
{
    emit saCleanUp();
}

int SocketHandler::CreateSocket(int socketType)
{
    QMutexLocker locker(&errorMutex);
    this->m_socketType = socketType;
    emit sbCreateSocket();
    return lastError;
}

int SocketHandler::TryConnectToHost(const QString &hostName, quint16 port)
{
    QMutexLocker locker(&errorMutex);
    this->toTryConnect = true;
    emit sbConnectToHost(hostName,port);
    return lastError;
}

int SocketHandler::ConnectToHost(const QString &hostName, quint16 port, int timeout)
{
    QMutexLocker locker(&errorMutex);
    this->toTryConnect = false;
    if(timeout != 0)
        this->connectTimeout = timeout;
    emit sbConnectToHost(hostName,port);
    return lastError;
}

int SocketHandler::ConnectToAddr(const QHostAddress &address, quint16 port, int timeout)
{
    QMutexLocker locker(&errorMutex);
    this->toTryConnect = false;
    if(timeout != 0)
        this->connectTimeout = timeout;
    emit sbConnectToAddr(address,port);
    return lastError;
}

bool SocketHandler::isValid()
{
    return this->m_socket->isValid();
}

int SocketHandler::DisConnectFromHost()
{
    //QMutexLocker locker(&errorMutex);
    emit sbDisConnectFromHost();
    return KE_SUCCESS;
}

int SocketHandler::WriteData(const QByteArray &byteArray)
{
    QMutexLocker locker(&errorMutex);
    emit sbWriteData(byteArray);
    return lastError;
}

int SocketHandler::SetSocketDescriptor(qintptr socketDescriptor)
{
    if(!m_socket){
        return KE_No_Initial;
    }
    //this->m_socket->setSocketDescriptor(socketDescriptor);
    //return 0;
    QMutexLocker locker(&errorMutex);
    emit sbSetSocketDescriptor(socketDescriptor);
    return lastError;
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
    if(this->m_socketType == 0){
        m_socket = new QTcpSocket();
    }
    else if(this->m_socketType == 1){
        m_socket = new QUdpSocket();
        bool b = this->m_socket->bind();
        if(!b){
            qWarning("bind error!");
        }
    }
    else{
        lastError = KE_Parameter_Error;
        return;
    }
    QObject::connect(m_socket,&QAbstractSocket::readyRead,
                     this,&SocketHandler::sReadData);
    QObject::connect(m_socket,&QAbstractSocket::disconnected,
                     this,&SocketHandler::saDisConnected);
    QObject::connect(m_socket,&QAbstractSocket::connected,
                     this,&SocketHandler::saConnected);
    lastError = KE_SUCCESS;
}

void SocketHandler::sConnectToHost(const QString &hostName, quint16 port)
{
    qDebug()<<"SocketHandler::sConnectToHost ip "<<hostName<<" port "<<port;
    this->m_addr = QHostAddress(hostName);
    this->m_port = port;
    if(this->m_socketType == QAbstractSocket::TcpSocket){
        this->m_socket->connectToHost(hostName,port);
        if(!this->toTryConnect && !this->m_socket->waitForConnected(connectTimeout)){
            qWarning("SocketHandler::sConnectToHost waitForConnected error");
            lastError = KE_NETCONNECT_ERROR;
            return;
        }
    }
    lastError = 0;
}

void SocketHandler::sConnectToAddr(const QHostAddress &address, quint16 port)
{
    this->m_addr = QHostAddress(address);
    this->m_port = port;
    if(this->m_socketType == QAbstractSocket::TcpSocket){
        this->m_socket->connectToHost(address,port);
        if(!this->m_socket->waitForConnected(connectTimeout)){
            qWarning("SocketHandler::sConnectToAddr waitForConnected error");
            lastError = KE_NETCONNECT_ERROR;
            return;
        }
    }

    lastError = 0;
}

void SocketHandler::sDisConnectFromHost()
{
    this->m_socket->disconnectFromHost();
    if(this->m_socket->state() == QAbstractSocket::UnconnectedState ||
            this->m_socket->waitForDisconnected(connectTimeout)){

        lastError = KE_SUCCESS;
        return;
    }
    qWarning("SocketHandler::sDisConnectFromHost waitForDisconnected error");

    lastError = KE_NETWORK_ERROR;
}

void SocketHandler::sWriteData(const QByteArray & byteArray)
{
    int ret = -1;
    if(this->m_socketType == QAbstractSocket::TcpSocket){
         ret = m_socket->write(byteArray);
    }
    else if(this->m_socketType == QAbstractSocket::UdpSocket){
        QUdpSocket * udp = qobject_cast<QUdpSocket *>(this->m_socket);
        ret = udp->writeDatagram(byteArray,this->m_addr,this->m_port);
    }
    if(ret == -1){
        lastError = KE_NETWRITE_ERROR;
        return;
    }
    lastError = 0;
}

void SocketHandler::sReadData()
{
    if(this->m_socketType == QAbstractSocket::TcpSocket)
    {
        QByteArray allBytes = m_socket->readAll();
        emit sdReadedData(allBytes);
    }
    else if(this->m_socketType == QAbstractSocket::UdpSocket)
    {
       QUdpSocket * udpSocket = qobject_cast<QUdpSocket *>(this->m_socket);
       while (udpSocket->hasPendingDatagrams())
       {
           QByteArray datagram;
           datagram.resize(udpSocket->pendingDatagramSize());
           int ret =  udpSocket->readDatagram(datagram.data(), datagram.size());
           if(ret == -1){
               qWarning("SocketHandler::sReadData readDatagram error!");
               return ;
           }
           emit sdReadedData(datagram);
       }
    }
}

void SocketHandler::sCleanUp()
{
    //this->deleteLater();
    delete this;
}

void SocketHandler::sSetSocketDescriptor(qintptr socketDescriptor)
{
    bool ret = this->m_socket->setSocketDescriptor(socketDescriptor);
    if(ret){
        this->m_addr = this->m_socket->peerAddress();
        this->m_port = this->m_socket->peerPort();
        this->lastError = KE_SUCCESS;
    }
    else{
        this->lastError = KE_NETCONNECT_ERROR;
    }

}
