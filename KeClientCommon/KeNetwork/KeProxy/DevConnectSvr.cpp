#include "DevConnectSvr.h"
#include "TcpDeviceHolder.h"
#include "ProtocalKaerProxy.h"
#include "KeProtocal/KeMessage.h"
#include "KeProtocal/DevRecordDownload.h"
#include "KeProtocal/DevAlarm.h"
#include "KeProtocal/ChPTZ.h"
#include "KeProtocal/DevRealPlay.h"
DevConnectSvr::DevConnectSvr(SocketHandler *s, ProtocalProcess *protocal, Device *parent)
    :DevConnection(s,protocal,parent)
{
    chLogin = new ChReceiveLogin(this);
    chLogin->setChannelID(this->getHandler());
    TcpDeviceHolder * holder = dynamic_cast<TcpDeviceHolder *>(parent);
    if(holder != 0){
        QObject::connect(chLogin,&ChReceiveLogin::getDeviceLogin,holder,&TcpDeviceHolder::havaNewLogin);
    }
    this->heartbeatTimer->start(3000);
}

int DevConnectSvr::LoginServer(std::string username, std::string pwd)
{
    return KE_Not_Support_Function;
}

int DevConnectSvr::Logout()
{
    return KE_Not_Support_Function;
}

void DevConnectSvr::OnRespond(QByteArray &msgData)
{
    //this->m_protocal->ParseMessage(msgData,this);
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    case KEMSG_TYPE_MEDIATRANS:
    case KEMSG_TYPE_VIDEOSTREAM:
    case KEMSG_TYPE_AUDIOSTREAM:
        this->ChildrenRespondMsg<DevRealPlay *>(msgData);
        break;
    case KEMSG_RecordFileList:
        this->ChildrenRespondMsg<DevRecordQuery *>(msgData);
        break;
    case KEMSG_REQUEST_DOWNLOAD_FILE:
    case KEMSG_RecordPlayData:
        this->ChildrenRespondMsg<DevRecordDownload *>(msgData);
        break;
    case KEDevMsg_AlarmVideo:
    case KEDevMsg_AlarmSenser:
        qDebug("receive alarm message!!");
        this->ChildrenRespondMsg<DevAlarm *>(msgData);
        break;
    case  DevMsg_GetPTZParam:
        break;
    case  DevMsg_WifiCheck:
        break;
    case  DevMsg_WifiStart:
        break;
    case DevMsg_HeartBeat:
        --this->heartCount;
        break;
    case DevMsg_SETVIDEOPARAM:
        break;
    case DevMsg_GETVIDEOPARAM:
        break;
    case DevMsg_SerialData:
        this->ChildrenRespondMsg<ChPTZ *>(msgData);
        break;
    case Client_GetKeyDevice:
        this->ChildrenRespondMsg<ChSecretKey *>(msgData);
        break;
    case Client_LoginDevice:
        this->ChildrenRespondMsg<ChLogin *>(msgData);
        break;
    case Device_LoginServer:
    case Device_GetKeyServer:
        this->ChildrenRespondMsg<ChReceiveLogin *>(msgData);
        break;
    default:
        qDebug("Receive unkown message: %d ",msgType);
        break;
    }
}

void DevConnectSvr::DevDisconnect()
{
    qDebug("DevConnectSvr::DevDisconnect delete connection");
    delete this;
}

void DevConnectSvr::HeartBeat()
{
//    if(heartCount > 2){
//        qWarning("heart beat loose 3 times, connection end");
//        this->DisConnect();
//    }
    ++heartCount;
    //qDebug("heart beat timer %d",heartCount);
}

int DevConnectSvr::Request()
{
    return 0;
}
