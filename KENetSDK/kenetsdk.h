#ifndef KENETSDK_H
#define KENETSDK_H
#if (defined(WIN32) || defined(WIN64))

#if defined(KENETSDK_LIBRARY)
#  define KENETSDKSHARED_EXPORT  __declspec(dllexport)
#else
#  define KENETSDKSHARED_EXPORT __declspec(dllimport)
#endif
#define CALLBACK __stdcall
#define CALL_METHOD  __stdcall  //__cdecl
#else


#endif//win32


#include "kenet_global.h"


#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
 ** 回调函数定义
 ***********************************************************************/

//监听连接回调
typedef void (CALLBACK *fNewConnection)(int sID, int connectHandle,char * param,int paramLen, void * user);

// 网络连接断开回调函数原形
//typedef void (CALLBACK *fDisConnect)(int connectHandle, char *addr, int port, void * user);

// 网络连接状态回调函数原形
typedef void (CALLBACK *fConnectStatusCallBack)(int connectHandle, int status, void * user);

// 网络连接恢复回调函数原形
//typedef void (CALLBACK *fHaveReConnect)(int connectHandle, char *addr, int port, void * user);

// 视频监视断开回调函数原形,
typedef void (CALLBACK *fRealPlayDisConnect)(int realHandle, EM_REALPLAY_DISCONNECT_EVENT_TYPE eventType, void* param, void * user);

//数据回调函数原形
typedef void (CALLBACK *fRealDataCallBack)(int realHandle, int dataType, const char *buffer, int bufSize, void * user);

// 回放数据回调函数原形
typedef void (CALLBACK *fRecordDataCallBack)(int recordHandle, int dataType,const char *buffer, int bufSize, void * user);

// 回放进度回调函数原形,totalSize == downloadSize means the download is end
typedef void (CALLBACK *fDownLoadPosCallBack)(int recordHandle, int totalSize, int downLoadSize, void * user);

// 消息回调函数原形
typedef void (CALLBACK *fAlarmCallBack)(int alarmHandle,int alarmType, char *pBuf, int dwBufLen, void * user);

//设备树回调
typedef void (CALLBACK *fDeviceTreeCallBack)(int sID, void * user);

//
typedef void (CALLBACK * fSearchDeviceCallBack)(int sID,int index,NET_DEVICEINFO * devInfo,void *pUser);

/************************************************************************
 ** 接口定义
 ***********************************************************************/

// SDK初始化,return instance id -- sID
KENETSDKSHARED_EXPORT int KENet_CreateInstance(int type);
// SDK退出清理
KENETSDKSHARED_EXPORT void KENet_ReleaseInstance(int sID);

// 获取SDK的版本信息
KENETSDKSHARED_EXPORT int KENet_GetSDKVersion();

// 设置登陆网络环境
KENETSDKSHARED_EXPORT void KENet_SetNetworkParam(int sID,NET_PARAM inNetParam);
KENETSDKSHARED_EXPORT void KENet_GetNetworkParam(int sID,NET_PARAM *outNetParam);

// seach ipc or other video device
KENETSDKSHARED_EXPORT int KENet_StartSearchDevice(int sID,fSearchDeviceCallBack  cbfunc,void * user);
KENETSDKSHARED_EXPORT int KENet_StopSearchDevice(int sID);

//开始监听连接,用于设备主动连接的情况
KENETSDKSHARED_EXPORT int KENet_StartListen(int sID,int port,fNewConnection newConnect, void * user);
//deviceMac can be seperate by '|' to add or remove more than one device
KENETSDKSHARED_EXPORT int KENet_AddAllowLoginDevice(int sID,const char * deviceMac);
KENETSDKSHARED_EXPORT int KENet_RemoveAllowLoginDevice(int sID,const char * deviceMac);

//asyncronize connect device,return connect handle
KENETSDKSHARED_EXPORT int KENet_TryConnect(
        int sID,const char * addr,int port,fConnectStatusCallBack cbConnectStatus, void * user);

//链接设备,return connect handle.
KENETSDKSHARED_EXPORT int KENet_Connect(
        int sID,const char * addr,int port,fConnectStatusCallBack cbConnectStatus, void * user);

// 设置断线重连成功回调函数，设置后SDK内部断线自动重连,autoReconnect
KENETSDKSHARED_EXPORT int KENet_SetAutoReconnect(int connectHandle,int autoReconnect);

KENETSDKSHARED_EXPORT int KENet_DisConnect(int connectHandle);

