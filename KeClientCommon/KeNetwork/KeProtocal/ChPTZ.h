#ifndef CHPTZ_H
#define CHPTZ_H

#include <QObject>
#include "Channel.h"
class ChPTZ : public Channel
{
    Q_OBJECT
public:
    explicit ChPTZ(Channel *parent);
    int cloudProtocal;//0,_D;1,_P
    int cloudAddr;
    bool isgot;
    int cloudCmd;
    int cloudSpeed;
signals:

public slots:
    void OnRespond(QByteArray &msgData);

public:
    int Request();
    QByteArray GetCloudData();
};

#endif // CHPTZ_H
