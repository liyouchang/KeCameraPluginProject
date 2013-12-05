#ifndef PROTOCALKAERPROXY_H
#define PROTOCALKAERPROXY_H

#include <QObject>
#include "KeProtocal/ProtocalProcess.h"
#include "DevConnectSvr.h"
class ProtocalKaerProxy : public ProtocalProcess
{
    Q_OBJECT
public:
    explicit ProtocalKaerProxy(QObject *parent = 0);

signals:

public slots:


};

#endif // PROTOCALKAERPROXY_H
