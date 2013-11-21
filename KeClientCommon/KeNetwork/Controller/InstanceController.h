#ifndef INSTANCECONTROLLER_H
#define INSTANCECONTROLLER_H

#include "AbstractController.h"

#include "kenet_global.h"




//监听连接回调
typedef void (CALLBACK *fNewConnection)(int sID, int connectHandle, void * user);


class InstanceController:public AbstractController
{
public:
    InstanceController();
    virtual int ListenConnect(int port) = 0;
    virtual void SetNewConnectionCallBack(fNewConnection cbfunc, void * user) = 0;
public:
    static InstanceController *CreateInstance(int controlType);
};

#endif // INSTANCECONTROLLER_H
