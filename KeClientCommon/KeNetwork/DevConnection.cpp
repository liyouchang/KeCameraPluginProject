#include "DevConnection.h"
#include <QBuffer>
#include <QDebug>
#include <QCoreApplication>
#include "SocketHandler.h"
#include "KeProtocal/ProtocalProcess.h"
DevConnection::DevConnection(SocketHandler *s, ProtocalProcess *protocal, QObject *parent) :
    Device(s,protocal,parent)
{

    cbReconnect = 0;
    cbDisConnect = 0;
    userDisConnect =0;
    userReconnect = 0;
    this->toHoldSocket = true;
    protocal->setParent(this);

    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                     this,&DevConnection::GetMessageData,
                     Qt::DirectConnection);
}

DevConnection::DevConnection(SocketHandler *s, Device *parent):Device(s,parent)
{
     this->toHoldSocket = true;

    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                     this,&DevConnection::GetMessageData,
                     Qt::DirectConnection);
}

DevConnection::~DevConnection()
{
}

void DevConnection::GetMessageData(QByteArray & allBytes)
{
    this->m_protocal->ExtractMessage(allBytes,this);
}

void DevConnection::OnRespond(QByteArray &data)
{
    this->m_protocal->ParseMessage(data,this);
}

int DevConnection::ConnectDevice(const std::string & addr, int port)
{
    QString name = QString::fromStdString(addr);
    int ret =   m_socketHandle->ConnectToHost(name,port);
    return ret;
}

int DevConnection::DisConnect()
{
    return this->m_socketHandle->DisConnectFromHost();
}

int DevConnection::LoginServer(std::string username, std::string pwd)
{
    return 0;
}

int DevConnection::Logout()
{
    return 0;
}


