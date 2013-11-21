#ifndef KETALKCHANNEL_H
#define KETALKCHANNEL_H

#include "channel.h"
class KeTalkChannel : public Channel
{
    Q_OBJECT
public:
    explicit KeTalkChannel(int cameraID, SocketHandler *s = 0, QObject *parent = 0);

signals:

public slots:


protected:
    int mCameraID;
    QByteArray talkData;
    // Channel interface
public:
    int Request();
    void setTalkData(const char * buf,int len);
};

#endif // KETALKCHANNEL_H
