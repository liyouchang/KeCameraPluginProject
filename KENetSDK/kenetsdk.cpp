#include "kenetsdk.h"
#include "Controller/InstanceController.h"
#include "Controller/ConnectionController.h"
#include "Controller/RealPlayController.h"
#include "Controller/RecordQueryController.h"
#include "Controller/RecordDownloadController.h"
#include "Controller/AlarmController.h"
#include "HandleManager.h"
#include <QtGlobal>
#include <QCoreApplication>

int KENet_CreateInstance(int type)
{
    static int argc=0;
    static char **argv={ 0 };
    if(!qApp){
        qDebug("new QCoreApplication");
        (void)new QCoreApplication(argc, argv);
    }
    InstanceController * inst = InstanceController::CreateInstance(type);
    if(inst == 0){
        return KE_CreateInstance_Error;
    }
    return inst->getHandler();
}


void KENet_ReleaseInstance(int sID)
{
    HandleManager::instance().RemoveObject(sID);
}

static const int sdkVersion = 1000;
int KENet_GetSDKVersion()
{
    return sdkVersion;
}

int KENet_Connect(int sID, const char *addr, int port, fConnectStatusCallBack cbConnectStatus, void *user)
{
    InstanceController * parent = dynamic_cast<InstanceController *>(HandleManager::instance().GetObject(sID));
    if(parent == 0){
        return KE_Wrong_Control;
    }
    ConnectionController * connect = ConnectionController::CreateInstance(parent);
    if(connect == 0){
        return KE_No_Initial;
    }
    connect->SetConnectStatusCallBack(cbConnectStatus,user);
    int ret = connect->ConnectDevice(addr,port);
    if(ret == 0){
        return connect->getHandler();
    }
    else{
        return ret;
    }

}

//int KENet_SetAutoReconnect(int connectHandle, fHaveReConnect cbAutoConnect, void *user)
//{
//    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
//    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
//    if(connect == 0)
//        return KE_Wrong_Control;

//    connect->SetAutoReconnectCallBack(cbAutoConnect,user);
//    return KE_SUCCESS;
//}


int KENet_StartListen(int sID, int port,fNewConnection newConnect, void *user)
{
    AbstractController * controller = HandleManager::instance().GetObject(sID);
    InstanceController * inst = dynamic_cast<InstanceController *>(controller);
    if(inst == 0){
        return KE_Wrong_Control;
    }
    inst->SetNewConnectionCallBack(newConnect,user);
    return inst->ListenConnect(port);
}


int KENet_DisConnect(int connectHandle)
{
    ConnectionController * connect = dynamic_cast<ConnectionController *>(HandleManager::instance().GetObject(connectHandle));
    if(connect == 0)
        return KE_Wrong_Control;

    return connect->DisConnect();
}


//int KENet_LoginServer(int connectHandle, const char *userName, const char *password, fDeviceTreeCallBack callBackFunc, void *user)
//{
//    return KE_Not_Support_Function;
//}


int KENet_Logout(int connectHandle)
{
    return KE_Not_Support_Function;
}


int KENet_StartRealPlay(int connectHandle, int channelID,
                        fRealDataCallBack cbRealData, fRealPlayDisConnect cbDisconnect,
                        void *dwUser, int dwWaitTime)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0)    return KE_Wrong_Control;

    RealPlayController * realPlay = RealPlayController::CreateInstance(connect);
    if(!realPlay->CheckAvaliable(channelID)){
        delete realPlay;
        return KE_Channel_NotFree;
    }
    realPlay->SetTimeout(dwWaitTime);
    realPlay->SetRealDataCallBack(cbRealData,dwUser);
    realPlay->SetRealPlayDisConnect(cbDisconnect,dwUser);

    int ret = realPlay->StartRealPlay(channelID);
    if(ret != 0){
        realPlay->StopRealPlay();
        return ret;
    }
    return realPlay->getHandler();
}


int KENet_StopRealPlay(int realHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->StopRealPlay();
}


int KENet_SaveRealData(int realHandle, const char *fileName)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->SaveRealData(fileName);

}


int KENet_StopSaveRealData(int realHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->StopSaveRealData();
}


int KENet_StartSound(int realHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->StartSound();
}


