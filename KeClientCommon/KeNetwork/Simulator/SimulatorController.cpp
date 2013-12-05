#include "SimulatorController.h"
#include "ProtocalIPC.h"
#include "DevSimulateIPC.h"
#include "SocketHandler.h"
#include "HandleManager.h"
#include "DevVideoSvr.h"
SimulatorController::SimulatorController()
{
    //cbReconnect = 0;
    //cbDisConnect = 0;
   // userDisConnect =0;
    //userReconnect = 0;
}

SimulatorController *SimulatorController::CreateInstance(AbstractController *parent)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0){
        qWarning("can not covert parent to QObject,parent must be a QObject");
        return 0;
    }

    SimulatorController * controller = 0;
    switch (parent->getType()){
    case  KE_IPC_Simulator:
    {
        ProtocalProcess *protocal =  new ProtocalIPC();
        SocketHandler * sh = new SocketHandler();
        sh->CreateSocket();
        DevSimulateIPC * dev =  new DevSimulateIPC(sh,protocal,parentDev);
        DevVideoSvr * subDev = new DevVideoSvr(dev);
        //QObject::connect(dev,&DevSimulateIPC::sendMedia,subDev,&DevVideoSvr::sendMedia);
        controller = dynamic_cast<SimulatorController *>(dev);

    }
        break;
    }
    HandleManager::instance().AddObject(controller);
    return controller;
}

SimulatorController *SimulatorController::CreateInstance(SocketHandler *socket, AbstractController *parent)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0 && parent != 0){
        qWarning("can not covert parent to QObject,parent must be a QObject");
        return 0;
    }

    SimulatorController * controller = 0;
    switch (parent->getType()){
    case  KE_IPC_Simulator:
    {
        ProtocalProcess *protocal =  new ProtocalIPC();
        DevSimulateIPC * dev =  new DevSimulateIPC(socket,protocal,parentDev);
        DevVideoSvr * subDev = new DevVideoSvr(dev);
        controller = dynamic_cast<SimulatorController *>(dev);
    }
        break;
    }
    HandleManager::instance().AddObject(controller);
    return controller;
}

