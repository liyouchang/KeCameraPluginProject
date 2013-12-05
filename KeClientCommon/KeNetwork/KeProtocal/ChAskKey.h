#ifndef CHASKKEY_H
#define CHASKKEY_H

#include <QObject>
#include "Channel.h"
class ChAskKey : public Channel
{
    Q_OBJECT
public:
    explicit ChAskKey(Channel *parent);
    QByteArray m_skey;

signals:

public slots:
    void OnRespond(QByteArray &msgData);

public:
    int Request();
};

#endif // CHASKKEY_H
