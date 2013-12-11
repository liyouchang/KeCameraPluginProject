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
    m_videoSocket = new SocketHandler();
    QObject::connect(m_videoSocket,&SocketHandler::sdReadedData,
                     this,&DevRealPlay::GetMessageData,
                     Qt::DirectConnection);
    this->m_realTimeout = 3000;

}

DevRealPlay::~DevRealPlay()
{
    m_videoSocket->cleanUp();
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
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }

    if(m_channelID == 0){
        m_videoSocket->CreateSocket();

//        int ret = m_videoSocket->ConnectToAddr(
//                    this->m_socketHandle->m_addr,
//                    this->m_socketHandle->m_port,
//                    this->m_realTimeout);
                int ret = m_videoSocket->ConnectToAddr(
                            this->m_socketHandle->m_addr,
                            22616,
                            this->m_realTimeout);
        if(ret != KE_SUCCESS){
            return ret;
        }
        this->setChannelID(this->m_channelID*256+channelID);
        this->chVideo = new ChAskVideo(m_videoSocket,this);
        this->chVideo->m_mediaType |= Media_Vedio;
        ret = this->chVideo->Request();
        if(ret != 0){
            qWarning("Real start request error!");
            return ret;
        }
        if(!this->chVideo->waitRespond(this->m_realTimeout)){
            qWarning("wait StartRealPlay respond error!");
            return KE_Msg_Timeout;
        }
        this->chRealData = new ChRealData(m_videoSocket,this);
        QObject::connect(this->chRealData,&ChRealData::recvMediaData,
                         this,&DevRealPlay::MediaDataRecv);
    }

    return KE_SUCCESS;
}

int DevRealPlay::StopRealPlay()
{
    if(this->m_channelID == 0){
        return KE_No_Initial;
    }
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
    if(this->chVideo == 0){
        return KE_No_Initial;
    }
    this->chVideo->m_mediaType |= Media_Listen;
    this->chVideo->Request();
    if(!this->chVideo->waitRespond(this->m_realTimeout)){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::StopSound()
{
    if(this->chVideo == 0){
        return KE_No_Initial;
    }
    this->chVideo->m_mediaType &= ~Media_Listen;
    this->chVideo->Request();
    if(!this->chVideo->waitRespond(this->m_realTimeout)){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::StartTalk()
{
    if(this->chVideo == 0){
        return KE_No_Initial;
    }
    this->chVideo->m_mediaType |= Media_Talk;
    this->chVideo->Request();
    if(!this->chVideo->waitRespond(this->m_realTimeout)){
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
    this->chVideo->m_mediaType &= ~Media_Talk;
    this->chVideo->Request();
    if(!this->chVideo->waitRespond(this->m_realTimeout)){
        qWarning("wait StartRealPlay respond error!");
        return KE_Msg_Timeout;
    }
    return KE_SUCCESS;
}

int DevRealPlay::PTZControl(int PTZCommand, int step, int stop)
{
    if(this->m_channelID == 0){
        return KE_No_Initial;
    }
    if(PTZCommand > 31 || PTZCommand < 0){
        return KE_Parameter_Error;
    }
    ChPTZ * ptz = this->GetChannel<ChPTZ *>();
    if(ptz == 0){
        ptz = new ChPTZ(this);
    }
    ptz->cloudCmd = PTZCommand;
    ptz->cloudSpeed = step;
    return ptz->Request();
    //    if(!ptz->waitRespond()){
    //        qWarning("wait PTZControl respond error!");
    //        return KE_Msg_Timeout;
    //    }

}

void DevRealPlay::SetTimeout(int time)
{
    this->m_realTimeout =  time;
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
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }
//    QByteArray msgSend = this->m_protocal->CreateMessage(this);
//    int ret = this->m_socketHandle->WriteData(msgSend);
//    return ret;
    return KE_SUCCESS;
}

bool DevRealPlay::CheckAvaliable(int channelID)
{
   // Device * devParent = qobject_cast<Device *>(this->parent());
    Device * devParent = this->getParentDev();
    if(devParent == 0)
        return false;
    return devParent->CheckChannelAvaliable<DevRealPlay *>(channelID);
}