int KENet_StopSound(int realHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->StopSound();
}


int KENet_StartTalk(int realHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->StartTalk();
}


int KENet_StopTalk(int realHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->StopTalk();
}


int KENet_TalkSendData(int realHandle, const char *pSendBuf, int dwBufSize)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->TalkSendData(pSendBuf,dwBufSize);
}


int KENet_PTZControl(int realHandle, int PTZCommand, int step, int stop)
{
    AbstractController * controller = HandleManager::instance().GetObject(realHandle);
    RealPlayController * realPlay = dynamic_cast<RealPlayController *>(controller);
    if(realPlay == 0)    return KE_Wrong_Control;

    return realPlay->PTZControl(PTZCommand,step,stop);
}


int KENet_QueryRecordFile(int connectHandle, int channelID, int recordFileType,
                          NET_TIME tmStart, NET_TIME tmEnd,
                          NET_RECORDFILE_INFO *nriFileinfo, int maxlen,
                          int *filecount, int waittime)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0)    return KE_Wrong_Control;
    RecordQueryController * record = RecordQueryController::CreateInstance(connect);
    if(!record->CheckAvaliable(channelID)){
        delete record;
        return KE_Channel_NotFree;
    }
    record->SetQueryTime(tmStart,tmEnd);
    record->SetQueryType(recordFileType);
    record->SetTimeout(waittime);
    std::vector<NET_RECORDFILE_INFO> recordFileInfo;
    int ret = record->QueryRecordFile(channelID,recordFileInfo);
    if(ret != 0){
        delete record;
        return ret;
    }
    int count = qMin<int>(recordFileInfo.size(),maxlen);
    *filecount = count;
    for(int i =0;i<count;i++){
        nriFileinfo[i] = recordFileInfo[i];
    }
    delete record;
    return KE_SUCCESS;
}


int KENet_QuickQueryRecordFile(int connectHandle, int channelID, int recordFileType,
                               NET_TIME tmStart, NET_TIME tmEnd,
                               NET_RECORDFILE_INFO *nriFileinfo, int maxlen,
                               int *filecount, int waittime)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0)    return KE_Wrong_Control;
    RecordQueryController * record = RecordQueryController::CreateInstance(connect);
    if(!record->CheckAvaliable(channelID)){
        delete record;
        return KE_Channel_NotFree;
    }
    record->SetQueryTime(tmStart,tmEnd);
    record->SetQueryType(recordFileType);
    record->SetTimeout(waittime);
    std::vector<NET_RECORDFILE_INFO> recordFileInfo;
    int ret = record->QuickQueryRecordFile(channelID,recordFileInfo);
    if(ret != 0){
        delete record;
        return ret;
    }
    int count = qMin<int>(recordFileInfo.size(),maxlen);
    *filecount = count;
    for(int i =0;i<count;i++){
        nriFileinfo[i] = recordFileInfo[i];
    }
    delete record;
    return KE_SUCCESS;
}
int KENet_FindFile(int connectHandle, int channelID, int nRecordFileType,
                   NET_TIME time_start, NET_TIME time_end, int waittime)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0)    {
        return KE_Wrong_Control;
    }
    RecordQueryController * record = RecordQueryController::CreateInstance(connect);
    if(!record->CheckAvaliable(channelID)){
        delete record;
        return KE_Channel_NotFree;
    }
    record->SetQueryTime(time_start,time_end);
    record->SetQueryType(nRecordFileType);
    record->SetTimeout(waittime);
    int ret = record->FindStart(channelID);
    if(ret != 0 ) {
        return ret ;
    }
    return record->getHandler();
}

int KENet_FindNextFile(int findHandle, NET_RECORDFILE_INFO *lpFindData)
{
    AbstractController * controller = HandleManager::instance().GetObject(findHandle);
    RecordQueryController * record = dynamic_cast<RecordQueryController *>(controller);
    if(record == 0)    return KE_Wrong_Control;

    return record->FindNext(lpFindData);
}

int KENet_FindClose(int findHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(findHandle);
    RecordQueryController * record = dynamic_cast<RecordQueryController *>(controller);
    if(record == 0)    return KE_Wrong_Control;

    return record->FindClose();
}


