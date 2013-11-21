#include "DevRealPlay.h"
#include "KeMessage.h"
#include <QDebug>
#include "ProtocalProcess.h"
#include "SocketHandler.h"
#include <QFile>
#include "ChPTZ.h"
DevRealPlay::DevRealPlay(Device *parent) :
    Device(parent)
{
    chRealData = 0;
    this->cbRealDisConnect = 0;
    this->userRealDisConnect = 0;
    this->cbRealData = 0;
    this->userRealData = 0;
    this->setChannelID(0);

}

void DevRealPlay::MediaDataRecv(int cameraID, int dataType, QByteArray data)
{
    emit recvMediaData(cameraID,dataType,data);
    if(cbRealData){
        cbRealData(this->getHandler(),dataType,data.constData(),data.size(), userRealData);
    }
}

int DevRealPlay::StartRealPlay(int channelID)
{
    if(m_channelID == 0){
        m_channelID = channelID;
        int ret = this->Request();
        if(ret != 0){
            qWarning("Real start request error!");
            return ret;
        }
        if(!this->waitRespond()){
            qWarning("wait StartRealPlay respond error!");
            return KE_Msg_Timeout;
        }
        chRealData = new ChRealData(channelID,this);
        QObject::connect(chRealData,&ChRealData::recvMediaData,this,&DevRealPlay::MediaDataRecv);
    }

    return KE_SUCCESS;
}

int DevRealPlay::StopRealPlay()
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    m_channelID = 0;
    delete this;
    return KE_SUCCESS;
}

void DevRealPlay::SetRealPlayDisConnect(fRealPlayDisConnect cbRealDisConnect, void *user)
{
    this->cbRealDisConnect = cbRealDisConnect;
    this->userRealDisConnect = user;
}

void DevRealPlay::SetRealDataCallBack(fRealDataCallBack cbRealData, void *user)
{
    this->cbRealData = cbRealData;
    this->userRealData = user;
}

int DevRealPlay::SaveRealData(const char *fileName)
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }

    QFile * file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly)){
        qWarning("open file  %s error",fileName);
        return KE_File_Open_Error;
    }

    this->chRealData->saveFile = file;

    return KE_SUCCESS;
}

int DevRealPlay::StopSaveRealData()
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    if(this->chRealData->saveFile->isOpen()){
        this->chRealData->saveFile->close();
    }

    return KE_SUCCESS;
}

int DevRealPlay::StartSound()
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    this->m_mediaType |= Media_Listen;
    this->Request();
    if(!this->waitRespond()){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::StopSound()
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    this->m_mediaType &= ~Media_Listen;
    this->Request();
    if(!this->waitRespond()){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::StartTalk()
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    this->m_mediaType |= Media_Talk;
    this->Request();
    if(!this->waitRespond()){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::TalkSendData(const char *pSendBuf, int dwBufSize)
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    const int timeSpanSize = 6;
    this->chRealData->timeSpan = QByteArray(pSendBuf,timeSpanSize);
    this->chRealData->talkData = QByteArray(pSendBuf+timeSpanSize,dwBufSize-timeSpanSize);
    return this->chRealData->Request();
}

int DevRealPlay::StopTalk()
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    this->m_mediaType &= ~Media_Talk;
    this->Request();
    if(!this->waitRespond()){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::PTZControl(int PTZCommand, int step, int stop)
{
    if(this->chRealData == 0){
        return KE_No_Initial;
    }
    if(PTZCommand > 31 || PTZCommand < 0){
        return KE_Parameter_Error;
    }
    ChPTZ * ptz = this->findChild<ChPTZ *>();
    if(ptz == 0){
        ptz = new ChPTZ(this);
    }
    ptz->cloudCmd = PTZCommand;
    ptz->cloudSpeed = step;
    ptz->Request();
    if(!this->waitRespond()){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return 0;
}

QString DevRealPlay::CreateNameByParam(int channelID)
{
    QString name = QString("RealPlayDevice_WithChannelID_%1").arg(channelID);
    return name;
}

QString DevRealPlay::createName()
{
    return DevRealPlay::CreateNameByParam(this->m_channelID);
}


void DevRealPlay::OnRespond(QByteArray &data)
{
    this->m_protocal->ParseMessage(data,this);
}
int DevRealPlay::Request()
{
    QByteArray msgSend = this->m_protocal->CreateMessage(this);
    int ret = this->m_socketHandle->WriteData(msgSend);
    return ret;
}

bool DevRealPlay::CheckAvaliable(int channelID)
{
    Device * devParent = qobject_cast<Device *>(this->parent());
    if(devParent == 0)
        return false;
    return devParent->CheckChannelAvaliable<DevRealPlay *>(channelID);
}


