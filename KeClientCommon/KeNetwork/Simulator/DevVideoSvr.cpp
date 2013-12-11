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
        if(cameraID != this->getChannelID()%256)
            return;

        this->transIp = qToBigEndian<quint32>(pMsg->transSvrIp);
        int tmpMediaType = 0;
        tmpMediaType |= (pMsg->video==0) ? Media_Vedio: Media_None ;
        tmpMediaType |= (pMsg->listen==0) ? Media_Audio: Media_None ;
        this->mediaType = tmpMediaType;
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
        ChSimulateVideo * ch = this->GetChannel<ChSimulateVideo *>();
        if(ch != 0){
            ch->mediaType = this->mediaType;
        }
        else{
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
                qDebug("DevVideoSvr %d create ChSimulateVideo",this->getChannelID());
                ChSimulateVideo * ch = new ChSimulateVideo(socket,this);
                ch->mediaType = this->mediaType;
                QObject::connect(this,&DevVideoSvr::toSendMediaData,ch,&ChSimulateVideo::sendMedia);
            }
            else{

                ChSimulateVideo * ch = new ChSimulateVideo(this);
                QObject::connect(this,&DevVideoSvr::toSendMediaData,ch,&ChSimulateVideo::sendMedia);
            }
        }
    }
    else{
        qDebug("DevVideoSvr %d delete  ChSimulateVideo",this->getChannelID());
        ChSimulateVideo * ch = this->GetChannel<ChSimulateVideo *>();
        if(ch != 0){
            delete ch;
        }
    }
    this->respond = RESP_ACK;
    this->Request();
}

void DevVideoSvr::sendMedia(int channelNo, QByteArray &mediaData)
{
    int tmpNo = this->getChannelID()%256;
    if(tmpNo == channelNo && this->mediaType != Media_None){
        emit this->toSendMediaData(mediaData);
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
