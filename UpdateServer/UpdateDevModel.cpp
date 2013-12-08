#include "UpdateDevModel.h"

UpdateDevModel::UpdateDevModel(const QStringList &headers, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->headerTexts = headers;
    //testData();
}

UpdateDevModel::~UpdateDevModel()
{
    qDeleteAll(items);
}

void UpdateDevModel::testData()
{
    DeviceItem * itm = new DeviceItem ;
    itm->mac = "22424324234";
    itm->type = "344";
    itm->version = "12312312";
    this->items << itm << itm;
}

bool UpdateDevModel::pushItem(DeviceItem *item)
{

    int pos = items.size();
    if (!this->insertRow(pos, QModelIndex()))
        return false;

    DeviceItem * newItem = items.value(pos);
    bool result = newItem->setData(item);
    QModelIndex left = this->createIndex(pos,0);
    QModelIndex right = this->createIndex(pos,headerTexts.size());
    if (result)
        emit dataChanged(left, right);

    return result;
}

void UpdateDevModel::changeItem(DeviceItem *item)
{

}

bool UpdateDevModel::changeItem(void *c, int updateStatus, int updatePercent)
{
    int pos = this->getDevItemPos(c);
    if(pos == -1)
        return false;
    DeviceItem * item = this->items.at(pos);

    bool toSet = item->setUpdateStatus(updateStatus,updatePercent);
    if(toSet){
        QModelIndex left = this->createIndex(pos,3);
        QModelIndex right = this->createIndex(pos,headerTexts.size());
        emit dataChanged(left, right);
    }
    return true;
}

bool UpdateDevModel::deleteItem(void *c)
{
    int pos = getDevItemPos(c);
    if(pos == -1)
        return false;
    int ret = this->removeRow(pos);
    return ret;
}


int UpdateDevModel::getDevItemPos(void *c)
{
    int pos = 0;
    for(pos = 0;pos< items.size();++pos){
        if(items.at(pos)->connect == c){
            break;
        }
    }
    if(pos == items.size())
        return -1;
    return pos;
}

int UpdateDevModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return items.size();
    else
        return 0;
}

int UpdateDevModel::columnCount(const QModelIndex &parent) const
{
    return headerTexts.size();
}

QModelIndex UpdateDevModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();


    if (!parent.isValid()){
        DeviceItem * item = items.value(row);
        return createIndex(row, column,item);
    }
}

QModelIndex UpdateDevModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant UpdateDevModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    if(index.row() < items.size()){
        DeviceItem * item = items[index.row()];
        return item->data(index.column());
    }
    return QVariant();
}

QVariant UpdateDevModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headerTexts[section];

    return QVariant();
}

bool UpdateDevModel::insertRows(int row, int count, const QModelIndex &parent)
{
    bool success = true;

    beginInsertRows(parent, row, row + count - 1);

    if (row < 0 || row > items.size())
        success = false;
    else
        for (int i = 0; i < count; ++i) {
            DeviceItem *item = new DeviceItem();
            items.insert(row+i, item);
        }

    endInsertRows();

    return success;
}

bool UpdateDevModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    DeviceItem *item = items.value(index.row());
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool UpdateDevModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool success = true;

    beginRemoveRows(parent, row, row + count - 1);
    if (row < 0 || row + count > items.size())
        success =  false;
    for (int i = 0; i < count; ++i)
        delete items.takeAt(row+i);
    endRemoveRows();

    return success;
}

int UpdateDevModel::itemsCount()
{
    return this->items.size();
}


