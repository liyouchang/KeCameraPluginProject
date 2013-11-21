#include "SvrSimulateTerminal.h"
#include "KeProtocal/ProtocalProcess.h"
SvrSimulateTerminal::SvrSimulateTerminal(ProtocalProcess *protocal, QObject *parent) :
    TcpDeviceHolder(parent)
{

}

int SvrSimulateTerminal::SendMediaData(int channelNo, const char *data, int len)
{
    return 0;
}
