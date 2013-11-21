#ifndef ALARMCONTROLLER_H
#define ALARMCONTROLLER_H

#include "AbstractController.h"

#include "kenet_global.h"


// 消息回调函数原形
typedef void (CALLBACK *fAlarmCallBack)(int alarmHandle,int alarmType, char *pBuf, int dwBufLen, void * user);

class AlarmController : public AbstractController
{
public:
    AlarmController();
    virtual void SetAlarmCallBack(fAlarmCallBack cbAlarm, void * user);
    static AlarmController *CreateInstance(AbstractController * parent);

protected:
    fAlarmCallBack cbAlarm;
    void * userAlarm;
};

#endif // ALARMCONTROLLER_H
