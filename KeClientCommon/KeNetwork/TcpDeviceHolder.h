#ifndef TCPDEVICEHOLDER_H
#define TCPDEVICEHOLDER_H

#include <QObject>
#include "Controller/InstanceController.h"
#include "Controller/ConnectionController.h"
#include "Device.h"
#include "kenet_global.h"
#include "TcpListener.h"

class SocketHandler;

class TcpDeviceHolder : public Device,public InstanceController
{
    Q_OBJECT
public:
    explicit TcpDeviceHolder();
    ~TcpDeviceHolder();
    TcpListener *m_server;
signals:
    void NewConnection(ConnectionController * c);
    void NewLoginInfo(int puid,QByteArray devMac);
public slots:
    void SearchedDev(int devIndex, QByteArray devInfo);
    virtual void sGetSocketHandler(void *);
    void havaNewLogin(int puid,QByteArray devMac);
    // InstanceController interface
public:
    int ListenConnect(int port);
    int SearchDevice();
    void SetNetParam(NET_PARAM param);
    NET_PARAM GetNetParam();
};

#endif // TCPDEVICEHOLDER_H
