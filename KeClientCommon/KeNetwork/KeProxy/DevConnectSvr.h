#ifndef DEVCONNECTSVR_H
#define DEVCONNECTSVR_H

#include <QObject>
#include "DevConnection.h"
#include "ChReceiveLogin.h"
class DevConnectSvr : public DevConnection
{
    Q_OBJECT
public:
    DevConnectSvr(SocketHandler * s, ProtocalProcess *protocal,Device * parent);
    ChReceiveLogin * chLogin;
signals:

public slots:
    // ConnectionController interface
public:
    int LoginServer(std::string username, std::string pwd);
    int Logout();

    // Channel interface
public slots:
    void OnRespond(QByteArray &msgData);
    virtual void DevDisconnect();
    virtual void HeartBeat();

public:
    int Request();
};

#endif // DEVCONNECTSVR_H
