#include "DeviceItem.h"

#if defined(_MSC_VER)
#pragma  execution_character_set("utf-8")
#endif

DeviceItem::DeviceItem()
{
    this->connect = 0;
    this->updateStatus = 0;
}

QVariant DeviceItem::data(int column) const
{
    switch (column) {
    case 0:
        return this->mac;
    case 1:
        return this->type;
    case 2:
        return this->version;
    case 3:
    {
        QString info = getUpdateStatusInfo();
        return info;
    }
    default:
        return QVariant();
    }
}

bool DeviceItem::setData(int column, const QVariant &value)
{
    switch (column) {
    case 0:
        this->mac = value.toString();
    case 1:
        this->type = value.toString();
    case 2:
        this->version = value.toString();
    case 3:
        this->updateInfo = value.toString();
    default:
        return false;
    }
    return true;
}

bool DeviceItem::setData(DeviceItem *other)
{
    this->connect = other->connect;
    this->mac = other->mac;
    this->version = other->version;
    this->type = other->type;
    this->updateStatus = other->updateStatus;
    this->updateInfo = other->updateInfo;
    return true;
}

QString DeviceItem::getUpdateStatusInfo() const
{
    switch (this->updateStatus) {
    case UpdateStatus_None:
        return "无需升级 ";
    case UpdateStatus_Need:
        return "未升级";
    case UpdateStatus_Ready:
        return "准备升级";
    case UpdateStatus_Doing:
        return QString("正在升级 %1%").arg(updatePercent);
    case UpdateStatus_Done:
        return "升级完成";
    default:
        return "";
    }
}

bool DeviceItem::setUpdateStatus(int status, int percent)
{
    if(this->updateStatus != status){
        this->updateStatus = status;
        this->updatePercent = percent;
        return true;
    }else if(status == UpdateStatus_Doing && this->updatePercent != percent){
        this->updateStatus = status;
        this->updatePercent = percent;
        return true;
    }else
        return false;
}

bool DeviceItem::operator ==(const DeviceItem &item)
{
    return this->connect == item.connect;
}



