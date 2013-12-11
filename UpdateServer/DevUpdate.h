#ifndef DEVUPDATE_H
#define DEVUPDATE_H

#include <QObject>
#include "KeProxy/DevConnectSvr.h"
#include <QVector>
#include <QTime>
#include <QTimer>

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
    QTime heartTime;
    int onceHeartTime;
    int heartTimeoutCount;
signals:
    //lastPack -1 start head , -2 error
    void toSendUpdate();
    void DevUpdateStatus(void * dev,QString devMac,QString version,QString puType);
    //percent 0-100 update process,200 update success,201 update error,202 write error
    void DevUpdateProcess(void * dev,int percent);
    void DeleteDevice(void *dev);
public slots:
    void OnRespond(QByteArray &msgData);
    void HeartBeat();
    void doRespond(int msgType);
    void doSendUpdate();
    void StartDevUpdate(QByteArray allData);
    void DevDisconnect();
public:
    int Request();


};

#endif // DEVUPDATE_H
