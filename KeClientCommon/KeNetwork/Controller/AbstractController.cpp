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


void AbstractController::setHandler(int handle)
{
    this->controlHandler = handle;
}

AbstractController::AbstractController()
{
    controlHandler = HandleManager::instance().AddObject(this);
}

AbstractController::~AbstractController()
{
    HandleManager::instance().RemoveObject(getHandler());
}

