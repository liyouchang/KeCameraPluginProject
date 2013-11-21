#include "DevVideoSvr.h"
#include "KeProtocal/KeMessage.h"
#include "ChSimulateVideo.h"
#include <QTimer>
#include "SocketHandler.h"
#include <QHostAddress>
#include <QtEndian>
DevVideoSvr::DevVideoSvr(Device *parent) :
    Device(parent)
{
    this->m_number = 1;
    QObject::connect(this,&DevVideoSvr::toDoRespond,this,&DevVideoSvr::DoRespond);
    this->mediaType = Media_None;
}

void DevVideoSvr::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    {
        qDebug("DevSimulateIPC::OnRespond msg KEMSG_TYPE_VIDEOSERVER!");
        KEVideoServerReq * pMsg = (KEVideoServerReq *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID,pMsg->channelNo);
        if(cameraID != this->getChannelID())
            return;

        this->transIp = qToBigEndian<quint32>(pMsg->transSvrIp);
        this->mediaType |= (pMsg->video==0) ? Media_Vedio: Media_None ;
        this->mediaType |= (pMsg->listen==0) ? Media_Audio: Media_None ;
        emit this->toDoRespond();
    }
        break;
    default:break;
    }
}


void DevVideoSvr::DoRespond()
{
    if(mediaType != Media_None)//create new video channel
    {
        if(transIp != 0)
        {
            SocketHandler * socket = new SocketHandler();
            socket->CreateSocket();
            QHostAddress addr(transIp);
            socket->ConnectToHost(addr.toString(),22615);
            if(socket->lastError != 0){
                qWarning()<<addr.toString()<<" connect error!";
                this->respond = RESP_NAK;
                this->Request();
                return;
            }
            ChSimulateVideo * ch = new ChSimulateVideo(socket,this);
        }
        else{
            ChSimulateVideo * ch = new ChSimulateVideo(this);
        }
    }
    this->Request();
}

void DevVideoSvr::sendMedia(int channelNo, QByteArray &mediaData)
{
    int tmpNo = this->getChannelID()%256;
    if(tmpNo == channelNo && this->mediaType != Media_None){
        QList<ChSimulateVideo *> listCommand =  this->findChildren<ChSimulateVideo *>();
        for(int i = 0; i < listCommand.size(); ++i) {
            listCommand[i]->mediaData = mediaData;
            listCommand[i]->Request();
        }
    }
}

int DevVideoSvr::Request()
{
    QByteArray msgSend;
    int msgLen = sizeof(KEVideoServerResp);
    msgSend.resize(msgLen);
    KEVideoServerResp * pMsg = (KEVideoServerResp *)msgSend.data();
    pMsg->protocal = PROTOCOL_HEAD;
    pMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
    pMsg->msgLength = msgLen;
    pMsg->channelNo = this->getChannelID()%256;
    pMsg->videoID = this->getChannelID()/256;
    pMsg->respType = this->respond;

    return this->m_socketHandle->WriteData(msgSend);
}

void DevVideoSvr::setChannelID(int id)
{
    int cameraID = CreateCameraID(id,this->m_number);
    this->m_channelID = cameraID;
}
