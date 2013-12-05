#ifndef CHREALDATA_H
#define CHREALDATA_H
#include "Channel.h"
#include <QFile>
class ChRealData : public Channel
{
    Q_OBJECT
public:
    explicit ChRealData(int channelID, Channel *parent);
    ChRealData(SocketHandler * s,Channel *parent);
    ~ChRealData();
    QFile * saveFile;
    QByteArray timeSpan;
    QByteArray talkData;
signals:
    void recvMediaData(int channelID,int mediaType,QByteArray data);
    void toDoRespond(int mediaType, QByteArray & data);
public slots:
    void DoRespond(int mediaType,QByteArray & data);
protected:
    // Channel interface
public:
    void OnRespond(QByteArray & data);
    int Request();
};

#endif // CHREALDATA_H
