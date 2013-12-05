#ifndef CHASKVIDEO_H
#define CHASKVIDEO_H

#include <QObject>
#include "Channel.h"

enum MediaType
{
    Media_None = 0x00,
    Media_Vedio = 0x01,
    Media_Listen = 0x02,
    Media_Talk = 0x04,
    Media_ALL = 7
};


class ChAskVideo : public Channel
{
    Q_OBJECT
public:
    ChAskVideo(SocketHandler * s,Channel *parent);
    int m_mediaType;
signals:

public slots:
    void OnRespond(QByteArray & msgData);

public:
    int Request();
};

#endif // CHASKVIDEO_H
