#include "DevUpdate.h"
#include "KeUpdateMessage.h"
#include "SocketHandler.h"
#include <QBuffer>
DevUpdate::DevUpdate(SocketHandler *s, ProtocalProcess *protocal, Device *parent) :
    DevConnectSvr(s,protocal,parent)
{
    this->heartbeatTimer->start(60000);
    oldPercent = 0;
}

void DevUpdate::StartDevUpdate(QByteArray allData)
{
    //TODO:param thread share
    this->currentPackage = 0;
    this->totalPackage = (allData.size()+ONE_PACK_SIZE-1)/ONE_PACK_SIZE;
    this->allData = allData;
    this->updateStart = false;
}

void DevUpdate::DevDisconnect()
{
    qDebug("DevUpdate::DevDisconnect delete connection");
    emit DeleteDevice(this);
    this->deleteLater();
}

void DevUpdate::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case Update_Register:
    {
        KEUpdateReq * pMsg = (KEUpdateReq *)msgData.data();
        emit DevUpdateStatus(this,pMsg->mac,pMsg->version,pMsg->puType);
        doRespond(msgType);
    }
        break;
    case Update_HeartBeat:
    {
        KEUpdateReq * pMsg = (KEUpdateReq *)msgData.data();
        emit DevUpdateStatus(this,pMsg->mac,pMsg->version,pMsg->puType);
        doRespond(msgType);
        this->heartCount -- ;
        if(!this->updateStart && !allData.isEmpty())
        {
            updateStart = true;
            doSendUpdate(-1);
        }
    }
        break;
    case Update_Data:
    {
        KEUpdateDataResp * pMsg = (KEUpdateDataResp *)msgData.data();
        doSendUpdate(pMsg->currentPack);
    }
        break;
    default:
        qDebug("Receive unkown message: %d ",msgType);
        break;
    }
}

void DevUpdate::HeartBeat()
{
    if(heartCount > 2){
        qWarning("heart beat loose 3 times, connection end");
        this->DisConnect();
    }
    ++heartCount;
}

void DevUpdate::doRespond(int msgType)
{
    QByteArray sendData;
    int msgLen = sizeof(KEUpdateResp);
    sendData.resize(msgLen);
    KEUpdateResp * pMsg =(KEUpdateResp *) sendData.data();
    pMsg->protocal = PROTOCOL_HEAD;
    pMsg->msgLength =  msgLen - sizeof(KEUpdateMsgHead);
    pMsg->msgType =msgType;
    pMsg->resp = RESP_ACK;
    this->m_socketHandle->sWriteData(sendData);
}

void DevUpdate::doSendUpdate(int lastPack)
{
    this->currentPackage = lastPack+1;
    int percent = this->currentPackage *100 /this->totalPackage;
    if(percent != this->oldPercent){
        emit this->DevUpdateProcess(this,percent);
        this->oldPercent = percent;
    }
    if(this->currentPackage == this->totalPackage){
        this->allData.clear();
        return;
    }
    QByteArray sendData;
    int msgLen = sizeof(KEUpdateDataReq);
    sendData.resize(msgLen);
    KEUpdateDataReq * pMsg =(KEUpdateDataReq *) sendData.data();
    pMsg->protocal = PROTOCOL_HEAD;

    pMsg->msgType = Update_Data;
    pMsg->totalPack = this->totalPackage;
    pMsg->currentPack = this->currentPackage;

    QByteArray tmpByte ;
    if(this->currentPackage + 1 == this->totalPackage)
    {
        tmpByte  = this->allData.mid(this->currentPackage*ONE_PACK_SIZE);
    }
    else
    {
        tmpByte  = this->allData.mid(this->currentPackage*ONE_PACK_SIZE,ONE_PACK_SIZE);
    }

    pMsg->msgLength = sendData.size() + tmpByte.size() - sizeof(KEUpdateMsgHead);

    this->m_socketHandle->sWriteData(sendData);
    this->m_socketHandle->sWriteData(tmpByte);
}

int DevUpdate::Request()
{
    return 0;
}
