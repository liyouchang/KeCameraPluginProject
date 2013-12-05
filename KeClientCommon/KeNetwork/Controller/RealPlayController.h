#ifndef REALPLAYCONTROLLER_H
#define REALPLAYCONTROLLER_H

#include "kenet_global.h"
#include "AbstractController.h"

// 视频监视断开回调函数原形,
typedef void (CALLBACK *fRealPlayDisConnect)(int realHandle, EM_REALPLAY_DISCONNECT_EVENT_TYPE eventType, void* param, void * user);

//数据回调函数原形
typedef void (CALLBACK *fRealDataCallBack)(int realHandle, int dataType,const char *buffer, int bufSize, void * user);

class RealPlayController:public AbstractController
{
public:
    virtual int StartRealPlay(int channelID) = 0;
    virtual int StopRealPlay() = 0;

    virtual void SetRealPlayDisConnect(fRealPlayDisConnect cbRealDisConnect, void * user) = 0;
    virtual void SetRealDataCallBack(fRealDataCallBack  ,void *user) = 0;

    virtual int SaveRealData(const char *fileName) = 0;
    virtual int StopSaveRealData() = 0;

    virtual int StartSound() = 0;
    virtual int StopSound() = 0;

    virtual int StartTalk() = 0;
    virtual int TalkSendData(const char *pSendBuf, int dwBufSize) = 0;
    virtual int StopTalk() = 0;
    virtual int PTZControl(int PTZCommand, int step, int stop) = 0;

    virtual void SetTimeout(int time) = 0;
    static RealPlayController *CreateInstance(AbstractController * parent);
};

#endif // REALPLAYCONTROLLER_H
