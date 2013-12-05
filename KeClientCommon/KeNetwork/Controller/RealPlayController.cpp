#include "RealPlayController.h"

#include <QAbstractSocket>
#include "device.h"
#include "KeProtocal/DevRealPlay.h"
#include "HandleManager.h"
#include "SocketHandler.h"
RealPlayController *RealPlayController::CreateInstance( AbstractController *parent)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0){
        qWarning("parent cannot covert to class Device !");
        return 0;
    }
    RealPlayController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    case KE_DevType_08Client:
    case KE_DevType_08Proxy:
    {
        controller =  new DevRealPlay(parentDev);
        controller->setType(parent->getType());
        controller->SetNetParam(parent->GetNetParam());
    }
        break;
    default:
        qWarning("RealPlayController::CreateInstance type error");
        break;
    }
    // HandleManager::instance().AddObject(controller);

    return controller;
}
