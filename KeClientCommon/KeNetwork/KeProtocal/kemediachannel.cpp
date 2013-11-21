#include "kemediachannel.h"
#include "KeMessage.h"

KeMediaChannel::KeMediaChannel(int cameraID,SocketHandler *s, QObject *parent) :
    Channel(s,0,parent)
{
    this->mCameraID = cameraID;
    this->mMediaType = Media_Vedio;
    this->setObjectName(this->createName());
}

void KeMediaChannel::OnRespond(const char *buf, int len)
{
    unsigned char  msgType = buf[1];
    //KEDevMsgHead * pHead = (KEDevMsgHead *)buf;
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    {
        KEVideoServerResp *pMsg = (KEVideoServerResp *)buf;
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != this->mCameraID){
            return;
        }
        qDebug("receive KEMSG_TYPE_VIDEOSERVER");
        this->waitCondition.wakeAll();
    }
        break;
    case KEMSG_TYPE_VIDEOSTREAM:
    case KEMSG_TYPE_AUDIOSTREAM:
    {
        KERTStreamHead * pMsg = (KERTStreamHead *)buf;
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != this->mCameraID){

            return;
        }
        int sendStartPos = 11;
        QByteArray data(buf+sendStartPos,pMsg->msgLength-sendStartPos);
        emit recvMediaData(mCameraID,pMsg->msgType,data);
    }
        break;
    }

}

int KeMediaChannel::Request()
{
    return ReqestVideoServer();
}

void KeMediaChannel::setMediaType(KeMediaChannel::MediaType type)
{
    this->mMediaType = type;
}



QString KeMediaChannel::createName()
{
    QString name = QString("KeMediaChannel_%1").arg(1);
    return name;
}


int KeMediaChannel::ReqestVideoServer()
{
    QVector<char> msgSend;
    int msgLen = sizeof(KEVideoServerReq);
    msgSend.resize(msgLen);
    PKEVideoServerReq pReqMsg;
    pReqMsg = (PKEVideoServerReq)&msgSend[0];
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
    pReqMsg->msgLength = msgLen;
    pReqMsg->clientID = 0;
    pReqMsg->channelNo = mCameraID%256;
    pReqMsg->videoID = mCameraID/256;
    pReqMsg->video =(mMediaType & Media_Vedio) ? 0 : 1;
    pReqMsg->listen = (mMediaType & Media_Listen) ? 0 : 1;
    pReqMsg->talk = (mMediaType & Media_Talk) ? 0 : 1;
    pReqMsg->protocalType = 0;
    pReqMsg->transSvrIp = 0;

    //int ret = m_socket->write(&msgSend[0],msgLen);
    //if (ret != msgLen)
    //{
    //    return 1;
    //}
    return 0;
}
