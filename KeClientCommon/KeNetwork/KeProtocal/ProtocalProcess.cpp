#include "ProtocalProcess.h"
#include "Device.h"
#include "KeMessage.h"
#include <QBuffer>
#include "DevRealPlay.h"
#include "ChPTZ.h"
#include "DevRecordDownload.h"
#include "DevAlarm.h"
ProtocalProcess::ProtocalProcess(QObject *parent) :
    QObject(parent)
{
    bufPos = 0;
    toRead = 0;
}

void ProtocalProcess::ExtractMessage(QByteArray &allBytes, Device *parser)
{
    int nRead= 0;
    QBuffer buffer(&allBytes);
    buffer.open(QIODevice::ReadOnly);
    const int headLen = 10;
    while(!buffer.atEnd())
    {
        if (msgRecv.isEmpty())//上一个消息已经读取完成
        {
            nRead = buffer.read(headBuf+bufPos,headLen-bufPos);
            if(nRead < headLen-bufPos)//消息头在最后几个字节，记录读取的字节，下次继续读取。
            {
                qDebug()<<"Continue Read head in new package\r\n ";
                bufPos = nRead;
                break;
            }
            unsigned char  protocal = headBuf[0];
            int msgLen = *((int*)&headBuf[2]);
            if (protocal != PROTOCOL_HEAD||  msgLen>msgMaxLen)
            {
                qWarning()<<"The message Protocal Head error, Clear the recv buffer!\r\n";
                msgRecv.clear();
                break;
            }
            msgRecv.resize(msgLen);
            bufPos = 0;
            memcpy(msgRecv.data(),headBuf,headLen);
            bufPos += headLen;
            toRead =  msgLen-headLen;
            if (toRead != 0)//防止 headLen 越界
            {
                nRead = buffer.read(msgRecv.data()+bufPos, toRead);
                bufPos += nRead;
                toRead -= nRead;
            }
        }
        else//上一个消息未完成读取
        {
            nRead = buffer.read(msgRecv.data()+bufPos,toRead);
            if (nRead < toRead){
                qDebug()<<"to read more and more!";
            }
            bufPos += nRead;
            toRead -= nRead;
        }
        if(toRead == 0 && bufPos == msgRecv.size())//全部读取
        {
            parser->OnRespond(msgRecv);
            msgRecv.clear();
            bufPos = 0;
        }
    }
}



QByteArray ProtocalProcess::CreateMessage(ChRealData *ch)
{

    QByteArray msgData;
    int headLen = sizeof(KERTStreamHead)+ sizeof(KEFrameHead);
    msgData.resize(headLen);
    KERTStreamHead * head = (KERTStreamHead *)msgData.data();
    head->protocal = PROTOCOL_HEAD;					//协议头
    head->msgType = KEMSG_TYPE_AUDIOSTREAM ;		//协议类型
    head->msgLength = headLen + ch->talkData.size();
    head->videoID = ch->getChannelID()/256;
    head->channelNo = ch->getChannelID()%256;

    KEFrameHead * frame = (KEFrameHead *)(msgData.data()+sizeof(KERTStreamHead));
    frame->second =*(int *)ch->timeSpan.data();
    frame->millisecond = ch->timeSpan.at(5);
    frame->frameLen = ch->talkData.size();

    msgData.append(ch->talkData);
    //协议长度
    return msgData;
}


