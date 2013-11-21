#include "Channel.h"
#include<QMutexLocker>
#include "CommunicationThread.h"
Channel::Channel(SocketHandler *s, ProtocalProcess *protocal, QObject *parent):
    QObject(parent),opened(false),m_channelID(0)
{
    this->m_socketHandle = s;
    this->m_protocal = protocal;
    this->toHoldSocket = false;
    //m_clientID = 0;
    this->setClientID(0);
    //this->setChannelID(parent->getChannelID());


}

Channel::Channel(Channel *parent):
    QObject(parent),opened(false),m_channelID(0)
{
    this->m_socketHandle = parent->m_socketHandle;
    this->m_protocal = parent->m_protocal;
    this->toHoldSocket = false;
    this->setClientID(parent->getClientID());
    this->setChannelID(parent->getChannelID());

}

Channel::Channel(SocketHandler *s, Channel *parent):
    QObject(parent),opened(false),m_channelID(0)
{
    this->m_socketHandle = s;
    this->m_protocal = parent->m_protocal;
    this->toHoldSocket = false;
    this->setClientID(parent->getClientID());
    this->setChannelID(parent->getChannelID());

}

Channel::~Channel()
{
    if(toHoldSocket){
        qDebug()<<"Channel::~Channel() to delete socket!";
        delete this->m_socketHandle;
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



void Channel::OnRespond(QByteArray &msgData)
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
    return 0;
}


