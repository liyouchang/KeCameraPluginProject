#include "DevConnection.h"
#include <QBuffer>
#include <QDebug>
#include <QCoreApplication>
#include "SocketHandler.h"
#include "KeProtocal/ProtocalProcess.h"
#include "KeProtocal/KeMessage.h"
#include "KeProtocal/ChAskKey.h"
#include "KeProtocal/ChLogin.h"
DevConnection::DevConnection(SocketHandler *s, ProtocalProcess *protocal, Device *parent) :
    Device(s,protocal,parent)
{
    this->toHoldSocket = true;
    heartbeatTimer = new QTimer(this);
    this->heartCount = 0;
    QObject::connect(heartbeatTimer,&QTimer::timeout,this,&DevConnection::HeartBeat);
    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                     this,&DevConnection::GetMessageData,Qt::DirectConnection);

    QObject::connect(m_socketHandle,&SocketHandler::saConnected,
                     this,&DevConnection::DevConnected);

    QObject::connect(m_socketHandle,&SocketHandler::saDisConnected,
                     this,&DevConnection::DevDisconnect);

    this->askKey = 0;
    this->login = 0;
    this->toReconnect = false;

    reconnectTimer = new QTimer(this);
    QObject::connect(reconnectTimer,&QTimer::timeout,this,&DevConnection::Reconnect);

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
    heartbeatTimer->deleteLater();
    reconnectTimer->deleteLater();
    delete m_protocal;

}

void DevConnection::GetMessageData(QByteArray & allBytes)
{
    this->m_protocal->ExtractMessage(allBytes,this);
}

void DevConnection::OnRespond(QByteArray &data)
{
    this->m_protocal->ParseMessage(data,this);
}

void DevConnection::HeartBeat()
{
    if(heartCount > 3){
        qWarning("heart beat loose 3 times, connection end");
        this->DisConnect();
    }
    ++heartCount;
    this->Request();
}

void DevConnection::Reconnect()
{
    qDebug("reconnect to addr %s,port %d",m_addr.c_str(),m_port);
    this->TryConnect(m_addr,m_port);
}

void DevConnection::DevDisconnect()
{
    qDebug("DevConnection::DevDisconnect");
    this->heartbeatTimer->stop();
    if(this->cbConnectStatus){
        //QByteArray ipAddr = this->m_socketHandle->m_addr.toString().toLatin1();
        //this->cbDisConnect(this->getHandler(),ipAddr.data(),this->m_socketHandle->m_port,this->userDisConnect);
        this->cbConnectStatus(this->getHandler(),Connect_Miss,this->userConnectStatus);
    }
    //start reconnect
    if(this->toReconnect)
    {
        this->reconnectTimer->start(10000);
    }
}

void DevConnection::DevConnected()
{
    qDebug("DevConnection::DevConnected");
    this->heartbeatTimer->start(this->netParam.heartbeatIntervalTime);
    if(this->cbConnectStatus){
        if(!this->toReconnect ){
            this->cbConnectStatus(this->getHandler(),Connect_Success,this->userConnectStatus);
        }
        if(this->toReconnect){
            this->cbConnectStatus(this->getHandler(),Connect_Again,this->userConnectStatus);
        }
    }
    //stop reconnect
    this->reconnectTimer->stop();
}

int DevConnection::ConnectDevice(const std::string & addr, int port)
{
    QString name = QString::fromStdString(addr);
    int ret =   m_socketHandle->ConnectToHost(name,port,this->netParam.connectTimeout);
    if(ret == KE_SUCCESS){
        //start heartbeat
    }
    return ret;
}

int DevConnection::DisConnect()
{
    toReconnect = false;
    return this->m_socketHandle->DisConnectFromHost();
}

int DevConnection::LoginServer(std::string username, std::string pwd)
{
    if(username.size() > 8 || pwd.size() > 8){
        return KE_Parameter_Error;
    }
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }
    static int PUID = 0;

    this->m_channelID = PUID;
    if(!this->askKey){
        this->askKey = new ChSecretKey(this);

    }
    this->askKey->Request();
    if(!askKey->waitRespond(this->netParam.waitTimeout)){
        qWarning("wait ChAskKey respond error!");
        return  KE_Msg_Timeout;
    }
    this->setClientID(askKey->getClientID());
    if(!this->login)
        this->login = new ChLogin(this);
    login->makeMd5(QString::fromLatin1(username.c_str()),
                   QString::fromLatin1(pwd.c_str()),askKey->m_skey);
    login->Request();
    if(!login->waitRespond(this->netParam.waitTimeout)){
        qWarning("wait login respond error!");
        return KE_Msg_Timeout;
    }
    qDebug("login result is %d",login->result);
    return login->result;
}

int DevConnection::Logout()
{
    return KE_Not_Support_Function;
}

int DevConnection::GetDeviceInfo(NET_DEVICEINFO *info)
{
    if(this->login == 0){
        return KE_No_Initial;
    }
    if(info)
        *info = this->login->devInfo;
    return KE_SUCCESS;
}

int DevConnection::TryConnect(const std::string &addr, int port)
{
    this->m_addr = addr;
    this->m_port = port;
    QString name = QString::fromStdString(addr);

    m_socketHandle->TryConnectToHost(name,port);

    return KE_SUCCESS;
}

int DevConnection::Request()
{
    QByteArray msgSend;
    int msgLen = sizeof(KEDevMsgHead);
    msgSend.resize(msgLen);
    KEDevMsgHead* pReqMsg = (KEDevMsgHead *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = DevMsg_HeartBeat;
    pReqMsg->msgLength = msgLen;
    pReqMsg->videoID = this->m_channelID;

    int ret = this->m_socketHandle->WriteData(msgSend);
    return ret;
}


