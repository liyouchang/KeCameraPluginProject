#ifndef SVRSIMULATETERMINAL_H
#define SVRSIMULATETERMINAL_H

#include <QObject>
#include "SimulatorController.h"
#include "Device.h"
#include "TcpDeviceHolder.h"


class ProtocalProcess;

class SvrSimulateTerminal : public TcpDeviceHolder,public SimulatorController
{
    Q_OBJECT
public:
    explicit SvrSimulateTerminal(ProtocalProcess * protocal,QObject *parent = 0);

signals:

public slots:


    // SimulatorController interface
public:
    int SendMediaData(int channelNo, const char *data, int len);
    ProtocalProcess *  svrProtocal;
};

#endif // SVRSIMULATETERMINAL_H
