#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QObject>
#include <QString>
#include <QVariant>

enum UpdateStatus{
    UpdateStatus_None,
    UpdateStatus_Need,
    UpdateStatus_Ready,
    UpdateStatus_Doing,
    UpdateStatus_Done
};

class DeviceItem
{
public:
    explicit DeviceItem();
    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);
    bool setData(DeviceItem * other);
    void  *connect;
    QString mac;
    QString type;
    QString version;
    int updateStatus;
    int updatePercent;
    QString updateInfo;
    bool operator ==(const DeviceItem & item);
    QString getUpdateStatusInfo() const;
    bool setUpdateStatus(int status,int percent = 0);
signals:

public slots:

};

#endif // DEVICEITEM_H
