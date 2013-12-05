#include "RecordQueryController.h"
#include "KeProtocal/DevRecordQuery.h"
RecordQueryController::RecordQueryController()
{
}

RecordQueryController *RecordQueryController::CreateInstance(AbstractController *parent)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0){
        qWarning("parent cannot covert to class Device !");
        return 0;
    }
    RecordQueryController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    {
       controller =  new DevRecordQuery(parentDev);
    }
        break;
    case KE_DevType_08Client:
        break;
    default:
        qWarning("RecordQueryController::CreateInstance error type");
        break;
    }
    return controller;
}
