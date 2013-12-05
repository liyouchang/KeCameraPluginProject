#include "ProtocalIPC.h"

#include "KeProtocal/KeMessage.h"
#include <QBuffer>
#include "DevSimulateIPC.h"

ProtocalIPC::ProtocalIPC(QObject *parent) :
    ProtocalProcess(parent)
{
}

void ProtocalIPC::ParseMessage(QByteArray &msgData, DevConnection *ch)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
        qDebug("KeDvrDevice::OnRespond msg KEMSG_TYPE_VIDEOSERVER! ");
    case KEMSG_TYPE_MEDIATRANS:
    case KEMSG_TYPE_VIDEOSTREAM:
    case KEMSG_TYPE_AUDIOSTREAM:
    {
        ch->ChildrenRespondMsg<DevSimulateIPC *>(msgData);
    }
        break;
    case KEMSG_RecordFileList:
    {
    }
        break;
    case  KEMSG_RecordPlayData:
        //RecvRecordPlayData(msgData);
        break;
    case  DevMsg_GetPTZParam:
        //RecvGetPTZParam(msgData);
        break;
    case  DevMsg_WifiCheck:
        //RecvDevWifiCheck(msgData);
        break;
    case  DevMsg_WifiStart:
        // RecvSetDevWifiResp(msgData);
        break;
    case DevMsg_HeartBeat:
        // RecvHeartBeat(msgData);
        break;
    case DevMsg_SETVIDEOPARAM:
        //RecvSetVideoParam(msgData);
        break;
    case DevMsg_GETVIDEOPARAM:
        //RecvGetVideoParam(msgData);
        break;
    default:
        break;
        //LOG_INFO("Receive unkown message: " <<pHead->msgType);

    }
}

void ProtocalIPC::ParseMessage(QByteArray &msgData, DevSimulateIPC *ch)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    {
//        qDebug("DevSimulateIPC::OnRespond msg KEMSG_TYPE_VIDEOSERVER! ");
//        KEVideoServerReq * pMsg = (KEVideoServerReq *)msgData.data();
//        if(pMsg->videoID != ch->getChannelID()){
//            return;
//        }
//        int mediaType = Media_None;
//        mediaType |= (pMsg->video==0) ? Media_Vedio: Media_None ;
//        mediaType |= (pMsg->listen==0) ? Media_Audio: Media_None ;
//        ch->DoRespond(pMsg->channelNo,mediaType);
//        ch->Request();
    }
        break;
    default:
        break;
    }
}

QByteArray ProtocalIPC::CreateMessage(ChSimulateVideo *ch)
{
    QByteArray msgSend;
    int msgLen = sizeof(KERTStreamHead);
    msgSend.resize(msgLen);
    msgSend.append(ch->mediaData);

    KEFrameHead * pFrame = (KEFrameHead *)ch->mediaData.data();
    ch->mediaFormat = pFrame->frameType & 0x7f;
    ch->currentFrameNo = pFrame->frameNo;
    KERTStreamHead * pReqMsg = (KERTStreamHead *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = ch->mediaFormat < 30 ? KEMSG_TYPE_VIDEOSTREAM:KEMSG_TYPE_AUDIOSTREAM;
    pReqMsg->msgLength = msgSend.size();
    pReqMsg->channelNo = ch->getChannelID()%256;
    pReqMsg->videoID = ch->getChannelID()/256;

    return msgSend;
}

QByteArray ProtocalIPC::CreateMessage(DevSimulateIPC *ch)
{
    QByteArray msgSend;
    int msgLen = sizeof(KEVideoServerResp);
    msgSend.resize(msgLen);
    KEVideoServerResp * pMsg = (KEVideoServerResp *)msgSend.data();
    pMsg->protocal = PROTOCOL_HEAD;
    pMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
    pMsg->msgLength = msgLen;
    pMsg->channelNo = ch->respChannelNo%256;
    pMsg->videoID = ch->getChannelID()/256;
    pMsg->respType = 0x0d;

    return msgSend;

}