QByteArray ProtocalProcess::CreateMessage(DevRealPlay *ch)
{
    QByteArray msgSend;
    int msgLen = sizeof(KEVideoServerReq);
    msgSend.resize(msgLen);
    KEVideoServerReq * pReqMsg;
    pReqMsg = (KEVideoServerReq *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = KEMSG_TYPE_VIDEOSERVER;
    pReqMsg->msgLength = msgLen;
    pReqMsg->clientID = 0;
    pReqMsg->channelNo = ch->getChannelID()%256;
    pReqMsg->videoID = ch->getChannelID()/256;
    pReqMsg->video =(ch->m_mediaType & Media_Vedio) ? 0 : 1;
    pReqMsg->listen = (ch->m_mediaType & Media_Listen) ? 0 : 1;
    pReqMsg->talk = (ch->m_mediaType & Media_Talk) ? 0 : 1;
    pReqMsg->protocalType = 0;
    pReqMsg->transSvrIp = 0;

    return msgSend;
}

void ProtocalProcess::ParseMessage(QByteArray & msgData,DevConnection *ch)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    case KEMSG_TYPE_MEDIATRANS:
    case KEMSG_TYPE_VIDEOSTREAM:
    case KEMSG_TYPE_AUDIOSTREAM:
        ch->ChildrenRespondMsg<DevRealPlay *>(msgData);
        break;
    case KEMSG_RecordFileList:
        ch->ChildrenRespondMsg<DevRecordQuery *>(msgData);
        break;
    case KEMSG_REQUEST_DOWNLOAD_FILE:
    case KEMSG_RecordPlayData:
        ch->ChildrenRespondMsg<DevRecordDownload *>(msgData);
        break;
    case KEDevMsg_AlarmVideo:
    case KEDevMsg_AlarmSenser:
        ch->ChildrenRespondMsg<DevAlarm *>(msgData);
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

void ProtocalProcess::ParseMessage(QByteArray &msgData, DevRealPlay *ch)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSERVER:
    {
        //qDebug("DevRealPlay::OnRespond msg KEMSG_TYPE_VIDEOSERVER! ");
        KEVideoServerResp *pMsg = (KEVideoServerResp *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != ch->getChannelID() || pMsg->clientID != ch->getClientID()){
            return;
        }
        ch->wakeup();
    }
        break;
    case KEMSG_TYPE_MEDIATRANS:
        break;
    case KEMSG_TYPE_VIDEOSTREAM:
    case KEMSG_TYPE_AUDIOSTREAM:
    {
        if(ch->chRealData)
            ch->chRealData->OnRespond(msgData);
    }
        break;
    case DevMsg_SerialData:
        ch->ChildrenRespondMsg<ChPTZ *>(msgData);
        break;
    default:break;
    }
}

void ProtocalProcess::ParseMessage(QByteArray &msgData, ChRealData *ch)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEMSG_TYPE_VIDEOSTREAM:
    case KEMSG_TYPE_AUDIOSTREAM:
    {
        KERTStreamHead * pMsg = (KERTStreamHead *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != ch->getChannelID()){
            return;
        }
        int sendStartPos = 11;
        QByteArray data = msgData.right(pMsg->msgLength-sendStartPos);
        ch->DoRespond(pMsg->msgType,data);
    }
        break;
    }

}


void ProtocalProcess::ParseMessage(QByteArray &msgData, DevRecordQuery *ch)
{
    unsigned char  msgType = msgData[1];
    if(msgType == KEMSG_RecordFileList)
    {
        KERecordFileListResp * pMsg = (KERecordFileListResp *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != ch->getChannelID()){
            return;
        }
        if (pMsg->msgLength == 16)//如果只收到消息头，表示文件内容结束
        {
            qDebug("no record find!");
            ch->findEnd = true;
            ch->wakeup();
            return;
        }

        int fileInfoNum = (pMsg->msgLength-sizeof(KERecordFileListResp))/100;
        KERecordFileInfo * pRecordFileInfo =
                (KERecordFileInfo *) (msgData.data()+sizeof(KERecordFileListResp));
        for (int i=0;i<fileInfoNum;i++,pRecordFileInfo++)
        {
            NET_RECORDFILE_INFO fileInfo;
            fileInfo.ch = cameraID;
            fileInfo.nRecordFileType = ch->m_recordFileType;
            fileInfo.fileNo = pRecordFileInfo->fileNo;
            fileInfo.size = pRecordFileInfo->fileSize;

            fileInfo.starttime.year = pRecordFileInfo->startTime[0]+2000;
            fileInfo.starttime.month = pRecordFileInfo->startTime[1];
            fileInfo.starttime.day = pRecordFileInfo->startTime[2];
            fileInfo.starttime.hour = pRecordFileInfo->startTime[3];
            fileInfo.starttime.minute = pRecordFileInfo->startTime[4];
            fileInfo.starttime.second = pRecordFileInfo->startTime[5];

            fileInfo.endtime.year = pRecordFileInfo->endTime[0]+2000;
            fileInfo.endtime.month = pRecordFileInfo->endTime[1];
            fileInfo.endtime.day = pRecordFileInfo->endTime[2];
            fileInfo.endtime.hour = pRecordFileInfo->endTime[3];
            fileInfo.endtime.minute = pRecordFileInfo->endTime[4];
            fileInfo.endtime.second = pRecordFileInfo->endTime[5];

            memcpy(fileInfo.filename,pRecordFileInfo->data,80);
            ch->recordFileList.push_back(fileInfo);
        }
        if (pMsg->resp == 6)//如果收到应答为6表示文件信息输送结束
        {
            ch->wakeup();
        }
    }
}

