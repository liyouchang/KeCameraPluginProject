#ifndef UPDATESERVER_H
#define UPDATESERVER_H

#include <QObject>
#include "TcpDeviceHolder.h"
class UpdateServer : public TcpDeviceHolder
{
    Q_OBJECT
public:
    explicit UpdateServer();

signals:

public slots:
    // TcpDeviceHolder interface
public slots:
    void sGetSocketHandler(SocketHandler *sh);
};

#endif // UPDATESERVER_H
