#include "DevRecordDownload.h"
#include "KeMessage.h"
#include "SocketHandler.h"
#include "kenet_global.h"
DevRecordDownload::DevRecordDownload(Device *parent) :
    Device(parent)
{
    recordFile = 0;
}

DevRecordDownload::~DevRecordDownload()
{
    delete recordFile;
}

void DevRecordDownload::UpdataDownload(int newSize)
{
    if(newSize >= 0){
        this->downloadedSize += newSize;
    }
    else{
        this->downloadedSize = this->recordFileInfo.size;
    }
    this->cbDownloadPos(this->getHandler(),this->recordFileInfo.size,this->downloadedSize,this->userDownloadPos);
    emit this->downloadPos(this->recordFileInfo.size,this->downloadedSize);
}

int DevRecordDownload::DownloadRecordFile(NET_RECORDFILE_INFO recordFile, const char *sSavedFileName)
{
    this->recordError = -1;
    this->setChannelID(recordFile.ch);
    this->recordFile = new QFile(sSavedFileName);
    if(!this->recordFile->open(QIODevice::WriteOnly)){
        return KE_File_Open_Error;
    }
    this->recordFileInfo = recordFile;
    this->Request();
    if(!this->waitRespond()){
        return KE_Msg_Timeout;
    }

    return recordError;
}

int DevRecordDownload::GetDownloadPos(int *nTotalSize, int *nDownLoadSize)
{
    *nTotalSize = this->recordFileInfo.size;
    *nDownLoadSize = this->downloadedSize;
    return KE_SUCCESS;
}

int DevRecordDownload::StopDownload()
{
    delete this;
    return KE_SUCCESS;
}

void DevRecordDownload::OnRespond(QByteArray &msgData)
{
    //unsigned char  msgType = msgData[1];
    KEChannelCommonResp * respMsg = (KEChannelCommonResp *)msgData.data();
    int cameraID = CreateCameraID(respMsg->videoID,respMsg->chanelNo);
    if(this->getChannelID() != cameraID)
        return;
    switch(respMsg->msgType)
    {
    case KEMSG_REQUEST_DOWNLOAD_FILE:
        this->recordError = KE_NoRecord;
        this->wakeup();
        break;
    case KEMSG_RecordPlayData:
    {
        if(this->recordError == -1)//first receive data
        {
            this->recordError = KE_SUCCESS;
            this->wakeup();
        }
        if(respMsg->resp == RESP_END)//download end
        {
            this->UpdataDownload(-1);
        }
        else if(respMsg->resp == RESP_ACK)
        {
             int recordDataLen = respMsg->msgLength-sizeof(KEPlayRecordDataHead);
             recordFile->write(msgData.data()+sizeof(KEPlayRecordDataHead),recordDataLen);
             this->UpdataDownload(recordDataLen);
        }

    }
        break;
    default:break;
    }
}

int DevRecordDownload::Request()
{
    QByteArray msgSend;
    int msgLen = sizeof(KEPlayRecordFileReq);
    msgSend.resize(msgLen);
    KEPlayRecordFileReq *pReqMsg = (KEPlayRecordFileReq *)msgSend.data();

    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = KEMSG_REQUEST_DOWNLOAD_FILE;
    pReqMsg->msgLength = msgLen;
    pReqMsg->clientID = this->getClientID();
    pReqMsg->channelNo = this->getChannelID()%256;
    pReqMsg->videoID = this->getChannelID()/256;
    pReqMsg->clientIp = 0;

    pReqMsg->protocalType = 1;

    pReqMsg->fileNo = recordFileInfo.fileNo;
    pReqMsg->fileType =0;

    pReqMsg->startTime[0] = recordFileInfo.starttime.year%2000;
    pReqMsg->startTime[1] = recordFileInfo.starttime.month;
    pReqMsg->startTime[2] = recordFileInfo.starttime.day;
    pReqMsg->startTime[3] = recordFileInfo.starttime.hour;
    pReqMsg->startTime[4] = recordFileInfo.starttime.minute;
    pReqMsg->startTime[5] = recordFileInfo.starttime.second;

    memcpy(pReqMsg->fileData,recordFileInfo.filename,80);


    return this->m_socketHandle->WriteData(msgSend);
}

bool DevRecordDownload::CheckAvaliable(int channelID)
{
    Device * devParent = qobject_cast<Device *>(this->parent());
    if(devParent == 0)
        return false;
    return devParent->CheckChannelAvaliable<DevRecordDownload *>(channelID);

}
