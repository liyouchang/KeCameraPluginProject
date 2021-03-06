﻿#include "DevRecordQuery.h"
#include "KeMessage.h"
#include "SocketHandler.h"
#include "kenet_global.h"
#include "ProtocalProcess.h"
DevRecordQuery::DevRecordQuery(Device *parent) :
    Device(parent)
{
    m_queryTimeout = 3000;
    m_recordFileType = 0;
}


bool operator >= (const NET_TIME &a,const NET_TIME& b )
{
    if(a.year > b.year)
        return true;
    if(a.year < b.year)
        return false;
    if(a.month > b.month)
        return true;
    if(a.month < b.month)
        return false;
    if(a.day > b.day)
        return true;
    if(a.day < b.day)
        return false;
    if(a.hour > b.hour)
        return true;
    if(a.hour < b.hour)
        return false;
    if(a.minute > b.minute)
        return true;
    if(a.minute < b.minute)
        return false;
    if(a.second > b.second)
        return true;
    if(a.second < b.second)
        return false;
//    if(a.millisecond > b.millisecond)
//        return true;
//    if(a.millisecond < b.millisecond)
//        return false;

    return true;
}



void DevRecordQuery::SetTimeout(int time)
{
    this->m_queryTimeout = time;
}

void DevRecordQuery::SetQueryTime(NET_TIME tmStart, NET_TIME tmEnd)
{
    this->m_startTime = tmStart;
    this->m_endTime = tmEnd;
}

void DevRecordQuery::SetQueryType(int recordFileType)
{
    this->m_recordFileType = recordFileType;
}

int DevRecordQuery::QueryRecordFile(int channelID, std::vector<NET_RECORDFILE_INFO> &vFileInfo)
{
    this->setChannelID(channelID);
    //this->recordFileList.clear();
    this->findEnd = false;
    do{
        qDebug("query record start time %d:%d:%d; end time %d:%d:%d",
               this->m_startTime.hour,this->m_startTime.minute,this->m_startTime.second,
               this->m_endTime.hour,this->m_endTime.minute,this->m_endTime.second);
        this->recordFileList.clear();
        this->Request();
        if(!this->waitRespond(m_queryTimeout)){
            return KE_Msg_Timeout;
        }
        if(this->recordFileList.empty()){
            qDebug("find empty");
            break;
        }

        if(recordFileList.size() == 1 && !vFileInfo.empty()){
            NET_RECORDFILE_INFO allLast = vFileInfo.back();
            NET_RECORDFILE_INFO oneLast = this->recordFileList.back();
            if(allLast.fileNo != oneLast.fileNo){
                vFileInfo.push_back(oneLast);
            }
        }
        else{
            for(int i = 0;i<recordFileList.size();i++){
                vFileInfo.push_back(this->recordFileList[i]);
            }
        }


        NET_RECORDFILE_INFO lastRecord = this->recordFileList.back();
        if(lastRecord.endtime >= this->m_endTime ||
                this->m_startTime >= lastRecord.endtime)
        {
            qDebug("find end");
            this->findEnd = true;
        }
        else{
            this->m_startTime = lastRecord.endtime;
        }
    }while(!this->findEnd);
 //   vFileInfo = this->recordFileList;
    return 0;
}

int DevRecordQuery::QuickQueryRecordFile(int channelID, std::vector<NET_RECORDFILE_INFO> &vFileInfo)
{
    this->setChannelID(channelID);
    this->findEnd = false;
    do{
        this->recordFileList.clear();
        this->Request();
        if(!this->waitRespond(m_queryTimeout)){
            return KE_Msg_Timeout;
        }
        if(recordFileList.empty()){
            break;
        }
        if(recordFileList.size() == 1 && !vFileInfo.empty()){
            NET_RECORDFILE_INFO allLast = vFileInfo.back();
            NET_RECORDFILE_INFO oneLast = this->recordFileList.back();
            if(allLast.fileNo != oneLast.fileNo){
                vFileInfo.push_back(oneLast);
            }
        }
        else{
            for(int i = 0;i<recordFileList.size();i++){
                vFileInfo.push_back(this->recordFileList[i]);
            }
        }

        NET_RECORDFILE_INFO lastRecord = this->recordFileList.back();

        if(vFileInfo.size() > 16 ||
                lastRecord.endtime >= this->m_endTime||
                this->m_startTime >= lastRecord.endtime)
        {
            this->findEnd = true;
        }
        else
        {
            this->m_startTime = lastRecord.endtime;
        }
    }while(!this->findEnd);
    //vFileInfo = this->recordFileList;
    return 0;
}
void DevRecordQuery::OnRespond(QByteArray &data)
{
    this->m_protocal->ParseMessage(data,this);
}

