#include "AlarmController.h"
#include "Device.h"
#include "KeProtocal/DevAlarm.h"
AlarmController::AlarmController()
{
    SetAlarmCallBack(0,0);
}

void AlarmController::SetAlarmCallBack(fAlarmCallBack cbAlarm, void *user)
{
    this->cbAlarm = cbAlarm;
    this->userAlarm = user;
}

AlarmController *AlarmController::CreateInstance(AbstractController *parent)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0){
        qWarning("parent cannot covert to class Device !");
        return 0;
    }
    AlarmController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    {
       controller =  new DevAlarm(parentDev);
    }
        break;
    case KE_DevType_08SERVER:
        break;
    }


    return controller;

}
