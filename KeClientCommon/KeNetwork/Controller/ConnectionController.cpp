#include "ConnectionController.h"
#include "DevConnection.h"
#include "kenet_global.h"
#include <QTcpSocket>
#include "device.h"
#include "CommunicationThread.h"
#include "SocketHandler.h"
#include "KeProtocal/ProtocalProcess.h"
#include "HandleManager.h"
void ConnectionController::SetDisConnectCallBack(fDisConnect cbDisConnect, void *user)
{
    this->cbDisConnect = cbDisConnect;
    this->userDisConnect = user;
}

void ConnectionController::SetAutoReconnectCallBack(fHaveReConnect cbAutoConnect, void *user)
{
    this->cbReconnect = cbAutoConnect;
    this->userReconnect = user;
}

ConnectionController *ConnectionController::CreateInstance(AbstractController *parent)
{
    QObject * parentDev = dynamic_cast<QObject *>(parent);
    if(parentDev == 0){
        qWarning("can not covert parent to QObject,parent must be a QObject");
        return 0;
    }

    ConnectionController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    case KE_DevType_08SERVER:
    case  KE_IPC_Simulator:
    {
        ProtocalProcess *protocal =  new ProtocalProcess();
        SocketHandler * sh = new SocketHandler();
        sh->CreateSocket();
        controller =  new DevConnection(sh,protocal,parentDev);
    }
        break;
    }
    HandleManager::instance().AddObject(controller);
    return controller;
}

ConnectionController *ConnectionController::CreateInstance(SocketHandler *socket, AbstractController *parent)
{
    QObject * parentDev = dynamic_cast<QObject *>(parent);
    if(parentDev == 0 && parent != 0){
        qWarning("can not covert parent to QObject,parent must be a QObject");
        return 0;
    }

    ConnectionController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    case KE_DevType_08SERVER:
    case KE_IPC_Simulator:
    {
        ProtocalProcess *protocal =  new ProtocalProcess();
        controller =  new DevConnection(socket,protocal,parentDev);
    }
        break;
    }
    HandleManager::instance().AddObject(controller);
    return controller;
}
