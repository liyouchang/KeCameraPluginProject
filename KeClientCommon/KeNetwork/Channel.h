#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include<QAbstractSocket>
#include <QMutex>
#include<QWaitCondition>
#include <QThread>
#include <QByteArray>
#include <QDebug>
class SocketHandler;
class ProtocalProcess;
class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(SocketHandler * s,ProtocalProcess *m_protocal,QObject *parent);
    Channel(Channel * parent);
    Channel(SocketHandler * s,Channel *parent);
    virtual ~Channel();
signals:

public slots:
    virtual void OnRespond(QByteArray & msgData);
protected:
    bool toHoldSocket;
    bool opened;
    int m_channelID;
    int m_clientID;
    QWaitCondition waitCondition;
    QMutex waitMutex;
    ProtocalProcess *m_protocal;

public:
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
