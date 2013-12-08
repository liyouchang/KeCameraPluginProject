#ifndef DEVUPDATE_H
#define DEVUPDATE_H

#include <QObject>
#include "KeProxy/DevConnectSvr.h"
#include <QVector>


class DevUpdate : public DevConnectSvr
{
    Q_OBJECT
public:
    explicit DevUpdate(SocketHandler *s, ProtocalProcess *protocal, Device *parent);
    short totalPackage;
    short currentPackage;
    bool updateStart;
    QByteArray allData;
    static const int ONE_PACK_SIZE = 1024;
    int oldPercent;
signals:
    void DevUpdateStatus(void * dev,QString devMac,QString version,QString puType);
    void DevUpdateProcess(void * dev,int percent);
    void DeleteDevice(void *dev);
public slots:
    void OnRespond(QByteArray &msgData);
    void HeartBeat();
    void doRespond(int msgType);
    void doSendUpdate(int lastPack);
    void StartDevUpdate(QByteArray allData);
    void DevDisconnect();

public:
    int Request();


};

#endif // DEVUPDATE_H
