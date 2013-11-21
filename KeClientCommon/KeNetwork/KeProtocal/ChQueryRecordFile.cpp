#include "ChQueryRecordFile.h"
#include "KeMessage.h"
ChQueryRecordFile::ChQueryRecordFile(int channelID, SocketHandler * s ,int type,QObject *parent) :
    Channel(s,type,parent)
{
    this->m_channelID = channelID;
}

void ChQueryRecordFile::OnRespond(const char *buf, int len)
{
    unsigned char  msgType = buf[1];
    if(msgType == KEMSG_RecordFileList)
    {
        KEVideoServerResp *pMsg = (KEVideoServerResp *)buf;
        int cameraID = CreateCameraID(pMsg->videoID, pMsg->channelNo);
        if(cameraID != this->m_channelID){
            return;
        }
        qDebug("ChRealStart::OnRespond :receive KEMSG_TYPE_VIDEOSERVER");
        this->wakeup();
    }
}

int ChQueryRecordFile::Request()
{
        int devID = cameraID/256;
        int channelNo = cameraID%256;
        std::vector<BYTE> msgSend;
        int msgLen = sizeof(KERecordFileListReq);
        msgSend.resize(msgLen,0);
        KERecordFileListReq * pReqMsg;
        pReqMsg = (KERecordFileListReq *)&msgSend[0];
        pReqMsg->protocal = PROTOCOL_HEAD;
        pReqMsg->msgType = KEMSG_RecordFileList;
        pReqMsg->msgLength = msgLen;
        pReqMsg->clientID = m_clientID;
        pReqMsg->channelNo = channelNo;
        pReqMsg->videoID = devID;
        CTime st(startTime);
        CTime et(endTime);
        pReqMsg->startTime[0] = st.GetYear()%2000;
        pReqMsg->startTime[1] = st.GetMonth();
        pReqMsg->startTime[2] = st.GetDay();
        pReqMsg->startTime[3] = st.GetHour();
        pReqMsg->startTime[4] = st.GetMinute();
        pReqMsg->startTime[5] = st.GetSecond();
        pReqMsg->endTime[0] = et.GetYear()%2000;
        pReqMsg->endTime[1] = et.GetMonth();
        pReqMsg->endTime[2] = et.GetDay();
        pReqMsg->endTime[3] = et.GetHour();
        pReqMsg->endTime[4] = et.GetMinute();
        pReqMsg->endTime[5] = et.GetSecond();

        pReqMsg->fileType = fileType;
        pReqMsg->alarmNo = 0;

        this->recordFileList.clear();

        int ret = this->Write(&msgSend[0],msgLen);
        if (ret != msgLen)
        {
            return KE_SOCKET_WRITEERROR;
        }

        ret = this->WaitRecvMsg(KEMSG_RecordFileList);

        fileInfoList = this->recordFileList;

        //return ret;
        return KE_SUCCESS;
}


