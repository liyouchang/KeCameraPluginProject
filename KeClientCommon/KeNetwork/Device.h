#ifndef DEVICE_H
#define DEVICE_H

#include "Channel.h"
#include <QList>
class Device : public Channel
{
    Q_OBJECT
public:
    Device();
    Device(SocketHandler * s, ProtocalProcess *m_protocal,Device * parent);
    Device(SocketHandler * s,Device *parent);
    Device(Device * parent);
    virtual ~Device();
    QList<Channel *> childrenChannel;
signals:

public slots:
    virtual void GetMessageData(QByteArray &allBytes);
    // Channel interface
public:
    bool AddChannel(Channel * c);
    bool RemoveChannel(Channel *c);
    template<typename T>
    inline T  GetChannel(QString &name = QString()){
        for(int i=0;i<childrenChannel.size();++i){
            T t = qobject_cast<T>(childrenChannel.at(i));
            if(t != 0)  return t;

            Device * d = qobject_cast<Device *>(childrenChannel.at(i));
            if(d != 0){
                t =  d->GetChannel<T>(name);
                if(t != 0) return t;
            }
        }
        return 0;
    }
    template<typename T>
    inline QList<T>  GetChannelList(){
        QList<T> retList;
        for(int i=0;i<childrenChannel.size();++i){
            T t = qobject_cast<T>(childrenChannel.at(i));
            if(t != 0){
                retList.append(t);
            }
            Device * d = qobject_cast<Device *>(childrenChannel.at(i));
            if(d != 0){
                QList<T> tmpList =  d->GetChannelList<T>();
                if(!tmpList.empty()){
                    retList.append(tmpList);
                }
            }
        }
        return retList;
    }
    template<typename T>
    inline void ChildrenRespondMsg(QByteArray &msgData){
        for(int i=0;i<childrenChannel.size();++i){
            T t = qobject_cast<T>(childrenChannel.at(i));
            if(t != 0){
                childrenChannel.at(i)->OnRespond(msgData);
            }
            Device * d = qobject_cast<Device *>(childrenChannel.at(i));
            if(d != 0)
                d->ChildrenRespondMsg<T>(msgData);
        }
    }
    template<typename T>
    inline bool CheckChannelAvaliable(int channelID){
        for(int i = 0 ;i<childrenChannel.size();++i ){
            T t = qobject_cast<T>(childrenChannel.at(i));
            if(t != 0){
                if(childrenChannel.at(i)->getChannelID() == channelID)
                    return false;
            }
            Device * d = qobject_cast<Device *>(childrenChannel.at(i));
            if(d != 0){
                bool b = d->CheckChannelAvaliable<T>(channelID);
                if(b == false)
                    return false;
            }
        }
        return true;
    }

    //virtual Channel * GetChannel(int type,QString &name = QString());
protected:
};

#endif // DEVICE_H
