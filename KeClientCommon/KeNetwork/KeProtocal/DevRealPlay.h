#ifndef REALPLAYDEVICE_H
#define REALPLAYDEVICE_H

#include <QObject>
#include "Device.h"
#include"Controller/RealPlayController.h"
#include "ChRealData.h"

enum MediaType
{
    Media_None = 0x00,
    Media_Vedio = 0x01,
    Media_Listen = 0x02,
    Media_Talk = 0x04,
    Media_ALL = 7
};

class DevRealPlay : public Device ,public RealPlayController
{
    Q_OBJECT
public:
    explicit DevRealPlay(Device *parent);

signals:
    void recvMediaData(int cameraID,int dataType,QByteArray data);
public slots:
    void MediaDataRecv(int cameraID,int dataType,QByteArray data);
    // RealPlayController interface
public:
    int StartRealPlay(int m_channelID);
    int StopRealPlay();
    void SetRealPlayDisConnect(fRealPlayDisConnect cbRealDisConnect, void *user);
    void SetRealDataCallBack(fRealDataCallBack cbRealData, void *user);
    int SaveRealData(const char *fileName);
    int StopSaveRealData();
    int StartSound();
    int StopSound();
    int StartTalk();
    int TalkSendData(const char *pSendBuf, int dwBufSize);
    int StopTalk();
    int PTZControl(int PTZCommand, int step, int stop);

protected:

    fRealPlayDisConnect cbRealDisConnect;
    void * userRealDisConnect;
    fRealDataCallBack cbRealData;
    void * userRealData;

    // Channel interface
public:
    static QString CreateNameByParam(int m_channelID);
    QString createName();
    ChRealData * chRealData;
    int m_mediaType;

    // Channel interface
public:
    void OnRespond(QByteArray & data);
    int Request();
    bool CheckAvaliable(int channelID);
};

#endif // REALPLAYDEVICE_H
