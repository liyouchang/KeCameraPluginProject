#ifndef DEVALARM_H
#define DEVALARM_H

#include <QObject>
#include "Device.h"
#include"Controller/AlarmController.h"
#include "kenet_global.h"

class DevAlarm : public Device,public AlarmController
{
    Q_OBJECT
public:
    explicit DevAlarm(Device *parent);
    int alarmType;
    KEAlarmInfo alarmInfo;
signals:
    void toDoRespond(int alarmType,QByteArray info);
public slots:
    void OnRespond(QByteArray &msgData);
    void DoRespond(int alarmType,QByteArray info);
public:
    int Request();
    bool CheckAvaliable(int channelID);

};

#endif // DEVALARM_H
