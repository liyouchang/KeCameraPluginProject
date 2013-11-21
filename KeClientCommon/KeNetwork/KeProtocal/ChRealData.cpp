#include "ChRealData.h"
#include "KeMessage.h"
#include "ProtocalProcess.h"
#include "SocketHandler.h"
ChRealData::ChRealData(int channelID, Channel *parent) :
    Channel(parent)
{
    this->setChannelID(channelID);
    this->saveFile = 0;
    qDebug("ChRealData::ChRealData channelid is %d",this->m_channelID);
    QObject::connect(this,&ChRealData::toDoRespond,this,&ChRealData::DoRespond);
}

ChRealData::~ChRealData()
{
    if(this->saveFile != 0)
        delete saveFile;
}

void ChRealData::DoRespond(int mediaType, QByteArray & data)
{
    emit this->recvMediaData(this->getChannelID(),mediaType,data);
    if(this->saveFile!=0 && this->saveFile->isOpen()){
        this->saveFile->write(data);
    }
}

void ChRealData::OnRespond(QByteArray &data)
{
    this->m_protocal->ParseMessage(data,this);
}

int ChRealData::Request()
{
    QByteArray msgSend = this->m_protocal->CreateMessage(this);
    int ret = this->m_socketHandle->WriteData(msgSend);
    return ret;
}