int KENet_DownloadByRecordFile(int connectHandle, NET_RECORDFILE_INFO recordFile,
                               const char *sSavedFileName, fDownLoadPosCallBack cbDownLoadPos,
                               void *dwUserData)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0)    return KE_Wrong_Control;

    RecordDownloadController * download = RecordDownloadController::CreateInstance(connect);
    if(!download->CheckAvaliable(recordFile.ch)){
        delete download;
        return KE_Channel_NotFree;
    }
    download->SetDownloadPosCallBack(cbDownLoadPos,dwUserData);
    int ret = download->DownloadRecordFile(recordFile,sSavedFileName);
    if(ret != 0){
        download->StopDownload();
        return ret;
    }

    return download->getHandler();
}


int KENet_GetDownloadPos(int recordHandle, int *nTotalSize, int *nDownLoadSize)
{
    AbstractController * controller = HandleManager::instance().GetObject(recordHandle);
    RecordDownloadController * download = dynamic_cast<RecordDownloadController *>(controller);
    if(download == 0)    return KE_Wrong_Control;

    return download->GetDownloadPos(nTotalSize,nDownLoadSize);
}


int KENet_StopDownload(int recordHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(recordHandle);
    RecordDownloadController * download = dynamic_cast<RecordDownloadController *>(controller);
    if(download == 0)    return KE_Wrong_Control;

    return download->StopDownload();
}


int KENet_StartAlarm(int connectHandle, fAlarmCallBack cbMessage, void *dwUser)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0)    return KE_Wrong_Control;

    AlarmController * alarm = AlarmController::CreateInstance(connect);
    if(alarm == 0){
        return KE_CreateInstance_Error;
    }

    if(!alarm->CheckAvaliable(1)){
        delete alarm;
        return KE_Channel_NotFree;
    }
    alarm->SetAlarmCallBack(cbMessage,dwUser);

    return alarm->getHandler();
}


int KENet_StopAlarm(int alarmHandle)
{
    AbstractController * controller = HandleManager::instance().GetObject(alarmHandle);
    AlarmController * alarm = dynamic_cast<AlarmController *>(controller);
    if(alarm == 0)    return KE_Wrong_Control;

    delete alarm;
    return KE_SUCCESS;
}


int KENet_StartSearchDevice(int sID, fSearchDeviceCallBack cbfunc, void *user)
{
    AbstractController * controller = HandleManager::instance().GetObject(sID);
    InstanceController * inst = dynamic_cast<InstanceController *>(controller);
    if(inst == 0){
        return KE_Wrong_Control;
    }
    inst->SetSearchDeviceCallBack(cbfunc,user);
    return inst->SearchDevice();
}


int KENet_StopSearchDevice(int sID)
{
    AbstractController * controller = HandleManager::instance().GetObject(sID);
    InstanceController * inst = dynamic_cast<InstanceController *>(controller);
    if(inst == 0){
        return KE_Wrong_Control;
    }
    inst->SetSearchDeviceCallBack(0,0);
    return KE_SUCCESS;

}


int KENet_LoginDevice(int connectHandle, const char *userName, const char *password, NET_DEVICEINFO *info)
{
    AbstractController * controller = HandleManager::instance().GetObject(connectHandle);
    ConnectionController * connect = dynamic_cast<ConnectionController *>(controller);
    if(connect == 0){
        return KE_Wrong_Control;
    }

    int ret =connect->LoginServer(userName,password);
    if(ret != KE_SUCCESS)
        return ret;
    connect->GetDeviceInfo(info);
    return KE_SUCCESS;
}


int KENet_TryConnect(int sID, const char *addr, int port, fConnectStatusCallBack cbConnectStatus, void *user)
{
    InstanceController * parent = dynamic_cast<InstanceController *>(HandleManager::instance().GetObject(sID));
    if(parent == 0){
        return KE_Wrong_Control;
    }
    ConnectionController * connect = ConnectionController::CreateInstance(parent);
    connect->SetConnectStatusCallBack(cbConnectStatus,user);
    int ret = connect->TryConnect(addr,port);
    if(ret == 0){
        return connect->getHandler();
    }
    else{
        return ret;
    }
}
