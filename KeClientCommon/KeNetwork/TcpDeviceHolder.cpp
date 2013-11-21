#include "TcpDeviceHolder.h"
#include "TcpListener.h"
#include "CommunicationThread.h"
TcpDeviceHolder::TcpDeviceHolder(QObject *parent) :
    QObject(parent)
{
    cbNewConnection = 0;
    userNewConnection = 0;
    //ChannelTread::instance().start();
    //this->moveToThread(&ChannelTread::instance());

}

void TcpDeviceHolder::sGetSocketHandler(SocketHandler *sh)
{
    ConnectionController * controller = ConnectionController::CreateInstance(sh,this);
    emit NewConnection(controller);
    if(cbNewConnection){
        cbNewConnection(this->getHandler(),controller->getHandler(),userNewConnection);
    }
}

int TcpDeviceHolder::ListenConnect(int port)
{
    TcpListener *server = new TcpListener(this);
    if (!server->isListening()) {
        // Set up the peer wire server
        if (!server->listen(QHostAddress::Any, port))
            return KE_NETWORK_ERROR;
    }
    QObject::connect(server,&TcpListener::newSocketHandler,
                     this,&TcpDeviceHolder::sGetSocketHandler);
    return 0;
}

void TcpDeviceHolder::SetNewConnectionCallBack(fNewConnection cbfunc, void *user)
{
    this->cbNewConnection = cbfunc;
    this->userNewConnection = user;
}
