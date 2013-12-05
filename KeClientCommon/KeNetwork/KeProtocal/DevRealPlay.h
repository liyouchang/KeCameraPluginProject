#ifndef REALPLAYDEVICE_H
#define REALPLAYDEVICE_H

#include <QObject>
#include "Device.h"
#include"Controller/RealPlayController.h"
#include "ChRealData.h"
#include "ChAskVideo.h"

class DevRealPlay : public Device ,public RealPlayController
{
    Q_OBJECT
public:
    explicit DevRealPlay(Device *parent);
    virtual ~DevRealPlay();
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
    void SetTimeout(int time);
protected:
    fRealPlayDisConnect cbRealDisConnect;
    void * userRealDisConnect;
    fRealDataCallBack cbRealData;
    void * userRealData;
    int m_realTimeout;
    // Channel interface
public:
    static QString CreateNameByParam(int m_channelID);
    QString createName();
    ChRealData * chRealData;
    ChAskVideo * chVideo;
    //int m_mediaType;
    SocketHandler * m_videoSocket;
    // Channel interface
public:
    void OnRespond(QByteArray & data);
    int Request();
    bool CheckAvaliable(int channelID);
};

#endif // REALPLAYDEVICE_H
