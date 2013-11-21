#include "HandleManager.h"
#include <QMutex>
#include <QMutexLocker>
#include <QAtomicInt>
#include "device.h"
static QAtomicInt handleNum;
HandleManager::HandleManager()
{
    handleNum.storeRelease(1);
}


HandleManager &HandleManager::instance()
{
    static HandleManager manager;
    return manager;
}

AbstractController *HandleManager::GetObject(int id)
{
    return handleMap[id];
}

int HandleManager::GetDevType(int id)
{
    int type = 0;
    AbstractController * obj =  handleMap[id];
    Device * dev = dynamic_cast<Device *>(obj);
    if(dev != 0){
        //type = dev->getType();
    }
    return type;
}

int HandleManager::GetObjectID(void *obj)
{
    std::map<int,AbstractController*>::iterator iter;
    for (iter = handleMap.begin(); iter != handleMap.end(); iter++)
    {
        if(iter->second == obj)
            return iter->first;
    }
    return 0;

}

int HandleManager::AddObject(AbstractController *obj)
{
    int id =  handleNum.fetchAndAddAcquire(1);
    handleMap[id] = obj;
    return id;
}

void HandleManager::RemoveObject(int id)
{
    handleMap.erase(id);
}
