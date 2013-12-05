#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include<QAbstractSocket>
#include <QMutex>
#include<QWaitCondition>
#include <QThread>
#include <QByteArray>
#include <QDebug>
#include "kenet_global.h"
class SocketHandler;
class ProtocalProcess;
class Device;
class Channel : public QObject
{
    Q_OBJECT
public:
    Channel();
    Channel(SocketHandler * s, ProtocalProcess *m_protocal,Channel * parent);
    Channel(Channel * parent);
    Channel(SocketHandler * s,Channel *parent);
    virtual ~Channel();
signals:

public slots:
    virtual void OnRespond(QByteArray & msgData);
protected:
    bool toHoldSocket;
    int m_channelID;
    int m_clientID;
    QWaitCondition waitCondition;
    QMutex waitMutex;
    ProtocalProcess *m_protocal;

public:
    Device * m_parentDev;
    Device * getParentDev(){return m_parentDev;}
    void setParentDev(Device * p){this->m_parentDev = p;}
    SocketHandler * m_socketHandle;
    SocketHandler * getSocketHandle(){return m_socketHandle;}
    void setSocketHandle(SocketHandler * s){this->m_socketHandle = s;}

    int getChannelID(){return m_channelID;}
    virtual void setChannelID(int id);
    int getClientID(){return m_clientID;}
    void setClientID(int id);
    virtual QString createName(){return QString();}
    virtual bool waitRespond(int timeout = 3000);
    virtual void wakeup();
    virtual int Request();

};

#endif // CHANNEL_H
