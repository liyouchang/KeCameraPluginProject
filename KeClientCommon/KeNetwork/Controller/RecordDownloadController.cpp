#include "RecordDownloadController.h"
#include "KeProtocal/DevRecordDownload.h"
RecordDownloadController::RecordDownloadController()
{
    SetDownloadPosCallBack(0,0);
}

void RecordDownloadController::SetDownloadPosCallBack(fDownLoadPosCallBack cbfunc, void *user)
{
    this->cbDownloadPos = cbfunc;
    this->userDownloadPos = user;
}

RecordDownloadController *RecordDownloadController::CreateInstance(AbstractController *parent)
{
    Device * parentDev = dynamic_cast<Device *>(parent);
    if(parentDev == 0){
        qWarning("parent cannot covert to class Device !");
        return 0;
    }
    RecordDownloadController * controller = 0;
    switch (parent->getType()){
    case KE_DevType_DVR:
    {
       controller =  new DevRecordDownload(parentDev);
    }
        break;
    case KE_DevType_08Client:
        break;
    }


    return controller;
}
