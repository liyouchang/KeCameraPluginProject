#include "DevSimulateIPC.h"
#include "KeProtocal/ProtocalProcess.h"
#include "SocketHandler.h"
#include "KeProtocal/KeMessage.h"
#include "ProtocalIPC.h"
#include "ChSimulateVideo.h"
#include "ChDeviceKey.h"
#include "kenet_global.h"
#include "DevConnection.h"
#include "DevVideoSvr.h"
DevSimulateIPC::DevSimulateIPC(SocketHandler *s, Device *parent):
    Device(s,parent)
{
    m_chLogin = 0;
    m_chKey = 0;
    this->toHoldSocket = true;
    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                 this,&DevSimulateIPC::GetMessageData,
                 Qt::DirectConnection);

    QObject::connect(m_socketHandle,&SocketHandler::saDisConnected,
                     this,&DevSimulateIPC::DevDisconnect);
    QObject::connect(m_socketHandle,&SocketHandler::saConnected,
                     this,&DevSimulateIPC::DevConnected);
}

DevSimulateIPC::DevSimulateIPC(SocketHandler *s, ProtocalProcess *protocal,Device * parent):
     Device(s,protocal,parent)
{
    m_chLogin = 0;
    m_chKey = 0;
    this->toHoldSocket = true;
    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                 this,&DevSimulateIPC::GetMessageData,
                 Qt::DirectConnection);

    QObject::connect(m_socketHandle,&SocketHandler::saDisConnected,
                     this,&DevSimulateIPC::DevDisconnect);
    QObject::connect(m_socketHandle,&SocketHandler::saConnected,
                     this,&DevSimulateIPC::DevConnected);

}

DevSimulateIPC::~DevSimulateIPC()
{
    delete this->m_protocal;
}

void DevSimulateIPC::SendMediaData(int channelNo, const char *data, int len)
{
    QByteArray mediaData(data,len);
    //emit sendMedia(channelNo,mediaData);
    QList<DevVideoSvr *> listCommand =  this->GetChannelList<DevVideoSvr *>();
    for(int i = 0; i < listCommand.size(); ++i) {
        listCommand[i]->sendMedia(channelNo,mediaData);
    }
}

void DevSimulateIPC::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    qDebug("DevSimulateIPC::OnRespond msg type %02X",msgType);
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
        this->ChildrenRespondMsg<DevVideoSvr *>(msgData);
        break;
    case Device_GetKeyServer:
        this->ChildrenRespondMsg<ChDeviceKey *>(msgData);
        break;
    case Device_LoginServer:
        this->ChildrenRespondMsg<ChDeviceLogin *>(msgData);
        break;
    default:
        qWarning("receive unsurported message");
        break;
    }
}

int DevSimulateIPC::Request()
{
    ProtocalIPC * protocal = qobject_cast<ProtocalIPC *>(m_protocal);
    QByteArray send = protocal->CreateMessage(this);
    return this->m_socketHandle->WriteData(send);
}
int DevSimulateIPC::LoginServer(std::string username, std::string pwd)
{
    if(m_chKey == 0){
        m_chKey = new ChDeviceKey(this);
    }
    m_chKey->Request();
    if(!m_chKey->waitRespond()){
        return KE_Msg_Timeout;
    }
    //set PUID
    this->m_channelID = m_chKey->getChannelID();
    if(m_chLogin == 0){
        m_chLogin = new ChDeviceLogin(this);
    }
    QString loginName = QString::fromLatin1(username.c_str());
    QString loginPwd = QString::fromLatin1(pwd.c_str());
    m_chLogin->initialize(loginName,loginPwd,m_chKey->m_skey);
    m_chLogin->setChannelID(this->getChannelID());
    m_chLogin->Request();
    if(!m_chLogin->waitRespond()){
        qWarning()<<"login message response timeout,mac:"<<m_mac;
        return KE_Msg_Timeout;
    }
    if(m_chLogin->result != RESP_ACK){
        return KE_Login_Error;
    }
    //set sub device id
    setAllSubDeviceID();

    return KE_SUCCESS;

}

int DevSimulateIPC::Logout()
{
    return 0;
}

int DevSimulateIPC::SetMac(std::string mac)
{
    this->m_mac = QString::fromLatin1(mac.c_str());

    if(m_chKey == 0){
        m_chKey = new ChDeviceKey(this);
    }
    strcpy(m_chKey->mac,mac.c_str());
    return 0;
}

int DevSimulateIPC::ConnectDevice(const std::string &addr, int port)
{
    QString name = QString::fromStdString(addr);
    int ret =   m_socketHandle->ConnectToHost(name,port);
    return ret;
}

int DevSimulateIPC::DisConnect()
{
    //toReconnect = false;
    return this->m_socketHandle->DisConnectFromHost();
}

int DevSimulateIPC::GetDeviceInfo(NET_DEVICEINFO *info)
{
    return KE_Not_Support_Function;
}

void DevSimulateIPC::setAllSubDeviceID()
{
    QList<Device *> listCommand =  this->GetChannelList<Device *>();
    for (int i = 0; i < listCommand.size(); ++i) {
        listCommand[i]->setChannelID(this->getChannelID());
    }
}

void DevSimulateIPC::GetMessageData(QByteArray &allBytes)
{
    this->m_protocal->ExtractMessage(allBytes,this);
}

void DevSimulateIPC::DevDisconnect()
{
    qDebug("DevSimulateIPC::DevDisconnect delete connection");
    delete this;
}

void DevSimulateIPC::DevConnected()
{
    qDebug("DevSimulateIPC::DevConnected connect");

}
