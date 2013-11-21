#ifndef KEMEDIACHANNEL_H
#define KEMEDIACHANNEL_H

#include "Channel.h"
class KeMediaChannel : public Channel
{
    Q_OBJECT
public:
    explicit KeMediaChannel(int cameraID,SocketHandler * s = 0,QObject *parent = 0);
    enum MediaType
    {
        Media_None = 0x00,
        Media_Vedio = 0x01,
        Media_Listen = 0x02,
        Media_Talk = 0x04,
        Media_ALL = 7
    };
signals:
    void recvMediaData(int cameraID,int mediaType,QByteArray data);
public slots:


    // Channel interface
public:
    void OnRespond(const char *buf, int len);
    int Request();
    void setMediaType(MediaType type);
    QString createName();

protected:
    int ReqestVideoServer();
private:
    int mCameraID;
    int mMediaType;
    // Channel interface
public:
};

#endif // KEMEDIACHANNEL_H
