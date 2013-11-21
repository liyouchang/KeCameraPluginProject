#include "DevAlarm.h"
#include "KeMessage.h"
#include "SocketHandler.h"
DevAlarm::DevAlarm(Device *parent) :
    Device(parent)
{
    QObject::connect(this,&DevAlarm::toDoRespond,this,&DevAlarm::DoRespond);
}

void DevAlarm::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case KEDevMsg_AlarmSenser:
    {
        KEAlarmSenserReq * pMsg = (KEAlarmSenserReq *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID,pMsg->channelNo);
        if(this->getChannelID() != pMsg->videoID){
            qDebug("DevAlarm::OnRespond pu id error");
            return;
        }
        this->alarmType =Alarm_Sensor;
        this->alarmInfo.channelID = cameraID;
        this->alarmInfo.state = pMsg->state;
        this->alarmInfo.second = pMsg->second;
        this->alarmInfo.msecond = pMsg->msecond;
        this->alarmInfo.number = pMsg->number;

        emit toDoRespond();

    }
        break;
    case KEDevMsg_AlarmVideo:
    {
        KEAlarmVideoReq * pMsg = (KEAlarmVideoReq *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID,pMsg->channelNo);
        if(this->getChannelID() != pMsg->videoID){
            qDebug("DevAlarm::OnRespond pu id error");
            return;
        }
        this->alarmType =pMsg->type;
        this->alarmInfo.channelID = cameraID;
        this->alarmInfo.state = pMsg->state;
        this->alarmInfo.second = pMsg->second;
        this->alarmInfo.msecond = pMsg->msecond;
        this->alarmInfo.number = pMsg->number;
        memcpy(this->alarmInfo.coordinate,pMsg->coordinate,9);

        emit toDoRespond();

    }
        break;
    default:break;
    }
}

void DevAlarm::DoRespond()
{
    this->Request();
}

int DevAlarm::Request()
{
    QByteArray sendMsg;
    if(this->alarmType == Alarm_Sensor){
        int msgLen = sizeof(KEAlarmSenserResp);
        sendMsg.resize(msgLen);
        KEAlarmSenserResp * pMsg = (KEAlarmSenserResp *)sendMsg.data();
        pMsg->protocal = PROTOCOL_HEAD;
        pMsg->msgType = KEDevMsg_AlarmSenser;
        pMsg->msgLength = msgLen;
        pMsg->videoID = this->alarmInfo.channelID/256;
        pMsg->channelNo = this->alarmInfo.channelID%256;
        pMsg->state = this->alarmInfo.state;
        pMsg->resp = RESP_ACK;
        pMsg->number = this->alarmInfo.number;
    }
    else{
        int msgLen = sizeof(KEAlarmVideoResp);
        sendMsg.resize(msgLen);
        KEAlarmVideoResp * pMsg = (KEAlarmVideoResp *)sendMsg.data();
        pMsg->protocal = PROTOCOL_HEAD;
        pMsg->msgType = KEDevMsg_AlarmVideo;
        pMsg->msgLength = msgLen;
        pMsg->videoID = this->alarmInfo.channelID/256;
        pMsg->channelNo = this->alarmInfo.channelID%256;
        pMsg->type = this->alarmType;
        memcpy(pMsg->coordinate,this->alarmInfo.coordinate,9);
        pMsg->state = this->alarmInfo.state;
        pMsg->resp = RESP_ACK;
        pMsg->number = this->alarmInfo.number;
    }
    return this->m_socketHandle->WriteData(sendMsg);
}

bool DevAlarm::CheckAvaliable(int channelID)
{
    QList<DevAlarm *> listCommand =  this->parent()->findChildren<DevAlarm*>();
    if(listCommand.size() != 1){
        return false;
    }
    return true;
}
