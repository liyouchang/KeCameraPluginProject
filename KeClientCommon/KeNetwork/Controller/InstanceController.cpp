#include "InstanceController.h"
#include "TcpDeviceHolder.h"
#include "HandleManager.h"
#include "DeviceFactory.h"
InstanceController::InstanceController()
{
    cbNewConnection = 0;
    userNewConnection = 0;
    this->cbSearchDev = 0;
    this->userSearchDev = 0;
}

void InstanceController::SetNewConnectionCallBack(fNewConnection cbfunc, void *user)
{
    this->cbNewConnection = cbfunc;
    this->userNewConnection = user;
}

void InstanceController::SetSearchDeviceCallBack(fSearchDeviceCallBack cbfunc, void *user)
{
    this->cbSearchDev = cbfunc;
    this->userSearchDev = user;
}


InstanceController *InstanceController::CreateInstance(int controlType)
{
    DeviceFactory & factory = DeviceFactory::instance();
    AbstractController * ac = factory.CreateInstanceController(controlType);
    return dynamic_cast<InstanceController * >(ac);
//    InstanceController * controller = 0;
//    switch (controlType){
//    case KE_DevType_DVR:
//    case KE_DevType_08Client:
//    case KE_IPC_Simulator:
//    case KE_DevType_08Proxy:
//    {
//        controller =  new TcpDeviceHolder();
//        controller->setType(controlType);
//    }
//        break;
//    }
//    //HandleManager::instance().AddObject(controller);
//    return controller;

}
