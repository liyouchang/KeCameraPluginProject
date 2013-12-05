#include "Channel.h"
#include<QMutexLocker>
#include "CommunicationThread.h"
#include "SocketHandler.h"
#include <QTimer>
#include "Device.h"
Channel::Channel():QObject(0)
{
    this->m_socketHandle = 0;
    this->m_protocal = 0;
    this->toHoldSocket = false;
    this->setClientID(0);
    this->setChannelID(0);
    this->moveToThread(&ChannelTread::instance());
    this->m_parentDev = 0;

}

Channel::Channel(SocketHandler *s, ProtocalProcess *protocal, Channel *parent):
    QObject(0)
{
    this->m_socketHandle = s;
    this->m_protocal = protocal;
    this->toHoldSocket = false;
    this->setClientID(0);
    this->setChannelID(0);

    this->moveToThread(&ChannelTread::instance());
    this->m_parentDev = 0;
    Device * d = qobject_cast<Device *>(parent);
    if(d){
        d->AddChannel(this);
    }
}

Channel::Channel(Channel *parent):
    QObject(0)
{
    this->m_socketHandle = parent->m_socketHandle;
    this->m_protocal = parent->m_protocal;
    this->toHoldSocket = false;
    this->setClientID(parent->getClientID());
    this->setChannelID(parent->getChannelID());
    this->moveToThread(&ChannelTread::instance());
    this->m_parentDev = 0;
    Device * d = qobject_cast<Device *>(parent);
    if(d){
        d->AddChannel(this);
    }
}

Channel::Channel(SocketHandler *s, Channel *parent):
    QObject(0)
{
    this->m_socketHandle = s;
    this->m_protocal = parent->m_protocal;
    this->toHoldSocket = false;
    this->setClientID(parent->getClientID());
    this->setChannelID(parent->getChannelID());
    this->moveToThread(&ChannelTread::instance());
    this->m_parentDev = 0;
    Device * d = qobject_cast<Device *>(parent);
    if(d){
        d->AddChannel(this);
    }
}

Channel::~Channel()
{
    if(this->m_parentDev){
        m_parentDev->RemoveChannel(this);
    }
    if(toHoldSocket){
        m_socketHandle->cleanUp();
    }
}

void Channel::setChannelID(int id)
{
    this->m_channelID = id;
}

void Channel::setClientID(int id)
{
    this->m_clientID = id;
}



void Channel::OnRespond(QByteArray &)
{
    qDebug()<<"the object "<<this->objectName()<<" not support OnRespond";
}
//timeout -- milliseconds
bool Channel::waitRespond(int timeout)
{
    //QMutexLocker locker(&waitMutex);
    waitMutex.lock();
    bool waitResult = waitCondition.wait(&waitMutex,timeout);
    waitMutex.unlock();
    return waitResult;
}

void Channel::wakeup()
{
    this->waitCondition.wakeAll();
}

int Channel::Request()
{
    if(!this->m_socketHandle->isValid()){
        return KE_Network_Invalid;
    }
    return 0;
}


