#include "DeviceItem.h"

#if defined(_MSC_VER)
#pragma  execution_character_set("utf-8")
#endif

DeviceItem::DeviceItem()
{
    this->connect = 0;
    this->updateStatus = 1;
    this->m_checked = 0;
    this->updatePercent = 0;
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
        break;
    case 1:
        this->type = value.toString();
        break;
    case 2:
        this->version = value.toString();
        break;
    case 3:{
        bool ok;
        this->updateStatus = value.toInt(&ok);
        return ok;
    }
        break;
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
        return QString("%1 无需升级 ").arg(updateStatus);
    case UpdateStatus_Need:
        return QString("%1 未升级").arg(updateStatus);
    case UpdateStatus_Waiting:
        return QString("%1 等待升级").arg(updateStatus);
    case UpdateStatus_Ready:
        return QString("%1 准备升级").arg(updateStatus);
    case UpdateStatus_Doing:
        return QString("%1 正在升级 %2%").arg(this->updateStatus).arg(updatePercent);
    case UpdateStatus_Done:
        return QString("%1 升级完成").arg(updateStatus);
    case UpdateStatus_Error:
        return QString("%1 升级失败").arg(updateStatus);
    default:
        return QString("%1").arg(updateStatus);
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

bool DeviceItem::isChecked()
{
    return m_checked;
}

void DeviceItem::setCheckState(bool state)
{
    m_checked = state;
}

bool DeviceItem::operator ==(const DeviceItem &item)
{
    return this->connect == item.connect;
}



