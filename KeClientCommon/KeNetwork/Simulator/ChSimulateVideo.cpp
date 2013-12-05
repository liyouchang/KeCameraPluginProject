#include "ChSimulateVideo.h"
#include "KeProtocal/ProtocalProcess.h"
#include "SocketHandler.h"
#include "ProtocalIPC.h"
ChSimulateVideo::ChSimulateVideo(Channel *parent) :
    Channel(parent)
{
    this->setChannelID(parent->getChannelID());

}

ChSimulateVideo:: ChSimulateVideo(SocketHandler *s, Channel *parent):
    Channel(s,parent)
{
    this->setChannelID(parent->getChannelID());
    this->toHoldSocket = true;
    QObject::connect(s,&SocketHandler::saDisConnected,
                     this,&ChSimulateVideo::VideoDisconnected);
}


void ChSimulateVideo::sendMedia(QByteArray mediaData)
{

    //int tmpNo = m_channelID%256;
    //if(tmpNo == channelNo){
        this->mediaData = mediaData;
        this->Request();
    //}
}

void ChSimulateVideo::VideoDisconnected()
{
    qDebug("disconnect video ,delete ChSimulateVideo");
    delete this;
}

void ChSimulateVideo::OnRespond(QByteArray &data)
{
    //ProtocalIPC * protocal = qobject_cast<ProtocalIPC *>(this->m_protocal);
    //protocal->ParseMessage(data,this);
}

int ChSimulateVideo::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }
    ProtocalIPC * protocal = qobject_cast<ProtocalIPC *>(this->m_protocal);
    QByteArray data = protocal->CreateMessage(this);
    return this->m_socketHandle->WriteData(data);
}
