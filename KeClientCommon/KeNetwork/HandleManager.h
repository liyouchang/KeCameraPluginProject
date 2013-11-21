#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include <map>
#include "Controller/AbstractController.h"
class HandleManager
{
public:
    static HandleManager & instance();
    AbstractController * GetObject(int id);
    int GetDevType(int id);
    int GetObjectID(void * obj);
    int AddObject(AbstractController *obj);
    void RemoveObject(int id);

private:
    HandleManager();
    std::map<int,AbstractController *> handleMap;
};

#endif // HANDLEMANAGER_H
