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

}

DevSimulateIPC::DevSimulateIPC(SocketHandler *s, ProtocalProcess *protocal, QObject *parent):
     Device(s,protocal,parent)
{
    m_chLogin = 0;
    m_chKey = 0;
    this->toHoldSocket = true;
}

int DevSimulateIPC::SendMediaData(int channelNo, const char *data, int len)
{

    QByteArray mediaData(data,len);
    QList<DevVideoSvr *> listCommand =  this->findChildren<DevVideoSvr *>();
    for(int i = 0; i < listCommand.size(); ++i) {
        listCommand[i]->sendMedia(channelNo,mediaData);
    }
    return 0;
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
        qWarning("login message response timeout");
        return KE_Msg_Timeout;
    }
    if(m_chLogin->result != RESP_ACK){
        return KE_Login_ERROR;
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

    if(ret == KE_SUCCESS){

        QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                     this,&DevSimulateIPC::GetMessageData,
                     Qt::DirectConnection);
    }
    return ret;
}

int DevSimulateIPC::DisConnect()
{
    return 0;
}

void DevSimulateIPC::setAllSubDeviceID()
{
    QList<Device *> listCommand =  this->findChildren<Device *>();
    for (int i = 0; i < listCommand.size(); ++i) {
        listCommand[i]->setChannelID(this->getChannelID());
    }
}

void DevSimulateIPC::GetMessageData(QByteArray &allBytes)
{
    this->m_protocal->ExtractMessage(allBytes,this);
}
