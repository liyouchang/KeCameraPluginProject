#ifndef RECORDDOWNLOADCONTROLLER_H
#define RECORDDOWNLOADCONTROLLER_H


#include "AbstractController.h"
#include "kenet_global.h"


// 回放数据回调函数原形
typedef void (CALLBACK *fRecordDataCallBack)(int recordHandle, int dataType,const char *buffer, int bufSize, void * user);
// 回放进度回调函数原形
typedef void (CALLBACK *fDownLoadPosCallBack)(int recordHandle, int totalSize, int downLoadSize, void * user);

class RecordDownloadController : public AbstractController
{
public:
    RecordDownloadController();
    virtual int DownloadRecordFile(NET_RECORDFILE_INFO recordFile,const char *sSavedFileName) = 0;
    virtual int GetDownloadPos(int *nTotalSize, int *nDownLoadSize) = 0;
    virtual int StopDownload() = 0;
    virtual void SetDownloadPosCallBack(fDownLoadPosCallBack cbfunc,void * user);

    static RecordDownloadController *CreateInstance(AbstractController * parent);

protected:
    fDownLoadPosCallBack cbDownloadPos;
    void * userDownloadPos;
};

#endif // RECORDDOWNLOADCONTROLLER_H
