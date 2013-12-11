#include "DevUpdate.h"
#include "KeUpdateMessage.h"
#include "SocketHandler.h"
#include <QBuffer>
DevUpdate::DevUpdate(SocketHandler *s, ProtocalProcess *protocal, Device *parent) :
    DevConnectSvr(s,protocal,parent)
{
    this->onceHeartTime = 60000;
    heartTimeoutCount = 3;
    this->heartbeatTimer->start(onceHeartTime);
    oldPercent = 0;
    this->heartTime = QTime::currentTime();
    QObject::connect(this,&DevUpdate::toSendUpdate,
                     this,&DevUpdate::doSendUpdate);
}

void DevUpdate::StartDevUpdate(QByteArray allData)
{
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
        doRespond(msgType);
        emit DevUpdateStatus(this,pMsg->mac,pMsg->version,pMsg->puType);
    }
        break;
    case Update_HeartBeat:
    {
        KEUpdateReq * pMsg = (KEUpdateReq *)msgData.data();
        this->heartTime = QTime::currentTime();
        doRespond(msgType);
        if(!this->updateStart && !allData.isEmpty())
        {
            updateStart = true;
            this->currentPackage = 0;
            emit toSendUpdate();
        }
        emit DevUpdateStatus(this,pMsg->mac,pMsg->version,pMsg->puType);
    }
        break;
    case Update_Data:
    {
        KEUpdateDataResp * pMsg = (KEUpdateDataResp *)msgData.data();
        if(pMsg->resp == RESP_ACK){
            emit this->DevUpdateProcess(this,200);
        }
        else if(pMsg->resp == RESP_NAK){
            emit this->DevUpdateProcess(this,201);
        }
    }
        break;
    default:
        qDebug("Receive unkown message: %d ",msgType);
        break;
    }
}

void DevUpdate::HeartBeat()
{
    QTime current = QTime::currentTime();
    if(heartTime.msecsTo(current) > heartTimeoutCount * onceHeartTime){
        qWarning("heart beat loose %d times, connection end",heartTimeoutCount);
        this->DisConnect();
    }
//    ++heartCount;
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

void DevUpdate::doSendUpdate()
{
    //send end
    if(this->currentPackage == this->totalPackage){
        this->allData.clear();
        return;
    }
    QByteArray headData;
    int msgLen = sizeof(KEUpdateDataReq);
    headData.resize(msgLen);
    KEUpdateDataReq * pMsg =(KEUpdateDataReq *) headData.data();
    pMsg->protocal = PROTOCOL_HEAD;
    pMsg->msgType = Update_Data;
    pMsg->totalPack = this->totalPackage;
    pMsg->currentPack = this->currentPackage;

    QByteArray tmpByte ;
    if(this->currentPackage + 1 == this->totalPackage)
    {
        int dataPack = this->currentPackage;
        tmpByte  = this->allData.mid(dataPack*ONE_PACK_SIZE);
    }
    else
    {
        int dataPack = this->currentPackage;
        tmpByte  = this->allData.mid(dataPack*ONE_PACK_SIZE,ONE_PACK_SIZE);
    }
    pMsg->msgLength = headData.size() + tmpByte.size() - sizeof(KEUpdateMsgHead);
    QByteArray sendData = headData + tmpByte;
    this->m_socketHandle->WriteData(sendData);
    this->currentPackage += 1;
    QTimer::singleShot(20,this,SLOT(doSendUpdate()));

    int percent = this->currentPackage *100 /this->totalPackage;
    if(percent != this->oldPercent){
        emit this->DevUpdateProcess(this,percent);
        this->oldPercent = percent;
    }

}

int DevUpdate::Request()
{
    return 0;
}