// 向设备注册
//KENETSDKSHARED_EXPORT int KENet_LoginServer(
//        int connectHandle,const char *userName,const char *password,
//        fDeviceTreeCallBack callBackFunc,void *user);

KENETSDKSHARED_EXPORT int KENet_LoginDevice(
        int connectHandle,const char *userName,const char *password,
        NET_DEVICEINFO * info);

// 向设备注销
KENETSDKSHARED_EXPORT int KENet_Logout(int connectHandle);

// 开始实时监视,return realHandle
KENETSDKSHARED_EXPORT int KENet_StartRealPlay(
        int connectHandle, int channelID,fRealDataCallBack cbRealData,
        fRealPlayDisConnect cbDisconnect, void * dwUser, int dwWaitTime = 3000);

// 设置实时监视数据回调
//KENETSDKSHARED_EXPORT int CLIENT_SetRealDataCallBack(
//        int realHandle, fRealDataCallBack cbRealData, void * dwUser);

// stop real play video
KENETSDKSHARED_EXPORT int KENet_StopRealPlay(int realHandle);

// 保存数据为文件
KENETSDKSHARED_EXPORT int KENet_SaveRealData(int realHandle,const char *fileName);
// 结束保存数据为文件
KENETSDKSHARED_EXPORT int KENet_StopSaveRealData(int realHandle);
// 打开声音
KENETSDKSHARED_EXPORT int KENet_StartSound(int realHandle);
// 关闭声音
KENETSDKSHARED_EXPORT int KENet_StopSound(int realHandle);
// 打开对讲
KENETSDKSHARED_EXPORT int KENet_StartTalk(int realHandle);
//
KENETSDKSHARED_EXPORT int KENet_TalkSendData(int realHandle,const char *pSendBuf, int dwBufSize);
// 停止语音对讲
KENETSDKSHARED_EXPORT int KENet_StopTalk(int realHandle);

// 通用云台控制
KENETSDKSHARED_EXPORT int KENet_PTZControl(
        int realHandle, int PTZCommand, int step, int stop);

//------------------------------------------------------------------------

// 查询时间段内的所有录像文件
KENETSDKSHARED_EXPORT int KENet_QueryRecordFile(
        int connectHandle,int channelID, int recordFileType,
        NET_TIME  tmStart, NET_TIME  tmEnd,
        NET_RECORDFILE_INFO * nriFileinfo, int maxlen, int *filecount,
        int waittime=3000);

// 快速查询时间段内的录像文件，如果时间段内录像超过16条时，只返回时间最早的16段录像。
KENETSDKSHARED_EXPORT int KENet_QuickQueryRecordFile(
        int connectHandle, int channelID, int recordFileType,
        NET_TIME tmStart, NET_TIME tmEnd,
        NET_RECORDFILE_INFO * nriFileinfo,
        int maxlen, int *filecount, int waittime=3000);

// 开始查找录像文件 return findHandle
KENETSDKSHARED_EXPORT int KENet_FindFile(
        int connectHandle,int channelID, int nRecordFileType,
        NET_TIME  time_start, NET_TIME time_end,
        int waittime=3000);
// 查找录像文件
KENETSDKSHARED_EXPORT int KENet_FindNextFile(
        int findHandle,NET_RECORDFILE_INFO * lpFindData);
// 结束录像文件查找
KENETSDKSHARED_EXPORT int KENet_FindClose(int findHandle);

// 下载录像文件,return recordHandle
KENETSDKSHARED_EXPORT int  KENet_DownloadByRecordFile(
        int connectHandle,NET_RECORDFILE_INFO recordFile,const char *sSavedFileName,
        fDownLoadPosCallBack cbDownLoadPos,void * dwUserData);

// 查询录像下载进度
KENETSDKSHARED_EXPORT int KENet_GetDownloadPos(
        int recordHandle, int *nTotalSize, int *nDownLoadSize);

// 停止录像下载
KENETSDKSHARED_EXPORT int KENet_StopDownload(int recordHandle);


//------------------------------------------------------------------------
// 向设备订阅报警,return alarmHandle
KENETSDKSHARED_EXPORT int KENet_StartAlarm(int connectHandle,fAlarmCallBack cbMessage,void * dwUser);

// 停止订阅报警
KENETSDKSHARED_EXPORT int KENet_StopAlarm(int alarmHandle);


#ifdef __cplusplus
}
#endif
#endif // KENETSDK_H
