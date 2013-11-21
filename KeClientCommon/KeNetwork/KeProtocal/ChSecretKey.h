#ifndef CHSECRETKEY_H
#define CHSECRETKEY_H

#include "Channel.h"

class ChSecretKey : public Channel
{
    Q_OBJECT
public:
    explicit ChSecretKey(Channel *parent = 0);

signals:
    // Channel interface
public slots:
    void OnRespond(QByteArray &data);

public:
    int Request();
    char key[8];
};

#endif // CHSECRETKEY_H
