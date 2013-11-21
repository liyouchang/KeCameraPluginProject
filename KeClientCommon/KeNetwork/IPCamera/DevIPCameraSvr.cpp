#include "DevIPCameraSvr.h"
#include "TcpListener.h"
#include "kenet_global.h"
#include "SocketHandler.h"
#include "HandleManager.h"
DevIPCameraSvr::DevIPCameraSvr(int type, QObject *parent) :
    QObject(parent)
{
    this->type = type;
    cbNewConnection = 0;
    userNewConnection = 0;
}

void DevIPCameraSvr::sGetSocketHandler(SocketHandler *sh)
{
    ConnectionController * controller = ConnectionController::CreateInstance(sh,this);
    emit NewConnection(controller);
    if(cbNewConnection){
        cbNewConnection(this->getHandler(),controller->getHandler(),userNewConnection);
    }
}

int DevIPCameraSvr::ListenConnect(int port)
{
    TcpListener *server = new TcpListener(this);
    if (!server->isListening()) {
        // Set up the peer wire server
        if (!server->listen(QHostAddress::Any, port))
            return KE_NETWORK_ERROR;
    }
    QObject::connect(server,&TcpListener::newSocketHandler,this,&DevIPCameraSvr::sGetSocketHandler);
    return 0;
}

void DevIPCameraSvr::SetNewConnectionCallBack(fNewConnection cbfunc, void *user)
{
    this->cbNewConnection = cbfunc;
    this->userNewConnection = user;
}
