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
    qDebug("receive alarm %d!",msgType);
    switch(msgType)
    {
    case KEDevMsg_AlarmSenser:
    {
        KEAlarmSenserReq * pMsg = (KEAlarmSenserReq *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID,pMsg->channelNo);
//        if(this->getChannelID() != pMsg->videoID){
//            qDebug("DevAlarm::OnRespond pu id error");
//            return;
//        }
        this->alarmType =Alarm_Sensor;
        this->alarmInfo.channelID = cameraID;
        this->alarmInfo.state = pMsg->state;
        this->alarmInfo.second = pMsg->second;
        this->alarmInfo.msecond = pMsg->msecond;
        this->alarmInfo.number = pMsg->number;

        QByteArray data((char *)&this->alarmInfo,sizeof(KEAlarmInfo));
        emit toDoRespond(this->alarmType,data);

    }
        break;
    case KEDevMsg_AlarmVideo:
    {
        KEAlarmVideoReq * pMsg = (KEAlarmVideoReq *)msgData.data();
        int cameraID = CreateCameraID(pMsg->videoID,pMsg->channelNo);
//        if(this->getChannelID() != pMsg->videoID){
//            qDebug("DevAlarm::OnRespond pu id error");
//            return;
//        }
        this->alarmType =pMsg->type;
        this->alarmInfo.channelID = cameraID;
        this->alarmInfo.state = pMsg->state;
        this->alarmInfo.second = pMsg->second;
        this->alarmInfo.msecond = pMsg->msecond;
        this->alarmInfo.number = pMsg->number;
        memcpy(this->alarmInfo.coordinate,pMsg->coordinate,9);

        QByteArray data((char *)&this->alarmInfo,sizeof(KEAlarmInfo));
        emit toDoRespond(this->alarmType,data);
    }
        break;
    default:break;
    }
}

void DevAlarm::DoRespond(int alarmType,QByteArray info)
{
    //response alarm message
    this->Request();
    if(this->cbAlarm){
        this->cbAlarm(this->getHandler(),alarmType,info.data(),info.size(),this->userAlarm);
    }
}

int DevAlarm::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }

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
    QList<DevAlarm *> listCommand = this->getParentDev()->GetChannelList<DevAlarm *>();
    if(listCommand.size() != 1){
        return false;
    }
    return true;
}
