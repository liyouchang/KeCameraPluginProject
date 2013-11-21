#include "ChRealStart.h"
#include "KeMessage.h"
#include "kenet_global.h"
#include "SocketHandler.h"
#include "ProtocalProcess.h"
ChRealStart::ChRealStart(int channelID, Channel *parent) :
    Channel(parent)
{
    this->setChannelID(channelID);
}

int ChRealStart::ReqestVideoServer()
{
    QByteArray msgSend;
    int msgLen = sizeof(KEVideoServerReq);
    msgSend.resize(msgLen);
    PKEVideoServerReq pReqMsg;
    pReqMsg = (PKEVideoServerReq)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
    pReqMsg->msgLength = msgLen;
    pReqMsg->clientID = 0;
    pReqMsg->channelNo = m_channelID%256;
    pReqMsg->videoID = m_channelID/256;
    pReqMsg->video =(m_mediaType & Media_Vedio) ? 0 : 1;
    pReqMsg->listen = (m_mediaType & Media_Listen) ? 0 : 1;
    pReqMsg->talk = (m_mediaType & Media_Talk) ? 0 : 1;
    pReqMsg->protocalType = 0;
    pReqMsg->transSvrIp = 0;

    return m_socketHandle->WriteData(msgSend);

}
void ChRealStart::OnRespond(QByteArray &data)
{
    this->m_protocal->ParseMessage(data,this);
}

int ChRealStart::Request()
{
    return ReqestVideoServer();
}
