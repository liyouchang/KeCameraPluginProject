#ifndef INSTANCECONTROLLER_H
#define INSTANCECONTROLLER_H

#include "AbstractController.h"





//监听连接回调
typedef void (CALLBACK *fNewConnection)(int sID, int connectHandle,char * param,int paramLen, void * user);
//searched device callback
typedef void (CALLBACK * fSearchDeviceCallBack)(int sID,int index,NET_DEVICEINFO *devInfo,void *pUser);


class InstanceController:public AbstractController
{
public:
    InstanceController();
    virtual int ListenConnect(int port) = 0;
    virtual void SetNewConnectionCallBack(fNewConnection cbfunc, void * user);
    virtual void SetSearchDeviceCallBack(fSearchDeviceCallBack cbfunc,void * user);
    virtual int SearchDevice() = 0;
public:
    static InstanceController *CreateInstance(int controlType);
    fSearchDeviceCallBack cbSearchDev;
    void * userSearchDev;
    fNewConnection cbNewConnection;
    void * userNewConnection;
};

#endif // INSTANCECONTROLLER_H
