#ifndef KEDVRDEVICE_H
#define KEDVRDEVICE_H

#include "Device.h"
#include "Controller/ConnectionController.h"
#include <QTimer>
#include "KeProtocal/ChSecretKey.h"
#include "KeProtocal/ChLogin.h"

class DevConnection : public Device , public ConnectionController
{
    Q_OBJECT
public:
    explicit DevConnection(SocketHandler * s, ProtocalProcess *protocal,Device *parent);
    DevConnection(SocketHandler * s, Device *parent);
    virtual ~ DevConnection();
    int heartCount;
    ChSecretKey * askKey;
    ChLogin * login;
    bool toReconnect;
signals:

public slots:
    virtual void GetMessageData(QByteArray &allBytes);
    virtual void OnRespond(QByteArray & data);
    void SendHeartBeat();
    void Reconnect();
    virtual void DevDisconnect();
    virtual void DevConnected();
    // ConnectionController interface
public:
    virtual int ConnectDevice(const std::string &addr, int port);
    virtual int DisConnect();
    virtual int LoginServer(std::string username, std::string pwd);
    virtual int Logout();
    virtual int GetDeviceInfo(NET_DEVICEINFO *info);
    virtual int TryConnect(const std::string &addr,int port);

    // Channel interface
public:
    int Request();
protected:
    QTimer *heartbeatTimer;
    QTimer *reconnectTimer;
    std::string m_addr;
    int m_port;

};

#endif // KEDVRDEVICE_H
