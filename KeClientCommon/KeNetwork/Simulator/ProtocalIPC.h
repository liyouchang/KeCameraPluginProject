#ifndef PROTOCALIPC_H
#define PROTOCALIPC_H

#include <QObject>
#include "KeProtocal/ProtocalProcess.h"
#include "ChSimulateVideo.h"
#include "DevSimulateIPC.h"
class ProtocalIPC : public ProtocalProcess
{
    Q_OBJECT
public:
    explicit ProtocalIPC(QObject *parent = 0);

signals:

public slots:
    virtual void ParseMessage(QByteArray & msgData,DevConnection * ch);
    virtual void ParseMessage(QByteArray &msgData, DevSimulateIPC *ch);
public:
    virtual QByteArray CreateMessage(ChSimulateVideo *ch);
    virtual QByteArray CreateMessage(DevSimulateIPC *ch);
};

#endif // PROTOCALIPC_H
