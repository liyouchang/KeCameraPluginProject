#ifndef DEVSIMULATEIPC_H
#define DEVSIMULATEIPC_H

#include "SimulatorController.h"
#include "Device.h"
#include <QByteArray>
#include "DevConnection.h"
#include "ChDeviceLogin.h"
#include "ChDeviceKey.h"
class DevSimulateIPC :public Device, public SimulatorController
{
    Q_OBJECT
public:
    explicit DevSimulateIPC(SocketHandler *s, Device *parent);
    DevSimulateIPC(SocketHandler * s, ProtocalProcess *protocal, Device *parent);
    ~DevSimulateIPC();
signals:
public slots:
    // SimulatorController interface
    void SendMediaData(int channelNo,const char *data, int len);
    void GetMessageData(QByteArray & allBytes);
    virtual void DevDisconnect();
    virtual void DevConnected();

    // Channel interface
public:
    void OnRespond(QByteArray &msgData);
    int Request();

public:
    QByteArray mediaData;
    int respChannelNo;
    QString m_mac;
    ChDeviceLogin * m_chLogin;
    ChDeviceKey * m_chKey;
    // ConnectionController interface
public:
    int LoginServer(std::string username, std::string pwd);
    int Logout();
    int SetMac(std::string mac);
    int ConnectDevice(const std::string &addr, int port);
    int DisConnect();
    int GetDeviceInfo(NET_DEVICEINFO *info);

protected:
    void setAllSubDeviceID();
};

#endif // DEVSIMULATEIPC_H
