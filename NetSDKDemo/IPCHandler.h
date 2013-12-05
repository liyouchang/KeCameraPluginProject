#ifndef IPCHANDLER_H
#define IPCHANDLER_H



struct DeviceInstance{
    int devId;
    int * realDataHandles;
    int * recordFindHandles;
    int * recordFileHandles;
    int alarmHandle;
};
struct DeviceInstList{
    DeviceInstance * dev;
    DeviceInstance * next;
};

struct SDKInstance{
    int sID;
    DeviceInstList * devListHead;
};

class IPCHandler
{
public:
    IPCHandler();
    int sID;
    int devId;
    int realDataHandler;
    int recordFindHandle;
    int recordSaveHandle;
    int alarmHandle;

};

#endif // IPCHANDLER_H
