#ifndef CHSIMULATEVIDEO_H
#define CHSIMULATEVIDEO_H

#include <QObject>
#include <Channel.h>
enum MediaType
{
    Media_None = 0x00,
    Media_Vedio = 0x01,
    Media_Audio = 0x02
};

class ChSimulateVideo : public Channel
{
    Q_OBJECT
public:
    explicit ChSimulateVideo(Channel *parent );
    ChSimulateVideo(SocketHandler * s,Channel *parent);

signals:

public slots:
    void sendMedia(int channelNo, QByteArray & mediaData);

    // Channel interface
public:
    void OnRespond(QByteArray & data);
    int Request();
    int clientID;
    int toSendData;
    QByteArray mediaData;
    short currentFrameNo;
    short lastFrameNo;
    int mediaType;
};

#endif // CHSIMULATEVIDEO_H
