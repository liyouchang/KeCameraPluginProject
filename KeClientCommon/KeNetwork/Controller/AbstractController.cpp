#include "AbstractController.h"

#include "HandleManager.h"
#include <QObject>
#include <QList>
#include "Channel.h"
int AbstractController::getHandler()
{
    if(controlHandler == 0)
        controlHandler = HandleManager::instance().GetObjectID(this);
    return controlHandler;
}

int AbstractController::getType()
{
    return this->type;
}

void AbstractController::setType(int type)
{
    this->type = type;
}

bool AbstractController::CheckAvaliable(int channelID)
{
    return true;
}

void AbstractController::SetNetParam(NET_PARAM param)
{
    this->netParam = param;
}

NET_PARAM AbstractController::GetNetParam()
{
    return this->netParam;
}


void AbstractController::setHandler(int handle)
{
    this->controlHandler = handle;
}

AbstractController::AbstractController()
{
    controlHandler = HandleManager::instance().AddObject(this);
    this->netParam.connectTimeout = 3000;
    this->netParam.waitTimeout = 3000;
    this->netParam.connectTryNum = 1;
    this->netParam.heartbeatIntervalTime = 10000;
}

AbstractController::~AbstractController()
{
    HandleManager::instance().RemoveObject(getHandler());
}

