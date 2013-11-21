#ifndef TCPDEVICEHOLDER_H
#define TCPDEVICEHOLDER_H

#include <QObject>
#include "Controller/InstanceController.h"
#include "Controller/ConnectionController.h"
#include "Device.h"
class SocketHandler;

class TcpDeviceHolder : public QObject,public InstanceController
{
    Q_OBJECT
public:
    explicit TcpDeviceHolder(QObject *parent = 0);

signals:
    void NewConnection(ConnectionController * c);
protected slots:
    void sGetSocketHandler(SocketHandler * sh);
public:
    int ListenConnect(int port);
    void SetNewConnectionCallBack(fNewConnection cbfunc, void *user);
protected:
    fNewConnection cbNewConnection;
    void * userNewConnection;
};

#endif // TCPDEVICEHOLDER_H
