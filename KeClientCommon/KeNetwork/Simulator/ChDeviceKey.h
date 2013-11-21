#ifndef CHDEVICEKEY_H
#define CHDEVICEKEY_H

#include "Channel.h"

class ChDeviceKey : public Channel
{
    Q_OBJECT
public:
    explicit ChDeviceKey(Channel *parent );

signals:

public:
    char key[8];
    char mac[16];
    QByteArray m_skey;
    // Channel interface
public slots:
    void OnRespond(QByteArray &msgData);

public:
    int Request();
};

#endif // CHDEVICEKEY_H
