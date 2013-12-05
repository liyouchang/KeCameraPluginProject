#ifndef SIMULATORCONTROLLER_H
#define SIMULATORCONTROLLER_H

#include "Controller/AbstractController.h"
#include "Controller/ConnectionController.h"
#include <string>
class SimulatorController : public ConnectionController
{
public:
    SimulatorController();
    virtual void SendMediaData(int channelNo,const char * data,int len) = 0;
    virtual int SetMac(std::string mac) = 0;
    static SimulatorController *CreateInstance(AbstractController * parent);

    static SimulatorController *CreateInstance(SocketHandler * socket,AbstractController * parent);


};

#endif // SIMULATORCONTROLLER_H
