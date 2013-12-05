#include "ChSearchDevice.h"

#include "ProtocalProcess.h"
#include "SocketHandler.h"
#include "KeMessage.h"
#include <QHostAddress>
#include <QtEndian>
ChSearchDevice::ChSearchDevice(SocketHandler *s, Channel *parent):
    Channel(s,parent)
{
    devIndex = 0;
    this->toHoldSocket = true;
    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                     this,&ChSearchDevice::GetMessageData,
                     Qt::DirectConnection);

}

ChSearchDevice::ChSearchDevice(SocketHandler *s, ProtocalProcess *protocal, Channel *parent):
    Channel(s,protocal,parent)
{
    devIndex = 0;
    this->toHoldSocket = true;

    QObject::connect(m_socketHandle,&SocketHandler::sdReadedData,
                     this,&ChSearchDevice::GetMessageData,
                     Qt::DirectConnection);
}



void ChSearchDevice::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEDevMsg_SearchOnlineDevice:
    {
        KESearchOnlineDeviceResp * pMsg = (KESearchOnlineDeviceResp *)msgData.data();
        CopyIP(this->devInfo.szIp,pMsg->devIp);
        CopyIP(this->devInfo.szMask,pMsg->devMask);
        CopyIP(this->devInfo.szGateway,pMsg->devGateWay);
        strcpy(this->devInfo.szUsrName,pMsg->userName);
        strcpy(this->devInfo.szUsrPwd,pMsg->userPwd);
        this->devInfo.channelCount = pMsg->channelCount;
        //this->devInfo.iTerminalType = pMsg->devType;
        strcpy(this->devInfo.deviceSN,pMsg->devSN);

        this->devInfo.hardwareVer = pMsg->hardVer;
        strcpy(this->devInfo.softwareVer,pMsg->softVer);

        this->devInfo.iPort = pMsg->port;
        QByteArray recvInfo((char *)&this->devInfo,sizeof(NET_DEVICEINFO));
        emit this->recvDeviceInfo(this->devIndex++,recvInfo);
    }
        break;
    default:
        qWarning("ChSearchDevice Receive unkown message: %d" ,msgType);
        break;
    }
}

void ChSearchDevice::GetMessageData(QByteArray &allBytes)
{
    this->m_protocal->ExtractMessage(allBytes,this);
}

int ChSearchDevice::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }
    //计数归零
    this->devIndex = 0;
    //发送数据
    QByteArray sendData;
    int msgLen = sizeof(KESearchOnlineDeviceReq);
    sendData.resize(msgLen);
    KESearchOnlineDeviceReq * pMsg = (KESearchOnlineDeviceReq *)sendData.data();

    pMsg->protocal = PROTOCOL_HEAD;
    pMsg->msgType = KEDevMsg_SearchOnlineDevice;
    pMsg->msgLength = msgLen;
    memcpy(pMsg->kaer,"KAER970326",10);

    return this->m_socketHandle->WriteData(sendData);

}
