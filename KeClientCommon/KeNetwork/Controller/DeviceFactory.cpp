#include "DeviceFactory.h"
#include "CommunicationThread.h"
#include "TcpDeviceHolder.h"
#include "DevConnection.h"
#include "SocketHandler.h"
#include "KeProtocal/ProtocalProcess.h"
#include "Simulator/ProtocalIPC.h"
#include "Simulator/DevVideoSvr.h"
#include "KeProxy/DevConnectSvr.h"

DeviceFactory::DeviceFactory(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&ChannelTread::instance());
    ChannelTread::instance().start();

    QObject::connect(this,&DeviceFactory::sbCreateInstanceController,
                     this,&DeviceFactory::sCreateInstanceController,
                     Qt::BlockingQueuedConnection);
    QObject::connect(this,&DeviceFactory::sbCreateConnectionController,
                     this,&DeviceFactory::sCreateConnectionController,
                     Qt::BlockingQueuedConnection);

}

DeviceFactory &DeviceFactory::instance()
{
    static DeviceFactory signal;
    return signal;
}

AbstractController *DeviceFactory::CreateInstanceController(int controlType)
{
    QMutexLocker locker(&resultMutex);
    emit sbCreateInstanceController(controlType);
    return this->currentControl;
}

AbstractController *DeviceFactory::CreateConnectionController(AbstractController *parent)
{
    QMutexLocker locker(&resultMutex);
    emit sbCreateConnectionController(parent,0);
    return this->currentControl;
}

AbstractController *DeviceFactory::CreateConnectionController(SocketHandler *socket, AbstractController *parent)
{
    QMutexLocker locker(&resultMutex);
    emit sbCreateConnectionController(parent,socket);
    return this->currentControl;

}

void DeviceFactory::sCreateInstanceController(int controlType)
{
    InstanceController * controller = 0;
    switch (controlType){
    case KE_DevType_DVR:
    case KE_DevType_08Client:
    case KE_IPC_Simulator:
    case KE_DevType_08Proxy:
    {
        controller =  new TcpDeviceHolder();
        controller->setType(controlType);
    }
        break;
    }

    this->currentControl = dynamic_cast<AbstractController *>(controller);
}

void DeviceFactory::sCreateConnectionController(AbstractController *parent, SocketHandler *socket)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0){
        qWarning("can not covert parent to QObject,parent must be a QObject");
        this->currentControl = 0;
    }
    SocketHandler * sh;
    if(socket != 0){
        sh = socket;
    }
    ConnectionController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    case KE_DevType_08Client:
    {
        ProtocalProcess *protocal =  new ProtocalProcess();
        if(socket == 0){
            sh = new SocketHandler();
            sh->CreateSocket();
        }
        controller =  new DevConnection(sh,protocal,parentDev);
        controller->setType(parent->getType());
        controller->SetNetParam(parent->GetNetParam());
    }
        break;
    case KE_IPC_Simulator:
    {
        ProtocalProcess *protocal =  new ProtocalIPC();
        if(socket == 0){
            sh = new SocketHandler();
            sh->CreateSocket();
        }
        DevSimulateIPC * dev =  new DevSimulateIPC(sh,protocal,parentDev);
        DevVideoSvr * subDev = new DevVideoSvr(dev);
        subDev->setChannelID(1);
        //QObject::connect(dev,&DevSimulateIPC::sendMedia,subDev,&DevVideoSvr::sendMedia);
        controller = dynamic_cast<SimulatorController *>(dev);
    }
        break;
    case KE_DevType_08Proxy:
    {
        ProtocalProcess *protocal =  new ProtocalProcess();
        if(socket == 0){
            sh = new SocketHandler();
            sh->CreateSocket();
        }
        controller =  new DevConnectSvr(sh,protocal,parentDev);
        controller->setType(parent->getType());
        controller->SetNetParam(parent->GetNetParam());
    }
        break;
    default:
        qWarning("DeviceFactory::sCreateConnectionController error type");
        break;
    }

    this->currentControl = dynamic_cast<AbstractController *>(controller);
}


