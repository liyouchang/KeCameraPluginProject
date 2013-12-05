#include "ChAskVideo.h"
#include "KeMessage.h"
#include "SocketHandler.h"
ChAskVideo::ChAskVideo(SocketHandler *s, Channel *parent) :
    Channel(s,parent)
{
    m_mediaType = Media_None;
}

void ChAskVideo::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    {
        KEVideoServerResp *pMsg = (KEVideoServerResp *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != this->getChannelID() || pMsg->clientID != this->getClientID()){
            return;
        }
        this->wakeup();
    }
        break;
    default:break;
    }
}

int ChAskVideo::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }

    QByteArray msgSend;
    int msgLen = sizeof(KEVideoServerReq);
    msgSend.resize(msgLen);
    KEVideoServerReq * pReqMsg;
    pReqMsg = (KEVideoServerReq *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
    pReqMsg->msgLength = msgLen;
    pReqMsg->clientID = this->getClientID();
    pReqMsg->channelNo = this->getChannelID()%256;
    pReqMsg->videoID = this->getChannelID()/256;
    pReqMsg->video = (this->m_mediaType & Media_Vedio) ? 0 : 1;
    pReqMsg->listen = (this->m_mediaType & Media_Listen) ? 0 : 1;
    pReqMsg->talk = (this->m_mediaType & Media_Talk) ? 0 : 1;
    pReqMsg->protocalType = 0;
    pReqMsg->transSvrIp = 0;

    return this->m_socketHandle->WriteData(msgSend);
}
