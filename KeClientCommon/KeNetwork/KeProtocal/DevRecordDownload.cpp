#include "DevRecordDownload.h"
#include "KeMessage.h"
#include "SocketHandler.h"
#include "kenet_global.h"
DevRecordDownload::DevRecordDownload(Device *parent) :
    Device(parent)
{
    recordFile = 0;
    this->downloadedSize = 0;
    QObject::connect(this,&DevRecordDownload::toUpdateDownload,
                     this,&DevRecordDownload::UpdataDownload);
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
        this->downloadedSize = this->downloadTotalSize;
    }
    //qDebug("downloadsize %d,totalSize %d",this->downloadedSize,this->downloadTotalSize);
    if(this->cbDownloadPos)
        this->cbDownloadPos(this->getHandler(),this->downloadTotalSize,this->downloadedSize,this->userDownloadPos);
    emit this->downloadPos(this->downloadTotalSize,this->downloadedSize);
}

int DevRecordDownload::DownloadRecordFile(NET_RECORDFILE_INFO recordFile, const char *sSavedFileName)
{
    this->recordError = -1;
    this->setChannelID(recordFile.ch);
    this->recordFile = new QFile(sSavedFileName);
    if(!this->recordFile->open(QIODevice::WriteOnly)){
        qWarning("open save record file error");
        return KE_File_Open_Error;
    }
    this->recordFileInfo = recordFile;
    this->downloadedSize = 0;
    this->downloadTotalSize = recordFile.size*1000;
    int ret = this->Request();
    if(ret != KE_SUCCESS){
        return ret;
    }
    if(!this->waitRespond()){
        return KE_Msg_Timeout;
    }
    return recordError;
}

int DevRecordDownload::GetDownloadPos(int *nTotalSize, int *nDownLoadSize)
{
    *nTotalSize = this->downloadTotalSize;
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
    {
        if(respMsg->resp == RESP_ACK)
            this->recordError = KE_SUCCESS;
        else
            this->recordError = KE_NoRecord;
        this->wakeup();
    }
        break;
    case KEMSG_RecordPlayData:
    {
//        if(this->recordError == -1)//first receive data
//        {
//            this->recordError = KE_SUCCESS;
//            this->wakeup();
//        }
        if(respMsg->resp == RESP_END)//download end
        {
           recordFile->close();
           emit this->toUpdateDownload(-1);
        }
        else if(respMsg->resp == RESP_ACK)
        {
             int recordDataLen = respMsg->msgLength-sizeof(KEPlayRecordDataHead);
             recordFile->write(msgData.data()+sizeof(KEPlayRecordDataHead),recordDataLen);
             emit this->toUpdateDownload(recordDataLen);
        }

    }
        break;
    default:break;
    }
}

int DevRecordDownload::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }

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
    Device * devParent = this->getParentDev();
    if(devParent == 0)
        return false;
    return devParent->CheckChannelAvaliable<DevRecordDownload *>(channelID);

}
