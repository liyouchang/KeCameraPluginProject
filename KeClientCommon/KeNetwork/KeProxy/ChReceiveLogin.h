#ifndef CHRECEIVELOGIN_H
#define CHRECEIVELOGIN_H

#include <QObject>
#include "Channel.h"
#include <list>
#include <string>
class ChReceiveLogin : public Channel
{
    Q_OBJECT
public:
    explicit ChReceiveLogin(Channel *parent );
    QByteArray devMac;
    std::list<std::string> allowLoginMacList;
signals:
    void getDeviceLogin(int puid,QByteArray devMac);
public slots:


    // Channel interface
public slots:
    void OnRespond(QByteArray &msgData);

public:
    int Request();
};

#endif // CHRECEIVELOGIN_H
