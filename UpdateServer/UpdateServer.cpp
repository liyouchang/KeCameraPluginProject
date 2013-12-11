#include "UpdateServer.h"
#include "DevUpdate.h"
#include "UpdateProtocal.h"
#include "SocketHandler.h"
UpdateServer::UpdateServer()
{
}

void UpdateServer::sGetSocketHandler(void* handle)
{

    qDebug("TcpDeviceHolder::sGetSocketHandler");
    SocketHandler * s = new SocketHandler();
    ProtocalProcess *protocal =  new UpdateProtocal();
    DevUpdate * dev = new DevUpdate(s,protocal,this);
    s->CreateSocket();
    s->SetSocketDescriptor((qintptr)handle);
    ConnectionController * c = dynamic_cast<ConnectionController * >(dev);
    emit this->NewConnection(c);
}
