#ifndef CHSEARCHDEVICE_H
#define CHSEARCHDEVICE_H

#include <QObject>
#include "Channel.h"
#include "kenet_global.h"
class ChSearchDevice : public Channel
{
    Q_OBJECT
public:
    ChSearchDevice(SocketHandler * s,Channel * parent);
    ChSearchDevice(SocketHandler * s,ProtocalProcess *protocal,Channel *parent);

signals:
    void recvDeviceInfo(int index,QByteArray devInfo);
public slots:
    void OnRespond(QByteArray &msgData);
    void GetMessageData(QByteArray &allBytes);

public:
    int Request();
    NET_DEVICEINFO devInfo;
    int devIndex;
};

#endif // CHSEARCHDEVICE_H
