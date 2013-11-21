#ifndef DEVICE_H
#define DEVICE_H

#include "Channel.h"

class Device : public Channel
{
    Q_OBJECT
public:
    explicit Device(SocketHandler * s,ProtocalProcess *m_protocal,QObject *parent = 0);
    Device(SocketHandler * s,Device *parent);
    Device(Device * parent);
    virtual ~Device();

signals:

public slots:


    // Channel interface
public:
    bool AddChannel(Channel * c);

    template<typename T>
    inline Channel * GetChannel(QString &name = QString()){
        T find = this->findChild<T>(name);
        return qobject_cast<Channel *>(find);
    }
    template<typename T>
    inline void ChildrenRespondMsg(QByteArray &msgData){
        QList<T> listCommand =  this->findChildren<T>();
        for (int i = 0; i < listCommand.size(); ++i) {
            Channel * c = qobject_cast<Channel *>(listCommand.at(i));
            if(c != 0)
                c->OnRespond(msgData);
        }
    }
    template<typename T>
    inline bool CheckChannelAvaliable(int channelID){
        QList<T> listCommand =  this->findChildren<T>();
        for (int i = 0; i < listCommand.size(); ++i) {
            Channel * c = qobject_cast<Channel *>(listCommand.at(i));
            if(c == 0){
                return false;
            }
            if(c->getChannelID() == channelID){
                return false;
            }
        }
    }

    //virtual Channel * GetChannel(int type,QString &name = QString());
protected:
};

#endif // DEVICE_H
