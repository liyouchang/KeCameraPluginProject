#ifndef KEDVRDEVICE_H
#define KEDVRDEVICE_H

#include "Device.h"
#include "Controller/ConnectionController.h"
class DevConnection : public Device , public ConnectionController
{
    Q_OBJECT
public:
    explicit DevConnection(SocketHandler * s, ProtocalProcess *protocal, QObject *parent);
    DevConnection(SocketHandler * s, Device *parent);
    virtual ~ DevConnection();
signals:

public slots:
    virtual void GetMessageData(QByteArray &allBytes);
public:
    virtual void OnRespond(QByteArray & data);

    // ConnectionController interface
public:
    virtual int ConnectDevice(const std::string &addr, int port);
    virtual int DisConnect();
    virtual int LoginServer(std::string username, std::string pwd);
    virtual int Logout();
    // Channel interface

};

#endif // KEDVRDEVICE_H
