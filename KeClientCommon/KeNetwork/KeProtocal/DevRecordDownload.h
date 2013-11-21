#ifndef DEVRECORDDOWNLOAD_H
#define DEVRECORDDOWNLOAD_H

#include <QObject>
#include "Device.h"
#include "Controller/RecordDownloadController.h"
#include <QFile>
class DevRecordDownload :public Device,public RecordDownloadController
{
    Q_OBJECT
public:
    explicit DevRecordDownload(Device *parent);
    ~DevRecordDownload();
signals:
    void downloadPos(int totalSize,int downloadSize);
public slots:
    //if newSize is -1 ,the download is end
    void UpdataDownload(int newSize);

    void OnRespond(QByteArray &msgData);

public:
    int DownloadRecordFile(NET_RECORDFILE_INFO recordFile, const char *sSavedFileName);
    int GetDownloadPos(int *nTotalSize, int *nDownLoadSize);
    int StopDownload();

    int Request();
    bool CheckAvaliable(int channelID);

public:
    NET_RECORDFILE_INFO recordFileInfo;
    int recordError;
    int downloadedSize;
    QFile * recordFile;

};

#endif // DEVRECORDDOWNLOAD_H
