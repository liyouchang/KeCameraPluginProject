#include "UpdateServer.h"
#include "DevUpdate.h"
#include "UpdateProtocal.h"
UpdateServer::UpdateServer()
{
}

void UpdateServer::sGetSocketHandler(SocketHandler *sh)
{
    qDebug("TcpDeviceHolder::sGetSocketHandler");
    ProtocalProcess *protocal =  new UpdateProtocal();
    DevUpdate * dev = new DevUpdate(sh,protocal,this);
    ConnectionController * c = dynamic_cast<ConnectionController * >(dev);
    emit this->NewConnection(c);
}