int DevRecordQuery::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }

    QByteArray msgSend;
    int msgLen = sizeof(KERecordFileListReq);
    msgSend.resize(msgLen);
    KERecordFileListReq * pReqMsg;
    pReqMsg = (KERecordFileListReq *)msgSend.data();
    pReqMsg->protocal = PROTOCOL_HEAD;
    pReqMsg->msgType = KEMSG_RecordFileList;
    pReqMsg->msgLength = msgLen;
    pReqMsg->clientID = 0;
    pReqMsg->channelNo = m_channelID%256;
    pReqMsg->videoID = m_channelID/256;
    pReqMsg->startTime[0] = m_startTime.year%2000;
    pReqMsg->startTime[1] = m_startTime.month;
    pReqMsg->startTime[2] = m_startTime.day;
    pReqMsg->startTime[3] = m_startTime.hour;
    pReqMsg->startTime[4] = m_startTime.minute;
    pReqMsg->startTime[5] = m_startTime.second;
    pReqMsg->endTime[0] = m_endTime.year % 2000;
    pReqMsg->endTime[1] = m_endTime.month;
    pReqMsg->endTime[2] = m_endTime.day;
    pReqMsg->endTime[3] = m_endTime.hour;
    pReqMsg->endTime[4] = m_endTime.minute;
    pReqMsg->endTime[5] = m_endTime.second;

    pReqMsg->fileType = m_recordFileType;//1计划录像；2表示传感器报警；	3移动侦测报警；4抓拍照片；
    pReqMsg->alarmNo = 0;

    //this->recordFileList.clear();
    int ret = this->m_socketHandle->WriteData(msgSend);
    return ret;
}

int DevRecordQuery::FindStart(int channelID)
{
    this->setChannelID(channelID);
    this->recordFileList.clear();
    recordPos = 0;
    this->findEnd = false;
    //    this->Request();
    //    if(!this->waitRespond(m_queryTimeout)){
    //        return KE_Msg_Timeout;
    //    }
    return 0;
}

int DevRecordQuery::FindNext(NET_RECORDFILE_INFO *lpFindData)
{
    if(this->findEnd ){
        qDebug("record find end,record find end !");
        return KE_Record_Find_End;
    }
    //check if we have queryed record list, if no record left, query record again
    if(this->recordPos >= this->recordFileList.size())
    {
        //record end when start time >= end time
        if(this->m_startTime >= this->m_endTime){
            qDebug("record find end,starttime>=endtime !");
            return KE_Record_Find_End;
        }
        this->recordFileList.clear();
        recordPos = 0;
        this->Request();
        if(!this->waitRespond(m_queryTimeout)){
            qWarning("wait query record respond error!");
            return KE_Msg_Timeout;
        }
        if(this->recordFileList.empty()){
            qDebug("record find end,record file empty !");
            findEnd = true;
            return KE_Record_Find_End;
        }
        NET_RECORDFILE_INFO lastRecord = this->recordFileList.back();
        this->m_startTime =lastRecord.endtime;
    }
    *lpFindData = this->recordFileList[this->recordPos++];

    return KE_SUCCESS;
}

int DevRecordQuery::FindClose()
{
    delete this;
    return KE_SUCCESS;
}

bool DevRecordQuery::CheckAvaliable(int channelID)
{
    Device * devParent = this->getParentDev();
    if(devParent == 0)
        return false;
    return devParent->CheckChannelAvaliable<DevRecordQuery *>(channelID);
}
