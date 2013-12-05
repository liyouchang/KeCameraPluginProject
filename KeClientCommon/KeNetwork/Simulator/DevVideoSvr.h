#ifndef DEVVIDEOSVR_H
#define DEVVIDEOSVR_H

#include "Device.h"
class ChSimulateVideo;
class DevVideoSvr : public Device
{
    Q_OBJECT
public:
    explicit DevVideoSvr(Device *parent);

signals:
    void toDoRespond();
    void toSendMediaData(QByteArray mediaData);
    // Channel interface
public slots:
    void OnRespond(QByteArray &msgData);
    void DoRespond();
    void sendMedia(int channelNo, QByteArray & mediaData);

public:
    int Request();

    int m_number;
public:
    int mediaType;
    int respond;
    quint32 transIp;
    // Channel interface
public:
    void setChannelID(int id);
};

#endif // DEVVIDEOSVR_H
