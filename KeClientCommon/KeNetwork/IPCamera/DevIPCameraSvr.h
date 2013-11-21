#ifndef DEVIPCAMERASVR_H
#define DEVIPCAMERASVR_H

#include "Device.h"
#include "Controller/InstanceController.h"
#include "Controller/ConnectionController.h"

class SocketHandler;
class DevIPCameraSvr : public QObject,public InstanceController
{
    Q_OBJECT
public:
    explicit DevIPCameraSvr(int type,QObject *parent = 0);

signals:
    void NewConnection(ConnectionController * c);
protected slots:
    void sGetSocketHandler(SocketHandler * sh);
    // InstanceController interface
public:
    int ListenConnect(int port);
    void SetNewConnectionCallBack(fNewConnection cbfunc, void *user);

protected:
    fNewConnection cbNewConnection;
    void * userNewConnection;
};

#endif // DEVIPCAMERASVR_H
