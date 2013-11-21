#include "InstanceController.h"
#include "TcpDeviceHolder.h"
#include "HandleManager.h"
InstanceController::InstanceController()
{
}

InstanceController *InstanceController::CreateInstance(int controlType)
{

    InstanceController * controller = 0;
    switch (controlType){
    case KE_DevType_DVR:
    case KE_DevType_08SERVER:
    case KE_IPC_Simulator:
    {
        controller =  new TcpDeviceHolder();
    }
        break;
    }
    controller->setType(controlType);
    //HandleManager::instance().AddObject(controller);
    return controller;

}
