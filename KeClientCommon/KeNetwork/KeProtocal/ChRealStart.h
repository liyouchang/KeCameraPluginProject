#ifndef CHREALSTART_H
#define CHREALSTART_H
#include "Channel.h"

enum MediaType
{
    Media_None = 0x00,
    Media_Vedio = 0x01,
    Media_Listen = 0x02,
    Media_Talk = 0x04,
    Media_ALL = 7
};

class ChRealStart : public Channel
{
    Q_OBJECT
public:
    explicit ChRealStart(int channelID, Channel *parent );

signals:
public slots:
protected:
    int ReqestVideoServer();
    // Channel interface
public:
    void OnRespond(QByteArray & data);
    int Request();
    void setMediaType(MediaType type);

    int m_mediaType;

};

#endif // CHREALSTART_H
