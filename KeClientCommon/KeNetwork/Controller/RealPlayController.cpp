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
    case KE_DevType_08SERVER:
    {
       controller =  new DevRealPlay(parentDev);
    }
        break;
    }
   // HandleManager::instance().AddObject(controller);

    return controller;
}
