#include "ConnectionController.h"
#include "DevConnection.h"
#include "kenet_global.h"
#include <QTcpSocket>
#include "device.h"
#include "CommunicationThread.h"
#include "SocketHandler.h"
#include "KeProtocal/ProtocalProcess.h"
#include "HandleManager.h"
#include "Controller/InstanceController.h"
#include "KeProtocal/ProtocalKaerServer.h"
#include "KeProxy/DevConnectSvr.h"
#include "KeProxy/ProtocalKaerProxy.h"
#include "TcpDeviceHolder.h"
#include "DeviceFactory.h"

int ConnectionController::TryConnect(const std::string &addr, int port)
{
    return KE_Not_Support_Function;
}

void ConnectionController::SetConnectStatusCallBack(fConnectStatusCallBack cbConnectStatus, void *user)
{
    this->cbConnectStatus = cbConnectStatus;
    this->userConnectStatus = user;
}

ConnectionController *ConnectionController::CreateInstance(AbstractController *parent)
{
    //DeviceFactory & factory = DeviceFactory::instance();
    AbstractController * ac = DeviceFactory::instance().CreateConnectionController(parent);
    return dynamic_cast<ConnectionController * >(ac);

//    Device * parentDev = dynamic_cast<Device *>(parent);
//    if(parentDev == 0){
//        qWarning("can not covert parent to QObject,parent must be a QObject");
//        return 0;
//    }

//    ConnectionController * controller = 0;
//    switch (parent->getType()){
//    case KE_DevType_DVR:
//    case KE_DevType_08Client:
//    case  KE_IPC_Simulator:
//    case KE_DevType_08Proxy:
//    {
//        ProtocalProcess *protocal =  new ProtocalProcess();
//        SocketHandler * sh = new SocketHandler();
//        sh->CreateSocket();
//        //controller =  new DevConnection(sh,protocal,parentDev);
//        DevConnection *newDev =  new DevConnection(sh,protocal,parentDev);
//        //parentDev->AddChannel(newDev);
//        controller = dynamic_cast<ConnectionController *>(newDev);
//        controller->setType(parent->getType());
//        controller->SetNetParam(parent->GetNetParam());
//    }
//        break;
//    }
//    //HandleManager::instance().AddObject(controller);
//    return controller;
}

ConnectionController *ConnectionController::CreateInstance(SocketHandler *socket, AbstractController *parent)
{
    AbstractController * ac = DeviceFactory::instance().CreateConnectionController(socket,parent);
    return dynamic_cast<ConnectionController * >(ac);

//    Device * parentDev = dynamic_cast<Device *>(parent);
//    if(parentDev == 0 && parent != 0){
//        qWarning("can not covert parent to QObject,parent must be a QObject");
//        return 0;
//    }

//    ConnectionController * controller = 0;
//    switch (parent->getType()){
//    case KE_DevType_DVR:
//    case KE_DevType_08Client:
//    case KE_IPC_Simulator:
//    {

//        ProtocalProcess *protocal =  new ProtocalProcess();
//        DevConnection * dev =  new DevConnection(socket,protocal,parentDev);
//        parentDev->AddChannel(dev);
//        controller = dynamic_cast<ConnectionController *>(dev);
//        controller->setType(parent->getType());
//        controller->SetNetParam(parent->GetNetParam());
//    }
//        break;
//    case KE_DevType_08Proxy:
//    {
//        ProtocalProcess * protocal =  new ProtocalProcess();
//        DevConnectSvr *newDev =  new DevConnectSvr(socket,protocal,parentDev);
//        //parentDev->AddChannel(newDev);
//        controller = dynamic_cast<ConnectionController *>(newDev);
//        controller->setType(parent->getType());
//        controller->SetNetParam(parent->GetNetParam());
//     }
//        break;

//    }
//    //HandleManager::instance().AddObject(controller);
//    return controller;
}

ConnectionController::ConnectionController()
{
    this->cbConnectStatus = 0;
    this->userConnectStatus = 0;
}
